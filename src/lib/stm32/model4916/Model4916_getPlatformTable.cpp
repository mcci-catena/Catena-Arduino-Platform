/*

Module:  Model4916_getPlatformTable.cpp

Function:
        Model4916::getPlatformTable()

Copyright notice:
        See accompanying LICENSE file.

Author:
        Pranau R, MCCI Corporation	November 2022

*/

#ifdef ARDUINO_ARCH_STM32

#include "Model4916.h"

#include "Catena_Log.h"
#include "Catena_Platforms.h"
#include "Catena_Guids.h"

/****************************************************************************\
|
|	Read-only data.
|
\****************************************************************************/

namespace McciCatena {

const CATENA_PLATFORM gkPlatformModel4916 =
	{
	Guid: GUID_HW_MODEL_4916_BASE(WIRE),
	pParent: &gkPlatformModel4916,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasSHT3x |
		CatenaBase::fHasBme680 |
		CatenaBase::fHasADS131M04 |
		CatenaBase::fHasSAMM8Q |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasFlash
	};

const CATENA_PLATFORM (* const Model4916::vPlatforms[]) =
	{
	// entry 0 is the default
	&gkPlatformModel4916,
	};

const size_t Model4916::nvPlatforms = sizeof(Model4916::vPlatforms) / sizeof(Model4916::vPlatforms[0]);

/*

Name:	Model4916::getPlatformTable()

Function:
	Get the known platform table.

Definition:
	public: virtual
		void Model4916::getPlatformTable(
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
Model4916::getPlatformTable(
        const CATENA_PLATFORM * const * &result_vPlatforms,
        size_t &result_nvPlatforms
        )
	{
        result_vPlatforms = vPlatforms;
        result_nvPlatforms = nvPlatforms;
	}

} /* namespace McciCatena */

#endif // ARDUINO_ARCH_STM32
