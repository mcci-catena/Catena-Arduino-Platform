/*

Module:  Catena4802_getPlatformTable.cpp

Function:
        Catena4802::getPlatformTable()

Copyright notice:
        See accompanying LICENSE file.

Author:
        Dhinesh Kumar Pitchai, MCCI Corporation	November 2020

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena4802.h"

#include "Catena_Log.h"
#include "Catena_Platforms.h"
#include "Catena_Guids.h"

namespace McciCatena {

const CATENA_PLATFORM gkPlatformCatena4802 =
	{
	Guid: GUID_HW_CATENA_4802_BASE(WIRE),
	pParent: &gkPlatformCatena4802,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasFlash |
		CatenaBase::fHasRS485 |
		CatenaBase::fHasSHT3x |
		CatenaBase::fHasI2cLevelShifter
	};

const CATENA_PLATFORM (* const Catena4802::vPlatforms[]) =
	{
	// entry 0 is the default
	&gkPlatformCatena4802,
	};

const size_t Catena4802::nvPlatforms = sizeof(Catena4802::vPlatforms) / sizeof(Catena4802::vPlatforms[0]);

/*

Name:	Catena4802::getPlatformTable()

Function:
	Get the known platform table.

Definition:
	public: virtual
		void Catena4802::getPlatformTable(
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
Catena4802::getPlatformTable(
        const CATENA_PLATFORM * const * &result_vPlatforms,
        size_t &result_nvPlatforms
        )
	{
        result_vPlatforms = vPlatforms;
        result_nvPlatforms = nvPlatforms;
	}

} /* namespace McciCatena */

#endif // ARDUINO_ARCH_STM32
