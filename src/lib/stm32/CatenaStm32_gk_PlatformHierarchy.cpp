/* gk_PlatformHierarchy.cpp	Sat Mar 11 2017 14:03:53 tmm */

/*

Module:  gk_PlatformHierarchy.cpp

Function:
	The hierarchy table.

Version:
	V0.5.0	Sat Mar 11 2017 14:03:53 tmm	Edit level 4

Copyright notice:
	This file copyright (C) 2016-2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	October 2016

Revision history:
   0.1.0  Mon Oct 17 2016 01:25:15  tmm
	Module created.

   0.3.0  Mon Oct 31 2016 13:49:37  tmm
	Add sensor for Anatolian project.

   0.4.0  Wed Mar  8 2017 18:12:57  tmm
	Add platforms for Catena 4450.

   0.5.0  Sat Mar 11 2017 14:03:53  tmm
	Rename catena_platforms.h and catena4410_guids.h.

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena_Platforms.h"
#include "Catena_Guids.h"

namespace McciCatena {


/****************************************************************************\
|
|		Manifest constants & typedefs.
|
|	This is strictly for private types and constants which will not 
|	be exported.
|
\****************************************************************************/



/****************************************************************************\
|
|	Read-only data.
|
|	If program is to be ROM-able, these must all be tagged read-only 
|	using the ROM storage class; they may be global.
|
\****************************************************************************/

#if defined(ARDUINO_MCCI_CATENA_4550) || defined(ARDUINO_CATENA_4550)
const CATENA_PLATFORM gkPlatformCatena4550 =
	{
	Guid: GUID_HW_CATENA_4550_BASE(WIRE),
	pParent: &gkPlatformCatena4550,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLuxSi1133 |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasFlash,
	};
#endif	/* CATENA_4550 */

#if defined(ARDUINO_MCCI_CATENA_4551) || defined(ARDUINO_CATENA_4551)
const CATENA_PLATFORM gkPlatformCatena4551 =
	{
	Guid: GUID_HW_CATENA_4551_BASE(WIRE),
	pParent: &gkPlatformCatena4551,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLuxSi1133 |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasFlash,
	};

const CATENA_PLATFORM gkPlatformCatena4551_m101 =
	{
	Guid: GUID_HW_CATENA_4551_M101(WIRE),
	pParent: &gkPlatformCatena4551,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLuxSi1133 |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasFlash |
		CatenaBase::fM101
	};

const CATENA_PLATFORM gkPlatformCatena4551_m102 =
	{
	Guid: GUID_HW_CATENA_4551_M102(WIRE),
	pParent: &gkPlatformCatena4551,
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

const CATENA_PLATFORM gkPlatformCatena4551_m103 =
	{
	Guid: GUID_HW_CATENA_4551_M103(WIRE),
	pParent: &gkPlatformCatena4551,
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

const CATENA_PLATFORM gkPlatformCatena4551_m104 =
	{
	Guid: GUID_HW_CATENA_4551_M104(WIRE),
	pParent: &gkPlatformCatena4551,
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
#endif	/* CATENA_4551 */

#if defined(ARDUINO_MCCI_CATENA_4610)
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
#endif	/* CATENA_4610 */

#if defined(ARDUINO_MCCI_CATENA_4611) || defined(ARDUINO_CATENA_4611)
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
#endif	/* CATENA_4611 */

#if defined(ARDUINO_MCCI_CATENA_4612) || defined(ARDUINO_CATENA_4612)
const CATENA_PLATFORM gkPlatformCatena4612 =
	{
	Guid: GUID_HW_CATENA_4612_BASE(WIRE),
	pParent: &gkPlatformCatena4612,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLuxSi1133 |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasFlash
	};

const CATENA_PLATFORM gkPlatformCatena4612_m101 =
	{
	Guid: GUID_HW_CATENA_4612_M101(WIRE),
	pParent: &gkPlatformCatena4612,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLuxSi1133 |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasFlash |
		CatenaBase::fM101
	};

const CATENA_PLATFORM gkPlatformCatena4612_m102 =
	{
	Guid: GUID_HW_CATENA_4612_M102(WIRE),
	pParent: &gkPlatformCatena4612,
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

const CATENA_PLATFORM gkPlatformCatena4612_m103 =
	{
	Guid: GUID_HW_CATENA_4612_M103(WIRE),
	pParent: &gkPlatformCatena4612,
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

const CATENA_PLATFORM gkPlatformCatena4612_m104 =
	{
	Guid: GUID_HW_CATENA_4612_M104(WIRE),
	pParent: &gkPlatformCatena4612,
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
#endif	/* CATENA_4612 */

#if defined(ARDUINO_MCCI_CATENA_4801) || defined(ARDUINO_CATENA_4801)
const CATENA_PLATFORM gkPlatformCatena4801 =
	{
	Guid: GUID_HW_CATENA_4801_BASE(WIRE),
	pParent: &gkPlatformCatena4801,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasFlash |
		CatenaBase::fHasRS485
	};
#endif	/* CATENA_4801 */


/****************************************************************************\
|
|	VARIABLES:
|
|	If program is to be ROM-able, these must be initialized
|	using the BSS keyword.  (This allows for compilers that require
|	every variable to have an initializer.)  Note that only those 
|	variables owned by this module should be declared here, using the BSS
|	keyword; this allows for linkers that dislike multiple declarations
|	of objects.
|
\****************************************************************************/


} /* namespace McciCatena */

#endif // ARDUINO_ARCH_STM32
