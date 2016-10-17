/* gk_PlatformHierarchy.cpp	Mon Oct 17 2016 01:25:15 tmm */

/*

Module:  gk_PlatformHierarchy.cpp

Function:
	The hierarchy table.

Version:
	V0.1.0	Mon Oct 17 2016 01:25:15 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2016 by

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

*/

#include "Catena4410.h"

#include "catena4410_guids.h"

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

const CATENA_PLATFORM	gkPlatformFeatherM0 =
	{
	Guid: GUID_HW_FEATHER_M0(NATIVE),
	pParent: NULL,
	Flags: 0,
	};

const CATENA_PLATFORM	gkPlatformM0LoRa =
	{
	Guid: GUID_HW_FEATHER_M0_LORA(NATIVE),
	pParent: &gkPlatformFeatherM0,
	Flags:	Catena4410::fHasLoRa,
	};

const CATENA_PLATFORM	gkPlatformM0LoRaTtnNyc =
	{
	Guid: GUID_HW_FEATHER_M0_LORA_TTNNYC(NATIVE),
	pParent: &gkPlatformM0LoRa,
	Flags:	Catena4410::fHasLoRa | 
		Catena4410::fHasTtnNycLoRa,
	};

const CATENA_PLATFORM gkPlatformCatena4410 =
	{
	Guid: GUID_HW_CATENA_4410_BASE(NATIVE),
	pParent: &gkPlatformM0LoRaTtnNyc,
	Flags:	Catena4410::fHasLoRa |
		Catena4410::fHasTtnNycLoRa |
		Catena4410::fHasBme280 |
		Catena4410::fHasLux,
	};

const CATENA_PLATFORM gkPlatformCatena4410_gh =
	{
	Guid: GUID_HW_CATENA_4410_BASE(NATIVE),
	pParent: &gkPlatformCatena4410,
	Flags:	Catena4410::fHasLoRa |
		Catena4410::fHasTtnNycLoRa |
		Catena4410::fHasBme280 |
		Catena4410::fHasLux |
		Catena4410::fHasSoilProbe |
		Catena4410::fHasSolarPanel,
	};

const CATENA_PLATFORM gkPlatformCatena4410_mh =
	{
	Guid: GUID_HW_CATENA_4410_BASE(NATIVE),
	pParent: &gkPlatformCatena4410,
	Flags:	Catena4410::fHasLoRa |
		Catena4410::fHasTtnNycLoRa |
		Catena4410::fHasBme280 |
		Catena4410::fHasLux |
		Catena4410::fHasSoilProbe,
	};

const CATENA_PLATFORM gkPlatformCatena4410_swh =
	{
	Guid: GUID_HW_CATENA_4410_BASE(NATIVE),
	pParent: &gkPlatformCatena4410,
	Flags:	Catena4410::fHasLoRa |
		Catena4410::fHasTtnNycLoRa |
		Catena4410::fHasBme280 |
		Catena4410::fHasLux |
		Catena4410::fHasWaterOneWire,
	};

const CATENA_PLATFORM gkPlatformCatena4410_pond =
	{
	Guid: GUID_HW_CATENA_4410_BASE(NATIVE),
	pParent: &gkPlatformCatena4410,
	Flags:	Catena4410::fHasLoRa |
		Catena4410::fHasTtnNycLoRa |
		Catena4410::fHasBme280 |
		Catena4410::fHasLux |
		Catena4410::fHasSoilProbe |
		Catena4410::fHasWaterOneWire,
	};

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


