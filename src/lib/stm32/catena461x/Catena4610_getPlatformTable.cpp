/* Catena4610_getPlatformTable.cpp	Mon Nov 26 2018 17:27:02 chwon */

/*

Module:  Catena4610_getPlatformTable.cpp

Function:
	Catena4610::getPlatformTable()

Version:
	V0.12.0	Mon Nov 26 2018 17:27:02 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	November 2018

Revision history:
   0.12.0  Mon Nov 26 2018 17:27:02  chwon
	Module created.

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena4610.h"

#include "Catena_Log.h"
#include "Catena_Platforms.h"
#include "Catena_Guids.h"

namespace McciCatena {

const CATENA_PLATFORM gkPlatformCatena4610 =
	{
	Guid: GUID_HW_CATENA_4610_BASE(WIRE),
	pParent: &gkPlatformCatena4610,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLuxSi1133 |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasFlash
	};

const CATENA_PLATFORM gkPlatformCatena4610_m101 =
	{
	Guid: GUID_HW_CATENA_4610_M101(WIRE),
	pParent: &gkPlatformCatena4610,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLuxSi1133 |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasFlash |
		CatenaBase::fM101
	};

const CATENA_PLATFORM gkPlatformCatena4610_m102 =
	{
	Guid: GUID_HW_CATENA_4610_M102(WIRE),
	pParent: &gkPlatformCatena4610,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLuxSi1133 |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasSoilProbe |
		CatenaBase::fHasWaterOneWire |
		CatenaBase::fM102
	};

const CATENA_PLATFORM gkPlatformCatena4610_m103 =
	{
	Guid: GUID_HW_CATENA_4610_M103(WIRE),
	pParent: &gkPlatformCatena4610,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLuxSi1133 |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasSoilProbe |
		CatenaBase::fHasSolarPanel |
		CatenaBase::fHasWaterOneWire |
		CatenaBase::fM103
	};

const CATENA_PLATFORM gkPlatformCatena4610_m104 =
	{
	Guid: GUID_HW_CATENA_4610_M104(WIRE),
	pParent: &gkPlatformCatena4610,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLuxSi1133 |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasSoilProbe |
		CatenaBase::fHasWaterOneWire |
		CatenaBase::fM104
	};

const CATENA_PLATFORM gkPlatformCatena4610_v2 =
	{
	Guid: GUID_HW_CATENA_4610_V2_BASE(WIRE),
	pParent: &gkPlatformCatena4610,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasSHT3x |
		CatenaBase::fHasLuxLtr329 |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasFlash
	};

const CATENA_PLATFORM (* const Catena4610::vPlatforms[]) =
	{
	// entry 0 is the default
	&gkPlatformCatena4610_v2,
	&gkPlatformCatena4610,
	&gkPlatformCatena4610_m101,
	&gkPlatformCatena4610_m102,
	&gkPlatformCatena4610_m103,
	&gkPlatformCatena4610_m104,
	};

const size_t Catena4610::nvPlatforms = sizeof(Catena4610::vPlatforms) / sizeof(Catena4610::vPlatforms[0]);

/*

Name:	Catena4610::getPlatformTable()

Function:
	Get the known platform table.

Definition:
	public: virtual
		void Catena4610::getPlatformTable(
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
Catena4610::getPlatformTable(
        const CATENA_PLATFORM * const * &result_vPlatforms,
        size_t &result_nvPlatforms
        )
	{
        result_vPlatforms = vPlatforms;
        result_nvPlatforms = nvPlatforms;
	}

} /* namespace McciCatena */

#endif // ARDUINO_ARCH_STM32
