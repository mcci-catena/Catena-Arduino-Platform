/* gk_WellKnownCpuBindings.cpp	Sat Mar 11 2017 14:06:00 tmm */

/*

Module:  gk_WellKnownCpuBindings.cpp

Function:
	The table of well-known CPU-instance to platform bindings.

Version:
	V0.5.0	Sat Mar 11 2017 14:06:00 tmm	Edit level 5

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
   0.1.0  Mon Oct 17 2016 03:38:02  tmm
	Module created.

   0.4.0  Sun Dec  4 2016 23:11:21  tmm
	Add new platform instances.

   0.4.0  Wed Mar  8 2017 18:25:47  tmm
	Add Catena 4450 instances.

   0.5.0  Sat Mar 11 2017 14:06:00  tmm
	Rename catena_platforms.h to follow naming spec.

*/

#ifdef ARDUINO_ARCH_STM32

#include "CatenaStm32.h"
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

const CatenaStm32::CPUID_PLATFORM_MAP CatenaStm32::vCpuIdToPlatform[] =
    {
        { CpuID: { 0xf6, 0x74, 0x4f, 0xbe, 0x35, 0x50, 0x4d, 0x51,
                   0x20, 0x20, 0x20, 0x35, 0x2d, 0x2e, 0x11, 0xff },
                pPlatform : &gkPlatformCatena4550,
                SysEUI : { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x4D},
                OperatingFlagsClear : 0,
                OperatingFlagsSet : 0,
        },

        { CpuID: { 0x95, 0x31, 0xbd, 0x24, 0x54, 0x53, 0x4b, 0x50,
                   0x4a, 0x31, 0x2e, 0x39, 0x3f, 0x16, 0x07, 0xff },
                pPlatform : &gkPlatformCatena4551,
                SysEUI : { 0x00, 0x02, 0xcc, 0x01, 0x00, 0x00, 0x00, 0x4E},
                OperatingFlagsClear : 0,
                OperatingFlagsSet : 0,
        },

    };

const size_t CatenaStm32::nvCpuIdToPlatform =
		MCCIADK_LENOF(CatenaStm32::vCpuIdToPlatform);

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


#endif // ARDUINO_ARCH_STM32
