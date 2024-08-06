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
        uint8_t version, size_type itemsize, OverflowPolicy_t policy
        )
        {
        auto const pCatena = CatenaBase::pCatenaBase;
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
                const uint8_t *pBuffer,
                size_type nBuffer
                );

Description:
        A slot is allocated for an item, and the item is appended to the buffer. The
        data is written before we update the ring buffer pointers, so that a failure
        while updating just results in the data being discarded.

Returns:
        true if data was written, false otehrwise.

Notes:
        This is a protected method; typically only the abstract template
        can call this. Regardless of the value of nBuffer, at most itemsize
        bytes will be written. If nBuffer is really variable length, then
        it must be self-sizing.

        If overwriting old entries, we must first delete the old head and
        confirm the deletion. Then we can append.

*/

#define FUNCTION "FramRingBuffer_t::put_tail"

bool
FramRingBuffer_t::put_tail(
        const uint8_t *pBuffer,
        size_type nBuffer
        )
        {
        if (nBuffer > this->m_logheader.queryItemSize())
                nBuffer = this->m_logheader.queryItemSize();
        
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

        // write the buffer
        auto fResult = this->m_logBufferCursor.putPartialValue(byteIndex, pBuffer, nBuffer);

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
        uint8_t *pBuffer,
        size_type nBuffer,
        size_type iEntry // defaults to zero
        )
        {
        if (iEntry >= this->m_logheader.size())
                return false;
        
        size_type const iItem = this->m_logheader.absoluteIndex(iEntry);
        size_type const byteIndex = this->m_logheader.indexToOffset(iItem);

        return this->m_logBufferCursor.getPartialValue(pBuffer, nBuffer, byteIndex);
        }

#undef FUNCTION
