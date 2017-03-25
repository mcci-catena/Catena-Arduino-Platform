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

#ifndef _CATENA_PERSISTENTSTORAGE_H_
# include "Catena_PersistentStorage.h"
#endif

#ifndef _CATENA_FRAMSTORAGE_H_
# include "Catena_FramStorage.h"
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
                for (unsigned i = 0; i < cFramStorage::StandardKeys::kMAX; ++i)
                        {
                        this->m_offsetCache[i] = cFramStorage::kInvalidOffset;
                        this->m_uVerCache[i] = 0;
                        this->m_endOffset = 0;
                        }
                return true;
                }
	virtual size_t read(
		cFramStorage::Offset uOffset, uint8_t *pBuffer, size_t nBuffer
		) = 0;

	virtual void write(
		cFramStorage::Offset uOffset, const uint8_t *pBuffer, size_t nBuffer
		) = 0;

	class Cursor;

protected:
        // to save storage, we make these parallel arrays

        // saved offset of objects. If not valid, will be kInvalidOffset.
        cFramStorage::Offset m_offsetCache[cFramStorage::kMAX];

        // saved versions of objects.
        uint8_t m_uVerCache[cFramStorage::kMAX];

        // offset of first free byte in FRAM
        cFramStorage::Offset m_endOffset;


        void loadCache(void);
	};


class cFram::Cursor
	{
public:
	Cursor(cFram *pFram) : m_pFram(pFram), m_offset(cFramStorage::kInvalidOffset) {};

        // set up a cursor to match a standard item
	bool locate(const cFramStorage::StandardItem);

        // get a buffer
	bool get(uint8_t *pBuffer, size_t nBuffer);

        // get a uint32_t.
	bool getuint32(uint32_t &v)
		{
		return this->get((uint8_t *)&v, sizeof(v));
		}

        // put a buffer
	void put(const uint8_t *pBuffer, size_t nBuffer);

        // put a uint32_t
	void putuint32(uint32_t &v)
		{
		return this->put((const uint8_t *)&v, sizeof(v));
		}
	
private:
	cFram *m_pFram;
	uint16_t m_uSize;
	uint8_t m_uKey;
	uint8_t m_uVer;
	uint32_t m_offset;
	};

}; /* namespace McciCatena */

/**** end of Catena_Fram.h ****/
#endif /* _CATENA_FRAM_H_ */
