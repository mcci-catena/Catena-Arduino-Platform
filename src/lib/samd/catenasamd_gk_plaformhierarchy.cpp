/* gk_PlatformHierarchy.cpp	Sat Mar 31 2018 19:28:30 tmm */

/*

Module:  gk_PlatformHierarchy.cpp

Function:
	The hierarchy table.

Version:
	V0.9.0	Sat Mar 31 2018 19:28:30 tmm	Edit level 5

Copyright notice:
	This file copyright (C) 2016-2018 by

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

   0.9.0  Sat Mar 31 2018 19:28:30  tmm
	Add platforms for Catena 4460.

*/

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

const CATENA_PLATFORM	gkPlatformFeatherM0 =
	{
	Guid: GUID_HW_FEATHER_M0(WIRE),
	pParent: NULL,
	PlatformFlags: 0,
	};

const CATENA_PLATFORM	gkPlatformM0LoRa =
	{
	Guid: GUID_HW_FEATHER_M0_LORA(WIRE),
	pParent: &gkPlatformFeatherM0,
	PlatformFlags: CatenaBase::fHasLoRa,
	};

const CATENA_PLATFORM	gkPlatformFeatherM0Proto =
	{
	Guid: GUID_HW_FEATHER_M0_PROTO(WIRE),
	pParent: NULL,
	PlatformFlags: 0,
	};


const CATENA_PLATFORM   gkPlatformFeatherM0ProtoWingLora =
        {
	Guid: GUID_HW_FEATHER_M0_PROTO_WINGLORA_TTNMCCI(WIRE),
	pParent: &gkPlatformFeatherM0Proto,
	PlatformFlags:	CatenaBase::fHasLoRa |
                        CatenaBase::fHasTtnMcciLoRa,
        };

const CATENA_PLATFORM	gkPlatformM0LoRaTtnNyc =
	{
	Guid: GUID_HW_FEATHER_M0_LORA_TTNNYC(WIRE),
	pParent: &gkPlatformM0LoRa,
	PlatformFlags: CatenaBase::fHasLoRa |
                       CatenaBase::fHasTtnNycLoRa,
	};

const CATENA_PLATFORM gkPlatformCatena4410 =
	{
	Guid: GUID_HW_CATENA_4410_BASE(WIRE),
	pParent: &gkPlatformM0LoRaTtnNyc,
	PlatformFlags:
                CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLux,
	};

const CATENA_PLATFORM gkPlatformCatena4410_gh =
	{
	Guid: GUID_HW_CATENA_4410_GH(WIRE),
	pParent: &gkPlatformCatena4410,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLux |
		CatenaBase::fHasSoilProbe |
		CatenaBase::fHasSolarPanel,
	};

const CATENA_PLATFORM gkPlatformCatena4410_mh =
	{
	Guid: GUID_HW_CATENA_4410_MH(WIRE),
	pParent: &gkPlatformCatena4410,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLux |
		CatenaBase::fHasSoilProbe,
	};

const CATENA_PLATFORM gkPlatformCatena4410_swh =
	{
	Guid: GUID_HW_CATENA_4410_SWH(WIRE),
	pParent: &gkPlatformCatena4410,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLux |
		CatenaBase::fHasWaterOneWire,
	};

const CATENA_PLATFORM gkPlatformCatena4410_pond =
	{
	Guid: GUID_HW_CATENA_4410_POND(WIRE),
	pParent: &gkPlatformCatena4410,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLux |
		CatenaBase::fHasSoilProbe |
		CatenaBase::fHasSolarPanel |
		CatenaBase::fHasWaterOneWire,
	};

const CATENA_PLATFORM gkPlatformCatena4410_anatolian =
	{
	Guid: GUID_HW_CATENA_4410_ANATOLIAN(WIRE),
	pParent: &gkPlatformCatena4410,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLux |
		CatenaBase::fHasSoilProbe |
		CatenaBase::fHasSolarPanel |
		CatenaBase::fHasWaterOneWire,
	};

const CATENA_PLATFORM gkPlatformCatena4450 =
	{
	Guid: GUID_HW_CATENA_4450_BASE(WIRE),
	pParent: &gkPlatformM0LoRaTtnNyc,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLuxRohm |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasI2cMux,
	};

