/* Catena4618_getPlatformTable.cpp	Tue Jun 18 2019 17:27:02 lakshmipriyan */

/*

Module:  Catena4618_getPlatformTable.cpp

Function:
	Catena4618::getPlatformTable()

Version:
	V0.11.0	Tue Jun 18 2019 17:27:02 lakshmipriyan	Edit level 1

Copyright notice:
	This file copyright (C) 2019 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Lakshmi Priya Natarajan, MCCI Corporation	June 2019

Revision history:
   0.11.0  Tue Jun 18 2019 17:27:02  lakshmipriyan
	Module created.

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena4618.h"

#include "Catena_Log.h"
#include "Catena_Platforms.h"
#include "Catena_Guids.h"

namespace McciCatena {

const CATENA_PLATFORM gkPlatformCatena4618 =
	{
	Guid: GUID_HW_CATENA_4618_BASE(WIRE),
	pParent: &gkPlatformCatena4618,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasSHT3x |
		CatenaBase::fHasLuxSi1133 |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasFlash
	};

const CATENA_PLATFORM (* const Catena4618::vPlatforms[]) =
	{
	// entry 0 is the default
	&gkPlatformCatena4618,
	};

const size_t Catena4618::nvPlatforms = sizeof(Catena4618::vPlatforms) / sizeof(Catena4618::vPlatforms[0]);

/*

Name:	Catena4618::getPlatformTable()

Function:
	Get the known platform table.

Definition:
	public: virtual 
		void Catena4618::getPlatformTable(
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
Catena4618::getPlatformTable(
        const CATENA_PLATFORM * const * &result_vPlatforms,
        size_t &result_nvPlatforms
        )
	{
        result_vPlatforms = vPlatforms;
        result_nvPlatforms = nvPlatforms;
	}

} /* namespace McciCatena */

#endif // ARDUINO_ARCH_STM32
