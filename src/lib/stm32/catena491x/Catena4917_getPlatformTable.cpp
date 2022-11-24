/*

Module:  Catena4917_getPlatformTable.cpp

Function:
        Catena4917::getPlatformTable()

Copyright notice:
        See accompanying LICENSE file.

Author:
        Pranau R, MCCI Corporation	November 2022

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena4917.h"

#include "Catena_Log.h"
#include "Catena_Platforms.h"
#include "Catena_Guids.h"

/****************************************************************************\
|
|	Read-only data.
|
\****************************************************************************/

namespace McciCatena {

const CATENA_PLATFORM gkPlatformCatena4917 =
	{
	Guid: GUID_HW_CATENA_4917_BASE(WIRE),
	pParent: &gkPlatformCatena4917,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasLIS2HH12 |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasFlash
	};

const CATENA_PLATFORM (* const Catena4917::vPlatforms[]) =
	{
	// entry 0 is the default
	&gkPlatformCatena4917,
	};

const size_t Catena4917::nvPlatforms = sizeof(Catena4917::vPlatforms) / sizeof(Catena4917::vPlatforms[0]);

/*

Name:	Catena4917::getPlatformTable()

Function:
	Get the known platform table.

Definition:
	public: virtual
		void Catena4917::getPlatformTable(
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
Catena4917::getPlatformTable(
        const CATENA_PLATFORM * const * &result_vPlatforms,
        size_t &result_nvPlatforms
        )
	{
        result_vPlatforms = vPlatforms;
        result_nvPlatforms = nvPlatforms;
	}

} /* namespace McciCatena */

#endif // ARDUINO_ARCH_STM32