const CATENA_PLATFORM gkPlatformCatena4450_m101 =
	{
	Guid: GUID_HW_CATENA_4450_M101(WIRE),
	pParent: &gkPlatformCatena4450,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLuxRohm |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasI2cMux |
		CatenaBase::fM101
	};

const CATENA_PLATFORM gkPlatformCatena4450_m102 =
	{
	Guid: GUID_HW_CATENA_4450_M102(WIRE),
	pParent: &gkPlatformCatena4450,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLuxRohm |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasI2cMux |
		CatenaBase::fHasSoilProbe |
		CatenaBase::fHasSolarPanel |
		CatenaBase::fHasWaterOneWire |
		CatenaBase::fM102
	};

const CATENA_PLATFORM gkPlatformCatena4450_m103 =
	{
	Guid: GUID_HW_CATENA_4450_M103(WIRE),
	pParent: &gkPlatformCatena4450,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLuxRohm |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasI2cMux |
		CatenaBase::fHasSoilProbe |
		CatenaBase::fHasSolarPanel |
		CatenaBase::fHasWaterOneWire |
		CatenaBase::fM103
	};

const CATENA_PLATFORM gkPlatformCatena4450_m104 =
	{
	Guid: GUID_HW_CATENA_4450_M104(WIRE),
	pParent: &gkPlatformCatena4450,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLuxRohm |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasI2cMux |
		CatenaBase::fHasSoilProbe |
		CatenaBase::fHasWaterOneWire |
		CatenaBase::fM104
	};

const CATENA_PLATFORM gkPlatformCatena4460 =
	{
	Guid: GUID_HW_CATENA_4460_BASE(WIRE),
	pParent: &gkPlatformM0LoRaTtnNyc,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme680 |
		CatenaBase::fHasLuxRohm |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasI2cMux,
	};

const CATENA_PLATFORM gkPlatformCatena4460_m101 =
	{
	Guid: GUID_HW_CATENA_4460_M101(WIRE),
	pParent: &gkPlatformCatena4460,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme680 |
		CatenaBase::fHasLuxRohm |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasI2cMux |
		CatenaBase::fM101
	};

const CATENA_PLATFORM gkPlatformCatena4460_m102 =
	{
	Guid: GUID_HW_CATENA_4460_M102(WIRE),
	pParent: &gkPlatformCatena4460,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme680 |
		CatenaBase::fHasLuxRohm |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasI2cMux |
		CatenaBase::fHasSoilProbe |
		CatenaBase::fHasSolarPanel |
		CatenaBase::fHasWaterOneWire |
		CatenaBase::fM102
	};

const CATENA_PLATFORM gkPlatformCatena4460_m103 =
	{
	Guid: GUID_HW_CATENA_4460_M103(WIRE),
	pParent: &gkPlatformCatena4460,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme680 |
		CatenaBase::fHasLuxRohm |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasI2cMux |
		CatenaBase::fHasSoilProbe |
		CatenaBase::fHasSolarPanel |
		CatenaBase::fHasWaterOneWire |
		CatenaBase::fM103
	};

const CATENA_PLATFORM gkPlatformCatena4460_m104 =
	{
	Guid: GUID_HW_CATENA_4460_M104(WIRE),
	pParent: &gkPlatformCatena4460,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLuxRohm |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasI2cMux |
		CatenaBase::fHasSoilProbe |
		CatenaBase::fHasWaterOneWire |
		CatenaBase::fM104
	};

const CATENA_PLATFORM gkPlatformCatena4470 =
	{
	Guid: GUID_HW_CATENA_4470_BASE(WIRE),
	pParent: &gkPlatformM0LoRaTtnNyc,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLuxRohm |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasRS485 |
		CatenaBase::fHasVout1,
	};

const CATENA_PLATFORM gkPlatformCatena4470_m101 =
	{
	Guid: GUID_HW_CATENA_4470_M101(WIRE),
	pParent: &gkPlatformCatena4470,
	PlatformFlags:
		CatenaBase::fHasLoRa |
		CatenaBase::fHasTtnNycLoRa |
		CatenaBase::fHasBme280 |
		CatenaBase::fHasLuxRohm |
		CatenaBase::fHasFRAM |
		CatenaBase::fHasRS485 |
		CatenaBase::fHasVout1,
		CatenaBase::fM101
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


} /* namespace McciCatena */
