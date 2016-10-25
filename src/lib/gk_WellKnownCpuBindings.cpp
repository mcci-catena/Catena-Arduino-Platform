/* gk_WellKnownCpuBindings.cpp	Mon Oct 17 2016 03:38:02 tmm */

/*

Module:  gk_WellKnownCpuBindings.cpp

Function:
	The table of well-known CPU-instance to platform bindings.

Version:
	V0.1.0	Mon Oct 17 2016 03:38:02 tmm	Edit level 1

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
   0.1.0  Mon Oct 17 2016 03:38:02  tmm
	Module created.

*/

#include "Catena4410.h"
#include "catena_platforms.h"

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

const CATENA_CPUID_TO_PLATFORM CatenaSamd21::vCpuIdToPlatform[] =
    {
	{ CpuID:  { 0x08, 0x3b, 0xaf, 0x31, 0x35, 0x50, 0x4d, 0x51, 
		    0x20, 0x20, 0x20, 0x33, 0x1b, 0x1f, 0x11, 0xff },
	  pPlatform: &gkPlatformCatena4410_pond,
	  SysEUI: { 0x00, 0x02, 0xCC, 0x01, 0x00, 0x00, 0x00, 0x00 },
	},

	{ CpuID:  { 0xea, 0xa3, 0x7e, 0x87, 0x35, 0x50, 0x4d, 0x51, 
		    0x20, 0x20, 0x20, 0x33, 0x3d, 0x21, 0x13, 0xff },
	  pPlatform: &gkPlatformCatena4410_gh,
	  SysEUI: { 0x00, 0x02, 0xCC, 0x01, 0x00, 0x00, 0x00, 0x01 },
	},

	{ CpuID:  { 0xb0, 0xa9, 0x2f, 0x58, 0x35, 0x50, 0x4d, 0x51, 
		    0x20, 0x20, 0x20, 0x33, 0x41, 0x23, 0x12, 0xff },
	  pPlatform: &gkPlatformCatena4410_gh,
	  SysEUI: { 0x00, 0x02, 0xCC, 0x01, 0x00, 0x00, 0x00, 0x02 },
	},
    };

const size_t CatenaSamd21::nvCpuIdToPlatform =
		MCCIADK_LENOF(CatenaSamd21::vCpuIdToPlatform);

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


