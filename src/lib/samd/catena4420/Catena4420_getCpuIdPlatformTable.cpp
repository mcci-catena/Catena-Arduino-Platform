/* Catena4420_getCpuIdPlatformTable.cpp	Wed Dec 05 2018 10:41:47 chwon */

/*

Module:  Catena4420_getCpuIdPlatformTable.cpp

Function:
	Catena4420::getCpuIdPlatformTable()

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

#include "Catena4420.h"

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

const CatenaBase::CPUID_PLATFORM_MAP Catena4420::vCpuIdToPlatform[] =
    {
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

const size_t Catena4420::nvCpuIdToPlatform =
		MCCIADK_LENOF(Catena4420::vCpuIdToPlatform);


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

Name:	Catena4420::getCpuIdPlatformTable()

Function:
	Get the known platform table.

Definition:
	public: virtual
		void Catena4420::getCpuIdPlatformTable(
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
Catena4420::getCpuIdPlatformTable(
        const CPUID_PLATFORM_MAP * &result_vCpuIdToPlatform,
        size_t &result_nvCpuIdToPlatform
        )
	{
        result_vCpuIdToPlatform = vCpuIdToPlatform;
        result_nvCpuIdToPlatform = nvCpuIdToPlatform;
	}

#endif // ARDUINO_ARCH_SAMD
