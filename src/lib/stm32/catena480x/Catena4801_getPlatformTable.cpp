/* Catena4801_getPlatformTable.cpp	Mon Nov 26 2018 17:27:02 chwon */

/*

Module:  Catena4801_getPlatformTable.cpp

Function:
	Catena4801::getPlatformTable()

Version:
	V0.12.0	Mon Nov 26 2018 17:27:02 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	November 2018

Revision history:
   0.12.0  Mon Nov 26 2018 17:27:02  chwon
	Module created.

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena4801.h"

#include "Catena_Log.h"
#include "Catena_Platforms.h"
#include "Catena_Guids.h"

namespace McciCatena {

const CATENA_PLATFORM gkPlatformCatena4801 =
	{
	Guid: GUID_HW_CATENA_4801_BASE(WIRE),
	pParent: &gkPlatformCatena4801,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasFlash |
		CatenaBase::fHasRS485
	};

const CATENA_PLATFORM (* const Catena4801::vPlatforms[]) =
	{
	// entry 0 is the default
	&gkPlatformCatena4801,
	};

const size_t Catena4801::nvPlatforms = sizeof(Catena4801::vPlatforms) / sizeof(Catena4801::vPlatforms[0]);

/*

Name:	Catena4801::getPlatformTable()

Function:
	Get the known platform table.

Definition:
	public: virtual
		void Catena4801::getPlatformTable(
                        const CATENA_PLATFORM * const * &vPlatforms,
                        size_t &nvPlatforms
                        ) override;

Description:
	This override for getPlatformTable() returns the vector of platform
        GUIDs for this Catena.

Returns:
	vPlatforms is set to the base of the array of pointers to platform
        stuctures; and nvPlatforms is set to the number of entries in
        the table.

*/

/* public virtual override */
void
Catena4801::getPlatformTable(
        const CATENA_PLATFORM * const * &result_vPlatforms,
        size_t &result_nvPlatforms
        )
	{
        result_vPlatforms = vPlatforms;
        result_nvPlatforms = nvPlatforms;
	}

} /* namespace McciCatena */

#endif // ARDUINO_ARCH_STM32
