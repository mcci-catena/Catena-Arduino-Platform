/* gk_WellKnownCpuBindings.cpp	Sun Dec  4 2016 23:11:21 tmm */

/*

Module:  gk_WellKnownCpuBindings.cpp

Function:
	The table of well-known CPU-instance to platform bindings.

Version:
	V0.4.0	Sun Dec  4 2016 23:11:21 tmm	Edit level 3

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

   0.4.0  Sun Dec  4 2016 23:11:21  tmm
	Add new platform instance.

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
        awk '{ 
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
          OperatingFlagsClear: 0,
          OperatingFlagsSet: fUnattended,
	},

	{ CpuID:  { 0xea, 0xa3, 0x7e, 0x87, 0x35, 0x50, 0x4d, 0x51, 
		    0x20, 0x20, 0x20, 0x33, 0x3d, 0x21, 0x13, 0xff },
	  pPlatform: &gkPlatformCatena4410_anatolian,
	  SysEUI: { 0x00, 0x02, 0xCC, 0x01, 0x00, 0x00, 0x00, 0x01 },
          OperatingFlagsClear: 0,
          OperatingFlagsSet: fUnattended,
	},

	{ CpuID:  { 0xb0, 0xa9, 0x2f, 0x58, 0x35, 0x50, 0x4d, 0x51, 
		    0x20, 0x20, 0x20, 0x33, 0x41, 0x23, 0x12, 0xff },
	  pPlatform: &gkPlatformCatena4410_gh,
	  SysEUI: { 0x00, 0x02, 0xCC, 0x01, 0x00, 0x00, 0x00, 0x02 },
          OperatingFlagsClear: 0,
          OperatingFlagsSet: fUnattended,
	},

        { CpuID:  { 0xcf, 0x1b, 0x20, 0xa5, 0x35, 0x50, 0x4d, 0x51,
		    0x20, 0x20, 0x20, 0x33, 0x2e, 0x06, 0x11, 0xff },
	  pPlatform: &gkPlatformCatena4410_anatolian,
	  SysEUI: { 0x00, 0x02, 0xCC, 0x01, 0x00, 0x00, 0x00, 0x03 },
          OperatingFlagsClear: 0,
          OperatingFlagsSet: fUnattended,
	},


        { CpuID:  { 0xa3, 0xad, 0xd0, 0x3a, 0x35, 0x50, 0x4d, 0x51,
		    0x20, 0x20, 0x20, 0x33, 0x1e, 0x13, 0x11, 0xff },
	  pPlatform: &gkPlatformCatena4410_anatolian,
	  SysEUI: { 0x00, 0x02, 0xCC, 0x01, 0x00, 0x00, 0x00, 0x04 },
          OperatingFlagsClear: 0,
          OperatingFlagsSet: fUnattended,
	},

        { CpuID:  { 0xda, 0x0e, 0x42, 0x5a, 0x35, 0x50, 0x4d, 0x51,
		    0x20, 0x20, 0x20, 0x33, 0x33, 0x2b, 0x12, 0xff },
	  pPlatform: &gkPlatformCatena4410_anatolian,
	  SysEUI: { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x05},
          OperatingFlagsClear: 0,
          OperatingFlagsSet: fUnattended,
        },

        { CpuID:  { 0xc4, 0x8b, 0x81, 0xf7, 0x35, 0x50, 0x4d, 0x51,
		    0x20, 0x20, 0x20, 0x33, 0x3c, 0x28, 0x13, 0xff },
	  pPlatform: &gkPlatformCatena4410_anatolian,
	  SysEUI: { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x06},
          OperatingFlagsClear: 0,
          OperatingFlagsSet: fUnattended,
	},

        { CpuID:  { 0x66, 0x52, 0x24, 0x98, 0x35, 0x50, 0x4d, 0x51,
		    0x20, 0x20, 0x20, 0x33, 0x1b, 0x08, 0x11, 0xff },
	  pPlatform: &gkPlatformCatena4410_anatolian,
	  SysEUI: { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x07},
          OperatingFlagsClear: 0,
          OperatingFlagsSet: fUnattended,
	},

        { CpuID:  { 0x34, 0xd8, 0xa0, 0xf7, 0x35, 0x50, 0x4d, 0x51,
		    0x20, 0x20, 0x20, 0x33, 0x18, 0x2d, 0x11, 0xff },
	  pPlatform: &gkPlatformCatena4410_anatolian,
	  SysEUI: { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x08},
          OperatingFlagsClear: 0,
          OperatingFlagsSet: fUnattended,
	},

        { CpuID:  { 0x67, 0x5c, 0xc2, 0xff, 0x35, 0x50, 0x4d, 0x51,
		    0x20, 0x20, 0x20, 0x33, 0x21, 0x32, 0x13, 0xff },
	  pPlatform: &gkPlatformCatena4410_anatolian,
	  SysEUI: { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x09},
          OperatingFlagsClear: 0,
          OperatingFlagsSet: fUnattended,
	},

        { CpuID:  { 0x23, 0x47, 0x48, 0xbb, 0x35, 0x50, 0x4d, 0x51,
		    0x20, 0x20, 0x20, 0x33, 0x27, 0x1a, 0x13, 0xff },
	  pPlatform: &gkPlatformCatena4410_anatolian,
	  SysEUI: { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x0b},
          OperatingFlagsClear: 0,
          OperatingFlagsSet: fUnattended,
	},

        { CpuID:  { 0xec, 0x26, 0xe8, 0x50, 0x35, 0x50, 0x4d, 0x51,
                    0x20, 0x20, 0x20, 0x35, 0x20, 0x21, 0x10, 0xff },
	  pPlatform: &gkPlatformFeatherM0ProtoWingLora,
	  SysEUI: { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x0c},
          OperatingFlagsClear: 0,
          OperatingFlagsSet: fUnattended,
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


