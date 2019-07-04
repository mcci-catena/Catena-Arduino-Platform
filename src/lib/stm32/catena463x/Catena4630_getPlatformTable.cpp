/*

Module:  Catena4630_getPlatformTable.cpp

Function:
        Catena4630::getPlatformTable()

Copyright notice:
        See accompanying LICENSE file.

Author:
        Dhinesh Kumar Pitchai, MCCI Corporation	April 2019

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena4630.h"

#include "Catena_Log.h"
#include "Catena_Platforms.h"
#include "Catena_Guids.h"

/****************************************************************************\
|
|	Read-only data.
|
\****************************************************************************/

namespace McciCatena {

const CATENA_PLATFORM gkPlatformCatena4630 =
	{
	Guid: GUID_HW_CATENA_4630_BASE(WIRE),
	pParent: &gkPlatformCatena4630,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasZMOD4410 |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasFlash
	};

const CATENA_PLATFORM (* const Catena4630::vPlatforms[]) =
	{
	// entry 0 is the default
	&gkPlatformCatena4630,
	};

const size_t Catena4630::nvPlatforms = sizeof(Catena4630::vPlatforms) / sizeof(Catena4630::vPlatforms[0]);

/*

Name:	Catena4630::getPlatformTable()

Function:
	Get the known platform table.

Definition:
	public: virtual
		void Catena4630::getPlatformTable(
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
Catena4630::getPlatformTable(
        const CATENA_PLATFORM * const * &result_vPlatforms,
        size_t &result_nvPlatforms
        )
	{
        result_vPlatforms = vPlatforms;
        result_nvPlatforms = nvPlatforms;
	}

} /* namespace McciCatena */

#endif // ARDUINO_ARCH_STM32
