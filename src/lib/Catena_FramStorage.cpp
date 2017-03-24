/* Catena_FramStorage.cpp	Thu Mar 23 2017 17:59:00 tmm */

/*

Module:  Catena_FramStorage.cpp

Function:
	Implementation routines for cFramStorage

Version:
	V0.5.0	Thu Mar 23 2017 17:59:00 tmm	Edit level 1

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
   0.5.0  Thu Mar 23 2017 17:59:00  tmm
	Module created.

*/

#include "Catena_FramStorage.h"

#include "Catena_Log.h"
#include <cstring>


/****************************************************************************\
|
|	Methods that are not in-line
|
\****************************************************************************/

/*

Name:	McciCatena::cFramStorage::Object::initialize()

Function:
	Initialize an FRAM storage object header.

Definition:
	bool cFramStorage::Object::initialize(
		const MCCIADK_GUID_WIRE &Guid,
		uint8_t uKey,
		size_t valueSizeInBytes,
		bool fReplicated
		);

Description:
	The object header is initialized appopriately according to the
	parameters passed in.

Returns:
	true if the parameters were valid, false if the size resulted
	in something that was impossible.

*/

bool
McciCatena::cFramStorage::Object::initialize(
	const MCCIADK_GUID_WIRE &Guid,
	uint8_t uKey,
	size_t valueSizeInBytes,
	bool fReplicated
	)
	{
        static const char FUNCTION[] = "cFramStorage::Object::initialize";

	// compute number of clicks based on value size and whether
	// there's one copy or two.
	const uint16_t nClicks = neededClicks(valueSizeInBytes, fReplicated);

	// compute the uSizeKey image
	const uint32_t uSizeKey = uSizeKey_SetDataSize(
				uSizeKey_SetClicks(0, nClicks),
				valueSizeInBytes
				);

        Log.printf(
                Log.kAlways, 
                " %s: kObjectQuantum(%u) nClicks(%u) sizeKey(nClicks)(%u)\n", 
                FUNCTION, 
                kObjectQuantum,
                nClicks,
                uSizeKey_SetClicks(0, nClicks)
                );

	// check for bad input parameters.
	if (nClicks == 0)
		return false;

	// build the object image
	this->uSizeKey = uSizeKey;
	this->Guid = Guid;
	this->uKey = uKey;
	this->uVer[0] = 0;
	this->uVer[1] = 0;
	this->uVer[2] = 0;

        return true;
	}

bool 
McciCatena::cFramStorage::Object::matchesGuid(const MCCIADK_GUID_WIRE &guid) const
        {
        if (! this->isStandard())
                return false;
        else
                return std::memcmp(&guid, &this->Guid, sizeof(guid)) == 0;
        }
