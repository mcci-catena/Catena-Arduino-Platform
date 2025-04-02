/*

Module:  Model4933_getPlatformTable.cpp

Function:
        Model4933::getPlatformTable()

Copyright notice:
        See accompanying LICENSE file.

Author:
        Pranau R, MCCI Corporation	October 2023

*/

#ifdef ARDUINO_ARCH_STM32

#include "Model4933.h"

#include "Catena_Log.h"
#include "Catena_Platforms.h"
#include "Catena_Guids.h"

/****************************************************************************\
|
|	Read-only data.
|
\****************************************************************************/

namespace McciCatena {

const CATENA_PLATFORM gkPlatformModel4933 =
	{
	Guid: GUID_HW_MODEL_4933_BASE(WIRE),
	pParent: &gkPlatformModel4933,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasSHT3x |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasFlash,
        PlatformFlags2:
		CatenaBase::fHasADS131M04 |
		CatenaBase::fHasBMP581
	};

const CATENA_PLATFORM (* const Model4933::vPlatforms[]) =
	{
	// entry 0 is the default
	&gkPlatformModel4933,
	};

const size_t Model4933::nvPlatforms = sizeof(Model4933::vPlatforms) / sizeof(Model4933::vPlatforms[0]);

/*

Name:   Model4933::getPlatformTable()

Function:
        Get the known platform table.

Definition:
        public: virtual
            void Model4933::getPlatformTable(
                const CATENA_PLATFORM * const * &vPlatforms,
                size_t &nvPlatforms
                ) override;

Description:
        This override for getPlatformTable() returns the vector of platform
        GUIDs for this Model.

Returns:
        vPlatforms is set to the base of the array of pointers to platform
        stuctures; and nvPlatforms is set to the number of entries in
        the table.

*/

/* public virtual override */
void
Model4933::getPlatformTable(
        const CATENA_PLATFORM * const * &result_vPlatforms,
        size_t &result_nvPlatforms
        )
	{
        result_vPlatforms = vPlatforms;
        result_nvPlatforms = nvPlatforms;
	}

} /* namespace McciCatena */

#endif // ARDUINO_ARCH_STM32
