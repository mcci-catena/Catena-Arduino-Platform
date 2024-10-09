/*

Module:  Catena_Fram_commands.cpp

Function:
        McciCatena::cFram::addCommands() and command processors.

Copyright notice:
        See accompanying LICENSE file.

Author:
        Terry Moore, MCCI Corporation	March 2017

*/

#include "Catena_FramRingBuf.h"

#include "CatenaBase.h"

using namespace McciCatena;

/*

Name:	McciCatena::FramRingBuffer_t::initializeFromFram()

Function:
        Initialize the in-memory copy of the log buffer control
        structure from FRAM.

Definition:
        bool McciCatena::FramRingBuffer_t::initializeFromFram(
                uint8_t version,                // the item version number
                size_type itemsize,             // the item size
                OverflowPolicy_t policy         // the overflow policy to use
                );

Description:
        If not yet inititialized, read the log header from FRAM. If
        not found, create one; otherwise either use what we read or
        just initailize (due to code changes). Make sure there's a log
        buffer in the FRAM.

Returns:
        true if successful, false otherwise.

Notes:


*/

#define FUNCTION "FramRingBuffer_t::initializeFromFram"

bool
FramRingBuffer_t::initializeFromFram(
        uint8_t version, size_type base_itemsize, OverflowPolicy_t policy
        )
        {
        auto const pCatena = CatenaBase::pCatenaBase;
        const size_t itemsize = base_itemsize + sizeof(sequence_type);
	auto pFram = pCatena->getFram();
        bool fMustInitializeLogHeader = false;

        if (! this->m_logBufferCursor.isinitialized())
                {
                auto const r1 = this->m_logBufferCursor.initialize(pFram);
                auto const r2 = this->m_logHeaderCursor.initialize(pFram);

                if (! (r1 && r2))
                        return false;

                // point it at the log buffer if it exists
                this->m_logBufferCursor.locate(cFramStorage::StandardKeys::kDataLogBuffer);
                this->m_logHeaderCursor.locate(cFramStorage::StandardKeys::kDataLogHeader);
                }

        // start by establishing that there is a log-buffer object
        if (! this->m_logBufferCursor.islocated())
                {
                // buffer object doesn't exist!  Try to create it.
                if (! (this->m_logBufferCursor.create() && this->m_logHeaderCursor.create()))
                        return false;
                
                // created log buffer sucessfully: we therfore need to initialize the
                // on-FRAM header from an empty header
                fMustInitializeLogHeader = true;
                }
        // buffer object exists, what about the header?
        else if (! this->m_logHeaderCursor.islocated())
                {
                if (! this->m_logHeaderCursor.create())
                        return false;

                // created log header successfully -- must initailize
                fMustInitializeLogHeader = true;
                }
        // header exists: read it and validate it.
        else
                fMustInitializeLogHeader = false;


        cFramStorage::DataLogHeader_t framHeader;
        
        if (! fMustInitializeLogHeader)
                {
                // read it.
                do      {
                        if (! this->m_logHeaderCursor.get((uint8_t *)&framHeader, sizeof(framHeader)))
                                {
                                // failed. must initialize.
                                fMustInitializeLogHeader = true;
                                break;
                                }

                        // verify
                        if (! framHeader.queryConsistent())
                                {
                                // failed. must initialize.
                                fMustInitializeLogHeader = true;
                                break;
                                }
                        
                        // compare to our parameters.
                        if (framHeader.queryVersionMatch(
                                version,
                                cFramStorage::kDataLogBufferSize,
                                itemsize))
                                {
                                // failed: must initialize.
                                }
                        } while (false);
                }

        if (fMustInitializeLogHeader)
                {
                if (! this->m_logheader.initialize(version, this->getBufferSize(), itemsize, policy))
                        return false;
                else if (! this->m_logHeaderCursor.put(this->m_logheader))
                        return false;
                else
                        return true;
                }
        else
                {
                // just set the header
                this->m_logheader = framHeader;
                return true;
                }
        }

#undef FUNCTION

/*

Name:	FramRingBuffer_t::put_tail()

Function:
        Append an object to the end of the ring buffer, if possible.

Definition:
        bool FramRingBuffer_t::put_tail(
                sequence_type sequenceNumber,
                const uint8_t *pBuffer,
                size_type nBuffer
                );

Description:
        A slot is allocated for an item, and the item is appended to the buffer. The
        data is written before we update the ring buffer pointers, so that a failure
        while updating just results in the data being discarded.

Returns:
        true if data was written, false otherwise.

Notes:
        This is a protected method; typically only the abstract template
        can call this. Regardless of the value of nBuffer, at most itemsize
        bytes will be written. If nBuffer is really variable length, then
        it must be self-sizing.

        If overwriting old entries, we must first delete the old head and
        confirm the deletion. Then we can append.

        This call advances the sequence number.

*/

#define FUNCTION "FramRingBuffer_t::put_tail"

