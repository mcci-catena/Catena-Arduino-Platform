/*

Module:  Catena5230_getPlatformTable.cpp

Function:
        class Catena5230: CatenaBase Platform to represent a Catena 5230
        (such as the 5230).

Copyright notice:
        See accompanying LICENSE file.

Author:
        Murali, MCCI Corporation	Sep 2025

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena5230.h"

#include "Catena_Log.h"
#include "Catena_Platforms.h"
#include "Catena_Guids.h"

namespace McciCatena {

const CATENA_PLATFORM gkPlatformCatena5230 =
	{
	Guid: GUID_HW_CATENA_5230_BASE(WIRE),
	pParent: &gkPlatformCatena5230,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasLuxLtr329 |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasFlash
	PlatformFlags2:
		CatenaBase::fHasSHT4X |
		CatenaBase::fHasNpm1300 |
		CatenaBase::fHasLIS2DUXS12
	};

const CATENA_PLATFORM (* const Catena5230::vPlatforms[]) =
	{
	// entry 0 is the default
	&gkPlatformCatena5230,
	};

const size_t Catena5230::nvPlatforms = sizeof(Catena5230::vPlatforms) / sizeof(Catena5230::vPlatforms[0]);

/*

Name:	Catena5230::getPlatformTable()

Function:
	Get the known platform table.

Definition:
	public: virtual
		void Catena5230::getPlatformTable(
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
Catena5230::getPlatformTable(
        const CATENA_PLATFORM * const * &result_vPlatforms,
        size_t &result_nvPlatforms
        )
	{
        result_vPlatforms = vPlatforms;
        result_nvPlatforms = nvPlatforms;
	}

} /* namespace McciCatena */

#endif // ARDUINO_ARCH_STM32
