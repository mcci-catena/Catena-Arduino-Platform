/*

Module:  Catena4617_getPlatformTable.cpp

Function:
        Catena4617::getPlatformTable()

Copyright notice:
        See accompanying LICENSE file.

Author:
        Lakshmi Priya Natarajan, MCCI Corporation	June 2019

Revision history:
        See https://github.com/mcci-catena/Catena-Arduino-Platform

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena4617.h"

#include "Catena_Log.h"
#include "Catena_Platforms.h"
#include "Catena_Guids.h"

/****************************************************************************\
|
|	Read-only data.
|
\****************************************************************************/

namespace McciCatena {

const CATENA_PLATFORM gkPlatformCatena4617 =
	{
	Guid: GUID_HW_CATENA_4617_BASE(WIRE),
	pParent: &gkPlatformCatena4617,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasHS001 |
		CatenaBase::fHasLuxSi1133 |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasFlash
	};

const CATENA_PLATFORM (* const Catena4617::vPlatforms[]) =
	{
	// entry 0 is the default
	&gkPlatformCatena4617,
	};

const size_t Catena4617::nvPlatforms = sizeof(Catena4617::vPlatforms) / sizeof(Catena4617::vPlatforms[0]);

/*

Name:	Catena4617::getPlatformTable()

Function:
	Get the known platform table.

Definition:
	public: virtual
		void Catena4617::getPlatformTable(
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
Catena4617::getPlatformTable(
        const CATENA_PLATFORM * const * &result_vPlatforms,
        size_t &result_nvPlatforms
        )
	{
        result_vPlatforms = vPlatforms;
        result_nvPlatforms = nvPlatforms;
	}

} /* namespace McciCatena */

#endif // ARDUINO_ARCH_STM32
