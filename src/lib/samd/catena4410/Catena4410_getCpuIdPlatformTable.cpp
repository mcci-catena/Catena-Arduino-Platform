/* Catena4410_getCpuIdPlatformTable.cpp	Wed Dec 05 2018 10:41:47 chwon */

/*

Module:  Catena4410_getCpuIdPlatformTable.cpp

Function:
	Catena4410::getCpuIdPlatformTable()

Version:
	V0.12.0	Wed Dec 05 2018 10:41:47 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	December 2018

Revision history:
   0.12.0  Wed Dec 05 2018 10:41:47  chwon
	Module created.

*/

#ifdef ARDUINO_ARCH_SAMD

#include "Catena4410.h"

#include "Catena_Log.h"
#include "Catena_Platforms.h"

using namespace McciCatena;

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

const CatenaBase::CPUID_PLATFORM_MAP Catena4410::vCpuIdToPlatform[] =
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

        { CpuID:  { 0x58, 0x35, 0xf7, 0xfd, 0x35, 0x50, 0x4d, 0x51,
		    0x20, 0x20, 0x20, 0x35, 0x1b, 0x1f, 0x14, 0xff },
	  pPlatform: &gkPlatformFeatherM0ProtoWingLora,
	  SysEUI: { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x0D},
          OperatingFlagsClear: 0,
          OperatingFlagsSet: fUnattended,
	},

        { CpuID:  { 0x2e, 0x59, 0x9c, 0xa1, 0x35, 0x50, 0x4d, 0x51,
                  0x20, 0x20, 0x20, 0x35, 0x1c, 0x0d, 0x14, 0xff },
           pPlatform: &gkPlatformFeatherM0ProtoWingLora,
           SysEUI: { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x0E},
           OperatingFlagsClear: 0,
           OperatingFlagsSet: fUnattended,
        },

        { CpuID:  { 0xe9, 0xad, 0xa4, 0x5e, 0x35, 0x50, 0x4d, 0x51,
                  0x20, 0x20, 0x20, 0x35, 0x1d, 0x04, 0x14, 0xff },
           pPlatform: &gkPlatformFeatherM0ProtoWingLora,
           SysEUI: { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x0F},
           OperatingFlagsClear: 0,
           OperatingFlagsSet: fUnattended,
        },

        { CpuID:  { 0x5e, 0x10, 0x31, 0xd2, 0x35, 0x50, 0x4d, 0x51,
                  0x20, 0x20, 0x20, 0x35, 0x26, 0x0a, 0x14, 0xff },
           pPlatform: &gkPlatformFeatherM0ProtoWingLora,
           SysEUI: { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x10},
           OperatingFlagsClear: 0,
           OperatingFlagsSet: fUnattended,
        },

        { CpuID:  { 0x1f, 0x84, 0x89, 0x75, 0x35, 0x50, 0x4d, 0x51,
                  0x20, 0x20, 0x20, 0x35, 0x43, 0x1c, 0x13, 0xff },
           pPlatform: &gkPlatformFeatherM0ProtoWingLora,
           SysEUI: { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x11},
           OperatingFlagsClear: 0,
           OperatingFlagsSet: fUnattended,
        },

    };

const size_t Catena4410::nvCpuIdToPlatform =
		MCCIADK_LENOF(Catena4410::vCpuIdToPlatform);


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


/*

Name:	Catena4410::getCpuIdPlatformTable()

Function:
	Get the known platform table.

Definition:
	public: virtual
		void Catena4410::getCpuIdPlatformTable(
                        const CPUID_PLATFORM_MAP * &vCpuIdToPlatform,
                        size_t &nvCpuIdToPlatform
                        ) override;

Description:
	This override for getCpuIdPlatformTable() returns the vector of platform
        GUIDs for this Catena.

Returns:
	vCpuIdToPlatform is set to the base of the array of pointers to platform
        stuctures; and nvCpuIdToPlatform is set to the number of entries in
        the table.

*/

/* public virtual override */
void
Catena4410::getCpuIdPlatformTable(
        const CPUID_PLATFORM_MAP * &result_vCpuIdToPlatform,
        size_t &result_nvCpuIdToPlatform
        )
	{
        result_vCpuIdToPlatform = vCpuIdToPlatform;
        result_nvCpuIdToPlatform = nvCpuIdToPlatform;
	}

#endif // ARDUINO_ARCH_SAMD