bool
FramRingBuffer_t::put_tail(
        sequence_type sequenceNumber,
        const uint8_t *pBuffer,
        size_type nBuffer
        )
        {
        const size_type max_buffer = this->m_logheader.queryItemSize() - sizeof(sequence_type);

        if (nBuffer > max_buffer)
                nBuffer = max_buffer;
        
        size_type itemIndex;
        if (! this->m_logheader.push_back(&itemIndex))
                {
                // no room, discarding this one.
                if (this->m_logheader.queryOverflowPolicy() == cFramStorage::DataLogHeader_t::OverflowPolicy_t::kDropNew)
                        return false;

                // otherwise: make an empty slot, and commit so that a crash while writing
                // new entry won't result in a partially corrupt but "valid" entry.
                if (! (this->m_logheader.pop_front() /* delete head */ &&
                       this->m_logHeaderCursor.put(this->m_logheader) /* commit */ &&
                       this->m_logheader.push_back(&itemIndex)) /* reserve space */)
                        return false;
                }

        // convert itemIndex to byte index
        auto const byteIndex = this->m_logheader.indexToOffset(itemIndex);

        //
        // write the sequence number
        // force little-endian. Throw a compile error if
        // we decide to change size of sequence_type.
        //
        bool fResult;
        static_assert(sizeof(sequence_type) == 2);
        uint8_t sequence_buffer[2];

        sequence_buffer[0] = uint8_t(sequenceNumber & 0xFF);
        sequence_buffer[1] = uint8_t(sequenceNumber >> 8);

        fResult = this->m_logBufferCursor.putPartialValue(byteIndex, sequence_buffer, sizeof(sequence_buffer));

        // write the buffer
        if (fResult)
                fResult = this->m_logBufferCursor.putPartialValue(
                                byteIndex + sizeof(sequenceNumber),
                                pBuffer, nBuffer
                                );

        if (fResult)
                {
                // write the pointers, committing the new item.
                fResult = this->m_logHeaderCursor.put(this->m_logheader);
                }

        return fResult;
        }

#undef FUNCTION

/*

Name:	FramRingBuffer_t::peek_front()

Function:
        Read an entry from the queue, indexing from head == 0.

Definition:
        bool FramRingBuffer_t::peek_front(
                sequence_type &sequenceNumber
                uint8_t *pBuffer,
                size_type nBuffer,
                size_type iEntry // defaults to zero
                );

Description:
        If the specified entry exists, its contents are read from the FRAM
        into the user's buffer. The ring buffer pointers are not chagned.

Returns:
        true for success, false for failure.

Notes:
        peek_front() is named followign <deque> terminology. 

*/

#define FUNCTION "FramRingBuffer_t::peek_front"

bool
FramRingBuffer_t::peek_front(
        sequence_type &sequenceNumber,
        uint8_t *pBuffer,
        size_type nBuffer,
        size_type iEntry // defaults to zero
        )
        {
        bool fResult;
        static_assert(sizeof(sequence_type) == 2);
        uint8_t sequence_buffer[2];

        if (iEntry >= this->m_logheader.size())
                return false;
        
        size_type const iItem = this->m_logheader.absoluteIndex(iEntry);
        size_type const byteIndex = this->m_logheader.indexToOffset(iItem);

        fResult = this->m_logBufferCursor.getPartialValue(sequence_buffer, sizeof(sequence_buffer), byteIndex);
        if (fResult)
                fResult = this->m_logBufferCursor.getPartialValue(pBuffer, nBuffer, byteIndex + sizeof(sequence_buffer));

        if (fResult)
                sequenceNumber = sequence_buffer[0] + (sequence_buffer[1] << 8);
        
        return fResult;
        }

#undef FUNCTION

/*

Name:	FramRingBuffer_t::newSequenceNumber()

Function:
        Assign a new sequence number and record the decision.

Definition:
        bool FramRingBuffer_t::newSequenceNumber(
                FramRingBuffer_t::sequence_type &sequenceNumber
                );

Description:
        A sequence number is issued, updating FRAM so that the sequence number
        can't be immediately reused.

Returns:
        This function returns true if the sequence number was successfully
        assigned, false otherwise. If false, then sequenceNumber is not changed.

Notes:
        

*/

#define FUNCTION "FramRingBuffer_t::newSequenceNumber"

bool
FramRingBuffer_t::newSequenceNumber(
        FramRingBuffer_t::sequence_type &sequenceNumber
        )
        {
        auto const newSequenceNumber = this->m_logheader.querySequenceNumber();

        this->m_logheader.advanceSequence();
        if (! this->m_logHeaderCursor.put(this->m_logheader))
                {
                this->m_logheader.setSequenceNumber(newSequenceNumber);
                return false;
                }
        else
                {
                sequenceNumber = newSequenceNumber;
                return true;
                }
        }

#undef FUNCTION

/*

Name:	FramRingBuffer_t::clear()

Function:
        Clear and save the ring buffer header.

Definition:
        bool FramRingBuffer_t::clear(
                void
                );

Description:
        The ring buffer pointers are re-initialized. Sequence number is not
        changed

Returns:
        This function returns true if and only if the header was successfully
        saved to FRAM.

Notes:


*/

#define FUNCTION "FramRingBuffer_t::clear"

bool
FramRingBuffer_t::clear(
        void
        )
        {
        this->m_logheader.clear();
        return this->m_logHeaderCursor.put(this->m_logheader);
        }

#undef FUNCTION
