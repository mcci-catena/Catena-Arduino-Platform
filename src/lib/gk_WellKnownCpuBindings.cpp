/* gk_WellKnownCpuBindings.cpp	Mon Oct 31 2016 13:22:28 tmm */

/*

Module:  gk_WellKnownCpuBindings.cpp

Function:
	The table of well-known CPU-instance to platform bindings.

Version:
	V0.3.0	Mon Oct 31 2016 13:22:28 tmm	Edit level 2

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

/*
function _cpuid { 
        echo "$1" | 
        sed -e 's/^/0x/' -e 's/-/, 0x/g' | 
        nawk '{ 
                $1 = "        { CpuID:  { " $1; 
                $8 = $8 "\n\t\t   "; 
                $16 = $16 " },"; 
                print }' ; 
}
*/

const CATENA_CPUID_TO_PLATFORM CatenaSamd21::vCpuIdToPlatform[] =
    {
	{ CpuID:  { 0x08, 0x3b, 0xaf, 0x31, 0x35, 0x50, 0x4d, 0x51, 
		    0x20, 0x20, 0x20, 0x33, 0x1b, 0x1f, 0x11, 0xff },
	  pPlatform: &gkPlatformCatena4410_pond,
	  SysEUI: { 0x00, 0x02, 0xCC, 0x01, 0x00, 0x00, 0x00, 0x00 },
	  OperatingFlagsClear:  0,
	  OperatingFlagsSet:    /* CatenaSamd21::fUnattended | */ 0,
	},

	{ CpuID:  { 0xea, 0xa3, 0x7e, 0x87, 0x35, 0x50, 0x4d, 0x51, 
		    0x20, 0x20, 0x20, 0x33, 0x3d, 0x21, 0x13, 0xff },
	  pPlatform: &gkPlatformCatena4410_anatolian,
	  SysEUI: { 0x00, 0x02, 0xCC, 0x01, 0x00, 0x00, 0x00, 0x01 },
	},

	{ CpuID:  { 0xb0, 0xa9, 0x2f, 0x58, 0x35, 0x50, 0x4d, 0x51, 
		    0x20, 0x20, 0x20, 0x33, 0x41, 0x23, 0x12, 0xff },
	  pPlatform: &gkPlatformCatena4410_gh,
	  SysEUI: { 0x00, 0x02, 0xCC, 0x01, 0x00, 0x00, 0x00, 0x02 },
	},

        { CpuID:  { 0xcf, 0x1b, 0x20, 0xa5, 0x35, 0x50, 0x4d, 0x51,
		    0x20, 0x20, 0x20, 0x33, 0x2e, 0x06, 0x11, 0xff },
	  pPlatform: &gkPlatformCatena4410_anatolian,
	  SysEUI: { 0x00, 0x02, 0xCC, 0x01, 0x00, 0x00, 0x00, 0x03 },
	},


        { CpuID:  { 0xa3, 0xad, 0xd0, 0x3a, 0x35, 0x50, 0x4d, 0x51,
		    0x20, 0x20, 0x20, 0x33, 0x1e, 0x13, 0x11, 0xff },
	  pPlatform: &gkPlatformCatena4410_anatolian,
	  SysEUI: { 0x00, 0x02, 0xCC, 0x01, 0x00, 0x00, 0x00, 0x04 },
	},

        { CpuID:  { 0xda, 0x0e, 0x42, 0x5a, 0x35, 0x50, 0x4d, 0x51,
		    0x20, 0x20, 0x20, 0x33, 0x33, 0x2b, 0x12, 0xff },
	  pPlatform: &gkPlatformCatena4410_anatolian,
	  SysEUI: { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x05},
        },

        { CpuID:  { 0xc4, 0x8b, 0x81, 0xf7, 0x35, 0x50, 0x4d, 0x51,
		    0x20, 0x20, 0x20, 0x33, 0x3c, 0x28, 0x13, 0xff },
	  pPlatform: &gkPlatformCatena4410_anatolian,
	  SysEUI: { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x06},
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


