/*

Module:	Catena_FramRingBuf.h

Function:
        FRAM-based non-volatile ring buffer templates

Copyright and License:
        This file copyright (C) 2024 by

                MCCI Corporation
                3520 Krums Corners Road
                Ithaca, NY  14850

        See accompanying LICENSE file for copyright and license information.

Author:
        Terry Moore, MCCI Corporation	August 2024

*/

#ifndef _Catena_FramRingBuf_h_
#define _Catena_FramRingBuf_h_	/* prevent multiple includes */

#pragma once

#ifndef _Catena_FramStorage_h_
# include "Catena_FramStorage.h"
#endif

#ifndef _Catena_Fram_h_
# include "Catena_Fram.h"
#endif


namespace McciCatena {

class FramRingBuffer_t;

class FramRingBuffer_t
        {
public:
        using OverflowPolicy_t = cFramStorage::DataLogHeader_t::OverflowPolicy_t;
        using size_type = cFramStorage::DataLogHeader_t::size_type;
        using sequence_type = cFramStorage::DataLogHeader_t::sequence_type;

	// neither copyable nor movable
	FramRingBuffer_t(const FramRingBuffer_t&) = delete;
	FramRingBuffer_t& operator=(const FramRingBuffer_t&) = delete;
	FramRingBuffer_t(const FramRingBuffer_t&&) = delete;
	FramRingBuffer_t& operator=(const FramRingBuffer_t&&) = delete;

        // constructor
        FramRingBuffer_t(uint8_t version, size_type itemsize, OverflowPolicy_t policy)
                : m_logheader(version, this->getBufferSize(), itemsize, policy)
                {}

protected:
        bool initializeFromFram(uint8_t version, size_type itemsize, OverflowPolicy_t policy);
        bool put_tail(sequence_type seqnum, const uint8_t *pBuffer, size_type nBuffer);
        bool peek_front(sequence_type &seqnum, uint8_t *pBuffer, size_type nBuffer, size_type iEntry = 0);
        bool pop_front();

public:
        size_type size() const
                {
                return this->m_logheader.size() - sizeof(sequence_type);
                }

        size_type getBufferSize() const
                {
                return cFramStorage::kDataLogBufferSize;
                }
 
        bool newSequenceNumber(sequence_type &sequenceNumber);

        bool clear();

private:
        cFramStorage::DataLogHeader_t   m_logheader;
        cFram::Cursor   m_logBufferCursor {nullptr};
        cFram::Cursor   m_logHeaderCursor {nullptr};
        };

template <class cDataItem>
class RecoverableUplinkQueue_t : public FramRingBuffer_t
        {
public:
	// neither copyable nor movable
	RecoverableUplinkQueue_t(const RecoverableUplinkQueue_t&) = delete;
	RecoverableUplinkQueue_t& operator=(const RecoverableUplinkQueue_t&) = delete;
	RecoverableUplinkQueue_t(const RecoverableUplinkQueue_t&&) = delete;
	RecoverableUplinkQueue_t& operator=(const RecoverableUplinkQueue_t&&) = delete;

        RecoverableUplinkQueue_t(uint8_t version, OverflowPolicy_t policy)
                : FramRingBuffer_t(version, sizeof(cDataItem), policy)
                {}

        bool put_tail(sequence_type sequenceNumber, cDataItem &refItem)
                {
                return this->FramRingBuffer_t::put_tail(sequenceNumber, (const uint8_t *)&refItem, sizeof(refItem));
                }
        
        bool peek_front(sequence_type &sequenceNumber, cDataItem &refItem, size_type iEntry = 0)
                {
                return this->FramRingBuffer_t::peek_front(sequenceNumber, (uint8_t *)&refItem, sizeof(refItem), iEntry);
                }

        bool initializeFromFram(uint8_t version)
                {
                return this->FramRingBuffer_t::initializeFromFram(version, sizeof(cDataItem), OverflowPolicy_t::kDropOldest);
                };
        };

} // namespace McciCatena

#endif /* _Catena_FramRingBuf_h_ */
