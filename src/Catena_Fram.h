/* Catena_Fram.h	Fri Mar 24 2017 14:53:52 tmm */

/*

Module:  Catena_Fram.h

Function:
	FRAM of various sizes and connectivities.

Version:
	V0.5.0	Fri Mar 24 2017 14:53:52 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	March 2017

Revision history:
   0.5.0  Fri Mar 24 2017 14:53:52  tmm
	Module created.

*/

#ifndef _CATENA_FRAM_H_		/* prevent multiple includes */
#define _CATENA_FRAM_H_

#pragma once

#ifndef _CATENA_COMMANDSTREAM_H_
# include "Catena_CommandStream.h"
#endif

#ifndef _CATENA_FRAMSTORAGE_H_
# include "Catena_FramStorage.h"
#endif

#ifndef _CATENA_PERSISTENTSTORAGE_H_
# include "Catena_PersistentStorage.h"
#endif

namespace McciCatena {

// this is a dummy layer for refactoring later
class cFram : public cPersistentStorage
	{
protected:
	using Super = cPersistentStorage;

public:
        virtual bool begin() override 
                {
                this->resetCache();
                this->m_endOffset = 0;
                this->m_fReady = false;
                return true;
                }

        // initialize the store
        virtual bool initialize() override;

        // check the store
        virtual bool isValid() override;

        // reset the store
        virtual bool reset() override;

        // read from the store.
	virtual size_t read(
		cFramStorage::Offset uOffset, uint8_t *pBuffer, size_t nBuffer
		) = 0;

        // write to the store
	virtual bool write(
		cFramStorage::Offset uOffset, const uint8_t *pBuffer, size_t nBuffer
		) = 0;

        // return the unformatted size of the store.
        virtual cFramStorage::Offset getsize() const = 0;

        // return state of the FRAM system
        virtual bool isReady() const override { return this->m_fReady; }

        virtual void invalidate() override { this->m_fReady = false; }

	class Cursor;

	// initialize the commands
	bool addCommands();

        // implement the dump command
        static cCommandStream::CommandStatus doDump(
                        cCommandStream *pThis,
                        void *pContext,
                        int argc,
                        char **argv
                        );

        // implement the reset command
        static cCommandStream::CommandStatus doReset(
                        cCommandStream *pThis,
                        void *pContext,
                        int argc,
                        char **argv
                        );

        void saveField(
                cFramStorage::StandardKeys uKey,
                const uint8_t *pValue,
                size_t nValue
                );

        template <typename T>
        void saveField(
                cFramStorage::StandardKeys uKey,
                const T &field
                )
                {
                this->saveField(uKey, (const uint8_t *)&field, sizeof(field));
                };

        bool getField(
                cFramStorage::StandardKeys uKey,
                uint8_t *pValue,
                size_t nValue
                );

        template <typename T>
        bool getField(
                cFramStorage::StandardKeys uKey,
                T &field
                )
                {
                return this->getField(uKey, (uint8_t *)&field, sizeof(field));
                };


protected:
        // record the "ready status"
        bool                    m_fReady;

        // to save storage, we make these parallel arrays

        // saved offset of objects. If not valid, will be kInvalidOffset.
        cFramStorage::Offset m_offsetCache[cFramStorage::kMAX];

        // saved versions of objects.
        uint8_t m_uVerCache[cFramStorage::kMAX];

        // offset of first free byte in FRAM
        cFramStorage::Offset m_endOffset;

        // load the cache from the media.
        void loadCache(void);

        // forget the cache contents.
        void resetCache(void);

        // set a specific entry.
        void setCacheEntry(cFramStorage::StandardKeys, cFramStorage::Offset, uint8_t uVer);

        cFramStorage::Offset allocate(size_t nBytes);

        bool writeItemData(
                cFramStorage::StandardItem item,
                const uint8_t *pBuffer,
                size_t nBuffer
                );

	};


class cFram::Cursor
	{
public:
        // the basic constructor just fills things in.
	Cursor(cFram *pFram) 
                : m_pFram(pFram), 
                  m_uKey(cFramStorage::StandardKeys::kMAX), 
                  m_offset(cFramStorage::kInvalidOffset) 
                {};

        // construct and locate
        Cursor(cFram *pFram, cFramStorage::StandardItem item);

        // construct and locate using key
        Cursor(cFram *pFram, cFramStorage::StandardKeys uKey) 
                : Cursor{pFram, cFramStorage::vItemDefs[uKey]} {};

        // take a located cursor and create an object if needed
        bool create(void);

        // test whether a cursro is bound to an object
        bool isbound() const
                {
                return this->m_uKey != cFramStorage::StandardKeys::kMAX;
                }

        // test whether a cursor was located
        bool islocated() const
                {
                return this->m_offset != cFramStorage::kInvalidOffset;
                }

        // set up a cursor to match a standard item
	bool locate(const cFramStorage::StandardItem);

        bool locate(cFramStorage::StandardKeys uKey)
                {
                return this->locate(cFramStorage::vItemDefs[uKey]);
                }

        // return the standard item associated with this cursor
        bool getitem(cFramStorage::StandardItem &) const;

        // get the item size associated with this cursor
        size_t getitemsize() const;

        // get a buffer
	bool get(uint8_t *pBuffer, size_t nBuffer);

        // get a uint32_t.
	bool getuint32(uint32_t &v)
		{
		return this->get((uint8_t *)&v, sizeof(v));
		}

        // put a buffer
	bool put(const uint8_t *pBuffer, size_t nBuffer);

        // put a uint32_t
	bool putuint32(uint32_t &v)
		{
		return this->put((const uint8_t *)&v, sizeof(v));
		}

        // parse a value
        bool parsevalue(
                const char *pValue,
                uint8_t *pData,
                size_t nData
                ) const;

        size_t formatvalue(
                char *pBuffer,
                size_t nBuffer,
                size_t iBuffer,
                const uint8_t *pData,
                size_t nData
                ) const;
	
private:
        cFramStorage::StandardItem getItem()
                {
                return cFramStorage::vItemDefs[this->m_uKey];
                };

	cFram *m_pFram;
	uint16_t m_uSize;
	uint8_t m_uKey;
	uint8_t m_uVer;
	uint32_t m_offset;
	};

}; /* namespace McciCatena */

/**** end of Catena_Fram.h ****/
#endif /* _CATENA_FRAM_H_ */
