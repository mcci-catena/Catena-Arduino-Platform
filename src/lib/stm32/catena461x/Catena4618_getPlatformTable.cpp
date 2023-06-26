/*

Module:  Catena4618_getPlatformTable.cpp

Function:
        Catena4618::getPlatformTable()

Copyright notice:
        See accompanying LICENSE file.

Author:
        Lakshmi Priya Natarajan, MCCI Corporation	June 2019

Revision history:
        See https://github.com/mcci-catena/Catena-Arduino-Platform

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena4618.h"

#include "Catena_Log.h"
#include "Catena_Platforms.h"
#include "Catena_Guids.h"

/****************************************************************************\
|
|	Read-only data.
|
\****************************************************************************/

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

const CATENA_PLATFORM gkPlatformCatena4618_v2 =
	{
	Guid: GUID_HW_CATENA_4618_V2_BASE(WIRE),
	pParent: &gkPlatformCatena4618,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasSHT3x |
		CatenaBase::fHasLuxLtr329 |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasFlash
	};

const CATENA_PLATFORM (* const Catena4618::vPlatforms[]) =
	{
	// entry 0 is the default
	&gkPlatformCatena4618,
	&gkPlatformCatena4618_v2,
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
