/*

Module:  Catena5220_getPlatformTable.cpp

Function:
        Catena5220::getPlatformTable()

Copyright notice:
        See accompanying LICENSE file.

Author:
        Pranau Ravikumar, MCCI Corporation	August 2026

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena5220.h"

#include "Catena_Log.h"
#include "Catena_Platforms.h"
#include "Catena_Guids.h"

/****************************************************************************\
|
|	Read-only data.
|
\****************************************************************************/

namespace McciCatena {

const CATENA_PLATFORM gkPlatformCatena5220 =
	{
	Guid: GUID_HW_CATENA_5220_BASE(WIRE),
	pParent: &gkPlatformCatena5220,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasRS485 |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasFlash,
	PlatformFlags2:
		CatenaBase::fHasSHT4X |
		CatenaBase::fHasLIS2DUXS12
	};

const CATENA_PLATFORM (* const Catena5220::vPlatforms[]) =
	{
	// entry 0 is the default
	&gkPlatformCatena5220,
	};

const size_t Catena5220::nvPlatforms = sizeof(Catena5220::vPlatforms) / sizeof(Catena5220::vPlatforms[0]);

/*

Name:	Catena5220::getPlatformTable()

Function:
	Get the known platform table.

Definition:
	public: virtual
		void Catena5220::getPlatformTable(
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
Catena5220::getPlatformTable(
        const CATENA_PLATFORM * const * &result_vPlatforms,
        size_t &result_nvPlatforms
        )
	{
        result_vPlatforms = vPlatforms;
        result_nvPlatforms = nvPlatforms;
	}

} /* namespace McciCatena */

#endif // ARDUINO_ARCH_STM32