/* Catena4611_getPlatformTable.cpp	Mon Nov 26 2018 17:27:02 chwon */

/*

Module:  Catena4611_getPlatformTable.cpp

Function:
	Catena4611::getPlatformTable()

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

#include "Catena4611.h"

#include "Catena_Log.h"
#include "Catena_Platforms.h"
#include "Catena_Guids.h"

namespace McciCatena {

const CATENA_PLATFORM gkPlatformCatena4611 =
	{
	Guid: GUID_HW_CATENA_4611_BASE(WIRE),
	pParent: &gkPlatformCatena4611,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLuxSi1133 |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasFlash
	};

const CATENA_PLATFORM gkPlatformCatena4611_m101 =
	{
	Guid: GUID_HW_CATENA_4611_M101(WIRE),
	pParent: &gkPlatformCatena4611,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLuxSi1133 |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasFlash |
		CatenaBase::fM101
	};

const CATENA_PLATFORM gkPlatformCatena4611_m102 =
	{
	Guid: GUID_HW_CATENA_4611_M102(WIRE),
	pParent: &gkPlatformCatena4611,
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

const CATENA_PLATFORM gkPlatformCatena4611_m103 =
	{
	Guid: GUID_HW_CATENA_4611_M103(WIRE),
	pParent: &gkPlatformCatena4611,
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

const CATENA_PLATFORM gkPlatformCatena4611_m104 =
	{
	Guid: GUID_HW_CATENA_4611_M104(WIRE),
	pParent: &gkPlatformCatena4611,
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

const CATENA_PLATFORM (* const Catena4611::vPlatforms[]) =
	{
	// entry 0 is the default
	&gkPlatformCatena4611,
	&gkPlatformCatena4611_m101,
	&gkPlatformCatena4611_m102,
	&gkPlatformCatena4611_m103,
	&gkPlatformCatena4611_m104,
	};

const size_t Catena4611::nvPlatforms = sizeof(Catena4611::vPlatforms) / sizeof(Catena4611::vPlatforms[0]);

/*

Name:	Catena4611::getPlatformTable()

Function:
	Get the known platform table.

Definition:
	public: virtual
		void Catena4611::getPlatformTable(
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
Catena4611::getPlatformTable(
        const CATENA_PLATFORM * const * &result_vPlatforms,
        size_t &result_nvPlatforms
        )
	{
        result_vPlatforms = vPlatforms;
        result_nvPlatforms = nvPlatforms;
	}

} /* namespace McciCatena */

#endif // ARDUINO_ARCH_STM32
