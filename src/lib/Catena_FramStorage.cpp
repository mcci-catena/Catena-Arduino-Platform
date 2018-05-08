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

#include "Catena_Guids_FramStorage.h"
#include <cstring>

using namespace McciCatena;

/****************************************************************************\
|
|	read-only data
|
\****************************************************************************/

const MCCIADK_GUID_WIRE McciCatena::cFramStorage::skFramGuid = GUID_FRAM_CATENA_V1(WIRE);

const cFramStorage::StandardItem 
McciCatena::cFramStorage::vItemDefs[cFramStorage::kMAX] =
        {
        cFramStorage::StandardItem(kHeader, sizeof(uint32_t), /* number */ true),
        cFramStorage::StandardItem(kSysEUI, sizeof(uint8_t[8]), /* number */ false),
        cFramStorage::StandardItem(kPlatformGuid, sizeof(MCCIADK_GUID_WIRE), /* number */ false),
        cFramStorage::StandardItem(kDevEUI, sizeof(uint8_t[8]), /* number */ true),
        cFramStorage::StandardItem(kAppEUI, sizeof(uint8_t[8]), /* number */ true),
        cFramStorage::StandardItem(kDevAddr, sizeof(uint32_t), /* number */ true),
        cFramStorage::StandardItem(kJoin, sizeof(uint8_t), /* number */ true),
        cFramStorage::StandardItem(kLoRaClass, sizeof(uint8_t), /* number */ true),
        cFramStorage::StandardItem(kNwkSKey, sizeof(uint8_t[16]), /* number */ false),
        cFramStorage::StandardItem(kAppSKey, sizeof(uint8_t[16]), /* number */ false),
        cFramStorage::StandardItem(kFCntDown, sizeof(uint32_t), /* number */ true),
        cFramStorage::StandardItem(kFCntUp, sizeof(uint32_t), /* number */ true),
        cFramStorage::StandardItem(kNetID, sizeof(uint32_t), /* number */ true),
        cFramStorage::StandardItem(kAppKey, sizeof(uint8_t[16]), /* number */ false),
        cFramStorage::StandardItem(kBootCount, sizeof(uint32_t), /* number */ true),
        cFramStorage::StandardItem(kOperatingFlags, sizeof(uint32_t), /* number */ true),

	/* the size field should match BSEC_MAX_STATE_BLOB_SIZE, whcich is 65 */
	cFramStorage::StandardItem(kBme680Cal, 65, /* number */ false),
        };


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

	bool cFramStorage::Object::initialize(
		cFramStorage::StandardItem item
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
McciCatena::cFramStorage::Object::initialize(
	cFramStorage::StandardItem item
	)
	{
	return this->initialize(
		cFramStorage::skFramGuid,
		item.getKey(),
		item.getSize(),
		item.isReplicated()
		);
	}

/*

Name:	McciCatena::cFramStorage::Object::matchesGuid()

Function:
	Check whether this object is identified by given GUID.

Definition:
	public: bool 
		McciCatena::cFramStorage::Object::matchesGuid(
			const MCCIADK_GUID_WIRE &guid
			) const;

Description:
	Simply compares the GUID to the one in the object. 

Returns:
	true for equality.

Notes:
	This could equally well be done by an operator= implementation
	for MCCIADK_GUID_WIRE, but that's currently a C-only header file.

	This routine does the ritual check for "standard object". Things
	would be more efficient if we had a ::StdObject type, and checked
	when we promoted the pointer.

*/

bool 
McciCatena::cFramStorage::Object::matchesGuid(
	const MCCIADK_GUID_WIRE &guid
	) const
        {
        if (! this->isStandard())
                return false;
        else
                return std::memcmp(&guid, &this->Guid, sizeof(guid)) == 0;
        }

bool
McciCatena::cFramStorage::Object::isValid() const
        {
        if (! this->isStandard())
                return false;

        if (! this->hasValidSize())
                return false;

        return true;
        }
