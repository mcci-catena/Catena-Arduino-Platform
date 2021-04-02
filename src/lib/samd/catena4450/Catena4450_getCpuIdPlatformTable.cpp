/* Catena4450_getCpuIdPlatformTable.cpp	Wed Dec 05 2018 10:41:47 chwon */

/*

Module:  Catena4450_getCpuIdPlatformTable.cpp

Function:
	Catena4450::getCpuIdPlatformTable()

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

#include "Catena4450.h"

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

const CatenaBase::CPUID_PLATFORM_MAP Catena4450::vCpuIdToPlatform[] =
    {
        { CpuID:  { 0x3e, 0x4f, 0x6c, 0x2e, 0x54, 0x53, 0x4b, 0x50,
                    0x4a, 0x31, 0x2e, 0x39, 0x2b, 0x27, 0x07, 0xff },
           pPlatform: &gkPlatformCatena4450_m102,
           SysEUI: { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x41},
           OperatingFlagsClear: 0,
           OperatingFlagsSet: fUnattended,
        },

        { CpuID:  { 0xfd, 0xf5, 0x07, 0x68, 0x54, 0x53, 0x4b, 0x50,
                    0x4a, 0x31, 0x2e, 0x39, 0x39, 0x23, 0x07, 0xff },
           pPlatform: &gkPlatformCatena4450_m101,
           SysEUI: { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x42},
           OperatingFlagsClear: 0,
           OperatingFlagsSet: fUnattended,
        },

        { CpuID:  { 0x5b, 0x6d, 0x71, 0x40, 0x54, 0x53, 0x4b, 0x50,
                    0x4a, 0x31, 0x2e, 0x39, 0x3a, 0x15, 0x07, 0xff },
           pPlatform: &gkPlatformCatena4450_m101,
           SysEUI: { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x43},
           OperatingFlagsClear: 0,
           OperatingFlagsSet: fUnattended,
        },

        { CpuID:  { 0x9f, 0xad, 0xbf, 0x02, 0x54, 0x53, 0x4b, 0x50,
                    0x4a, 0x31, 0x2e, 0x39, 0x37, 0x0e, 0x07, 0xff },
           pPlatform: &gkPlatformCatena4450_m101,
           SysEUI: { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x44},
           OperatingFlagsClear: 0,
           OperatingFlagsSet: fUnattended,
        },

        { CpuID:  { 0x5f, 0x6f, 0x4b, 0xa6, 0x54, 0x53, 0x4b, 0x50,
                    0x4a, 0x31, 0x2e, 0x39, 0x3c, 0x2c, 0x07, 0xff },
           pPlatform: &gkPlatformCatena4450_m101,
           SysEUI: { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x45},
           OperatingFlagsClear: 0,
           OperatingFlagsSet: fUnattended,
        },

        { CpuID:  { 0x05, 0x25, 0xdc, 0xef, 0x54, 0x53, 0x4b, 0x50,
                    0x4a, 0x31, 0x2e, 0x39, 0x36, 0x1a, 0x07, 0xff },
           pPlatform: &gkPlatformCatena4450_m101,
           SysEUI: { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x46},
           OperatingFlagsClear: 0,
           OperatingFlagsSet: fUnattended,
        },


        { CpuID:  { 0xf4, 0xe8, 0xc5, 0x24, 0x54, 0x53, 0x4b, 0x50,
		    0x4a, 0x31, 0x2e, 0x39, 0x37, 0x13, 0x07, 0xff },
           pPlatform: &gkPlatformCatena4450_m101,
           SysEUI: { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x47},
           OperatingFlagsClear: 0,
           OperatingFlagsSet: fUnattended,
        },

        { CpuID: { 0x8e, 0x3f, 0x7c, 0x7b, 0x54, 0x53, 0x4b, 0x50,
                  0x4a, 0x31, 0x2e, 0x39, 0x38, 0x13, 0x07, 0xff },
           pPlatform: &gkPlatformCatena4450_m102,
           SysEUI: { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x48},
           OperatingFlagsClear: 0,
           OperatingFlagsSet: 0,
        },

        { CpuID: { 0x82, 0xd1, 0xc4, 0x38, 0x54, 0x53, 0x4b, 0x50,
                  0x4a, 0x31, 0x2e, 0x39, 0x3b, 0x13, 0x07, 0xff },
                  pPlatform : &gkPlatformCatena4450_m102,
                  SysEUI : { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x49},
                  OperatingFlagsClear : 0,
                  OperatingFlagsSet : 0,
        },

        { CpuID: { 0xbf, 0x3d, 0x75, 0xd3, 0x54, 0x53, 0x4b, 0x50,
                  0x4a, 0x31, 0x2e, 0x39, 0x37, 0x2a, 0x07, 0xff },
           pPlatform: &gkPlatformCatena4450_m102,
           SysEUI: { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x4B},
           OperatingFlagsClear: 0,
           OperatingFlagsSet: 0,
        },

        { CpuID: { 0xf6, 0x74, 0x4f, 0xbe, 0x35, 0x50, 0x4d, 0x51,
                   0x20, 0x20, 0x20, 0x35, 0x2d, 0x2e, 0x11, 0xff },
                pPlatform : &gkPlatformCatena4450,
                SysEUI : { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x4D},
                OperatingFlagsClear : 0,
                OperatingFlagsSet : 0,
        },

        { CpuID: { 0x95, 0x31, 0xbd, 0x24, 0x54, 0x53, 0x4b, 0x50,
                   0x4a, 0x31, 0x2e, 0x39, 0x3f, 0x16, 0x07, 0xff },
                pPlatform : &gkPlatformCatena4450,
                SysEUI : { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x4E},
                OperatingFlagsClear : 0,
                OperatingFlagsSet : 0,
        },

    };

const size_t Catena4450::nvCpuIdToPlatform =
		MCCIADK_LENOF(Catena4450::vCpuIdToPlatform);


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

Name:	Catena4450::getCpuIdPlatformTable()

Function:
	Get the known platform table.

Definition:
	public: virtual
		void Catena4450::getCpuIdPlatformTable(
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
Catena4450::getCpuIdPlatformTable(
        const CPUID_PLATFORM_MAP * &result_vCpuIdToPlatform,
        size_t &result_nvCpuIdToPlatform
        )
	{
        result_vCpuIdToPlatform = vCpuIdToPlatform;
        result_nvCpuIdToPlatform = nvCpuIdToPlatform;
	}

#endif // ARDUINO_ARCH_SAMD
