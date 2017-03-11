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

#include "Catena_Platforms.h"
#include "Catena_Guids.h"


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
	PlatformFlags: 0,
	};

const CATENA_PLATFORM	gkPlatformM0LoRa =
	{
	Guid: GUID_HW_FEATHER_M0_LORA(NATIVE),
	pParent: &gkPlatformFeatherM0,
	PlatformFlags:	Catena4410::fHasLoRa,
	};

const CATENA_PLATFORM	gkPlatformFeatherM0Proto =
	{
	Guid: GUID_HW_FEATHER_M0_PROTO(NATIVE),
	pParent: NULL,
	PlatformFlags: 0,
	};


const CATENA_PLATFORM   gkPlatformFeatherM0ProtoWingLora =
        {
	Guid: GUID_HW_FEATHER_M0_PROTO_WINGLORA_TTNMCCI(NATIVE),
	pParent: &gkPlatformFeatherM0Proto,
	PlatformFlags:	CatenaSamd21::fHasLoRa |
                        CatenaSamd21::fHasTtnMcciLoRa,
        };

const CATENA_PLATFORM	gkPlatformM0LoRaTtnNyc =
	{
	Guid: GUID_HW_FEATHER_M0_LORA_TTNNYC(NATIVE),
	pParent: &gkPlatformM0LoRa,
	PlatformFlags:	Catena4410::fHasLoRa | 
		Catena4410::fHasTtnNycLoRa,
	};

const CATENA_PLATFORM gkPlatformCatena4410 =
	{
	Guid: GUID_HW_CATENA_4410_BASE(NATIVE),
	pParent: &gkPlatformM0LoRaTtnNyc,
	PlatformFlags:
		Catena4410::fHasLoRa |
		Catena4410::fHasTtnNycLoRa |
		Catena4410::fHasBme280 |
		Catena4410::fHasLux,
	};

const CATENA_PLATFORM gkPlatformCatena4410_gh =
	{
	Guid: GUID_HW_CATENA_4410_GH(NATIVE),
	pParent: &gkPlatformCatena4410,
	PlatformFlags:
		Catena4410::fHasLoRa |
		Catena4410::fHasTtnNycLoRa |
		Catena4410::fHasBme280 |
		Catena4410::fHasLux |
		Catena4410::fHasSoilProbe |
		Catena4410::fHasSolarPanel,
	};

const CATENA_PLATFORM gkPlatformCatena4410_mh =
	{
	Guid: GUID_HW_CATENA_4410_MH(NATIVE),
	pParent: &gkPlatformCatena4410,
	PlatformFlags:
		Catena4410::fHasLoRa |
		Catena4410::fHasTtnNycLoRa |
		Catena4410::fHasBme280 |
		Catena4410::fHasLux |
		Catena4410::fHasSoilProbe,
	};

const CATENA_PLATFORM gkPlatformCatena4410_swh =
	{
	Guid: GUID_HW_CATENA_4410_SWH(NATIVE),
	pParent: &gkPlatformCatena4410,
	PlatformFlags:
		Catena4410::fHasLoRa |
		Catena4410::fHasTtnNycLoRa |
		Catena4410::fHasBme280 |
		Catena4410::fHasLux |
		Catena4410::fHasWaterOneWire,
	};

const CATENA_PLATFORM gkPlatformCatena4410_pond =
	{
	Guid: GUID_HW_CATENA_4410_POND(NATIVE),
	pParent: &gkPlatformCatena4410,
	PlatformFlags:
		Catena4410::fHasLoRa |
		Catena4410::fHasTtnNycLoRa |
		Catena4410::fHasBme280 |
		Catena4410::fHasLux |
		Catena4410::fHasSoilProbe |
		Catena4410::fHasSolarPanel |
		Catena4410::fHasWaterOneWire,
	};

const CATENA_PLATFORM gkPlatformCatena4410_anatolian =
	{
	Guid: GUID_HW_CATENA_4410_ANATOLIAN(NATIVE),
	pParent: &gkPlatformCatena4410,
	PlatformFlags:
		Catena4410::fHasLoRa |
		Catena4410::fHasTtnNycLoRa |
		Catena4410::fHasBme280 |
		Catena4410::fHasLux |
		Catena4410::fHasSoilProbe |
		Catena4410::fHasSolarPanel |
		Catena4410::fHasWaterOneWire,
	};

const CATENA_PLATFORM gkPlatformCatena4450 =
	{
	Guid: GUID_HW_CATENA_4450_BASE(NATIVE),
	pParent: &gkPlatformM0LoRaTtnNyc,
	PlatformFlags:
		Catena4410::fHasLoRa |
		Catena4410::fHasTtnNycLoRa |
		Catena4410::fHasBme280 |
		Catena4410::fHasLuxRohm |
		Catena4410::fHasFRAM |
		Catena4410::fHasI2cMux,
	};

const CATENA_PLATFORM gkPlatformCatena4450_m101 =
	{
	Guid: GUID_HW_CATENA_4450_M101(NATIVE),
	pParent: &gkPlatformCatena4450,
	PlatformFlags:
		Catena4410::fHasLoRa |
		Catena4410::fHasTtnNycLoRa |
		Catena4410::fHasBme280 |
		Catena4410::fHasLuxRohm |
		Catena4410::fHasFRAM |
		Catena4410::fHasI2cMux |
		Catena4410::fM101
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


