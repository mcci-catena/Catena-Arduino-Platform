/*

Module:  Model4931_getPlatformTable.cpp

Function:
        Model4931::getPlatformTable()

Copyright notice:
        See accompanying LICENSE file.

Author:
        Pranau R, MCCI Corporation	November 2023

*/

#ifdef ARDUINO_ARCH_STM32

#include "Model4931.h"

#include "Catena_Log.h"
#include "Catena_Platforms.h"
#include "Catena_Guids.h"

/****************************************************************************\
|
|	Read-only data.
|
\****************************************************************************/

namespace McciCatena {

const CATENA_PLATFORM gkPlatformModel4931 =
	{
	Guid: GUID_HW_MODEL_4931_BASE(WIRE),
	pParent: &gkPlatformModel4931,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasSHT3x |
		CatenaBase::fHasRS485 |
		CatenaBase::fHasSolarPanel |
		CatenaBase::fHasWaterOneWire |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasFlash,
        PlatformFlags2:
		CatenaBase::fHasADS1015 |
		CatenaBase::fHasBMP581
	};

const CATENA_PLATFORM (* const Model4931::vPlatforms[]) =
	{
	// entry 0 is the default
	&gkPlatformModel4931,
	};

const size_t Model4931::nvPlatforms = sizeof(Model4931::vPlatforms) / sizeof(Model4931::vPlatforms[0]);

/*

Name:   Model4931::getPlatformTable()

Function:
        Get the known platform table.

Definition:
        public: virtual
            void Model4931::getPlatformTable(
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
Model4931::getPlatformTable(
        const CATENA_PLATFORM * const * &result_vPlatforms,
        size_t &result_nvPlatforms
        )
	{
        result_vPlatforms = vPlatforms;
        result_nvPlatforms = nvPlatforms;
	}

} /* namespace McciCatena */

#endif // ARDUINO_ARCH_STM32