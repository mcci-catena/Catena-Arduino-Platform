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

	// neither copyable nor movable
	FramRingBuffer_t(const FramRingBuffer_t&) = delete;
	FramRingBuffer_t& operator=(const FramRingBuffer_t&) = delete;
	FramRingBuffer_t(const FramRingBuffer_t&&) = delete;
	FramRingBuffer_t& operator=(const FramRingBuffer_t&&) = delete;

        // constructor
        FramRingBuffer_t(uint8_t version, size_type itemsize, OverflowPolicy_t policy)
                : m_logheader(version, this->getBufferSize(), itemsize, policy)
                {}

        bool initializeFromFram(uint8_t version, size_type itemsize, OverflowPolicy_t policy);

protected:
        bool put_tail(const uint8_t *pBuffer, size_type nBuffer);
        bool peek_front(uint8_t *pBuffer, size_type nBuffer, size_type iEntry = 0);

public:
        bool pop_front();
        size_type queryDropped() const
                {
                return this->m_logheader.queryDropped();
                }
        size_type adjustDropped(size_type adjustment)
                {
                return this->m_logheader.adjustDropped(adjustment);
                }

        size_type size() const
                {
                return this->m_logheader.size();
                }

        size_type getBufferSize() const
                {
                return cFramStorage::kDataLogBufferSize;
                }
 
        void clear();

private:
        cFramStorage::DataLogHeader_t   m_logheader;
        cFram::Cursor   m_logBufferCursor {nullptr};
        cFram::Cursor   m_logHeaderCursor {nullptr};
        };

template <class cDataItem>
class AbstractRingBuffer_t : public FramRingBuffer_t
        {
public:
	// neither copyable nor movable
	AbstractRingBuffer_t(const AbstractRingBuffer_t&) = delete;
	AbstractRingBuffer_t& operator=(const AbstractRingBuffer_t&) = delete;
	AbstractRingBuffer_t(const AbstractRingBuffer_t&&) = delete;
	AbstractRingBuffer_t& operator=(const AbstractRingBuffer_t&&) = delete;

        AbstractRingBuffer_t(uint8_t version, OverflowPolicy_t policy)
                : FramRingBuffer_t(version, sizeof(cDataItem), policy)
                {}

        bool put_tail(const cDataItem &refItem)
                {
                return this->FramRingBuffer_t::put_tail((const uint8_t *)&refItem, sizeof(refItem));
                }
        
        bool peek_front(cDataItem &refItem, size_type iEntry = 0)
                {
                return this->FramRingBuffer_t::peek_front((uint8_t *)&refItem, sizeof(refItem), iEntry);
                }
        };

} // namespace McciCatena

#endif /* _Catena_FramRingBuf_h_ */
