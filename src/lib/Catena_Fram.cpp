/* Catena_Fram.cpp	Fri Mar 24 2017 16:50:02 tmm */

/*

Module:  Catena_Fram.cpp

Function:
	class McciCatena::cFram implementation.

Version:
	V0.5.0	Fri Mar 24 2017 16:50:02 tmm	Edit level 1

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
   0.5.0  Fri Mar 24 2017 16:50:02  tmm
	Module created.

*/

#include "Catena_Fram.h"

using namespace McciCatena;

/*

Name:	McciCatena::cFram::Cursor::locate()

Function:
	Set up a cursor for accessing an item.

Definition:
	public: bool 
		McciCatena::cFram::Cursor::locate(
			const cFramStorage::StandardItem item
			);

Description:
	The Cursor instance is bound to the first instance of the specified
	item in the FRAM.  If the FRAM is not initialized or if the item
	cannot be found, this returns false and the cursor is left unbound.

Returns:
	true if cursor is bound and ready for I/O.

*/

bool 
McciCatena::cFram::Cursor::locate(
	const cFramStorage::StandardItem item
	)
	{
        uint8_t const uKey = item.getKey();
	if (! this->m_pFram->isReady())
		return false;

	this->m_uKey = uKey;
	this->m_uSize = item.getSize();
	this->m_offset = this->m_pFram->m_offsetCache[uKey];

	// load the cache?
	if (this->m_offset == cFramStorage::kInvalidOffset)
		{
                return false;
		}

	// load the cached version toggle
	this->m_uVer = this->m_pFram->m_uVerCache[uKey];

	return true;
	}

bool 
McciCatena::cFram::Cursor::get(
	uint8_t *pBuffer, 
	size_t nBuffer
	)
	{
	if (! this->m_pFram->isReady())
		return false;
	if (this->m_offset == cFramStorage::kInvalidOffset)
		return false;
	if (nBuffer != this->m_uSize)
		return false;

	return this->m_pFram->read(
		this->m_offset + cFramStorage::dataOffset(this->m_uSize, this->m_uVer),
                pBuffer,
                nBuffer
		);

	return false;
	}

void
McciCatena::cFram::loadCache(void)
        {
        cFramStorage::Object object;
        const MCCIADK_GUID_WIRE * const pGuid = &cFramStorage::skFramGuid;

        if (! this->isReady())
                return;

        for (cFramStorage::Offset offset = 0; 
             offset < this->m_endOffset; 
             offset += object.getObjectSize())
                {
                if (! this->read(offset, (uint8_t *)&object, sizeof(object)))
                        return;

                if (! object.isValid())
                        continue;

                const uint8_t uThisKey = object.getKey();

                // TODO(tmm@mcci.com) handle GUIDs other than skFramGuid
                if (! (object.matchesGuid(cFramStorage::skFramGuid) &&
                       uThisKey < cFramStorage::StandardKeys::kMAX))
                        continue;

                // make sure things match definition
                cFramStorage::StandardItem const item =
                        cFramStorage::vItemDefs[uThisKey];

                if (object.isReplicated() != item.isReplicated())
                        continue;

                if (object.getDataSize() != item.getSize())
                        continue;

                // take the first one.
                if (this->m_offsetCache[uThisKey] == cFramStorage::kInvalidOffset)
                        {
                        this->m_offsetCache[uThisKey] = offset;
                        this->m_uVerCache[uThisKey] = object.getCurrent();
                        }
                }
        }