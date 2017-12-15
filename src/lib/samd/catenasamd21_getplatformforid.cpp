/* GetPlatformForID.cpp	Thu Oct 27 2016 23:03:33 tmm */

/*

Module:  GetPlatformForID.cpp

Function:
	CatenaSamd21::GetPlatformForId()

Version:
	V0.3.0	Thu Oct 27 2016 23:03:33 tmm	Edit level 3

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
   0.1.0  Mon Oct 17 2016 04:16:14  tmm
	Module created.

*/

#ifdef ARDUINO_ARCH_SAMD

#include "CatenaSamd21.h"

#include "Catena_Log.h"
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

const CATENA_PLATFORM * 
CatenaSamd21::GetPlatformForID(
	const UniqueID_buffer_t *pIdBuffer,
	EUI64_buffer_t *pSysEui
	)
	{
	return this->GetPlatformForID(pIdBuffer, pSysEui, NULL);
	}

/* public virtual */
const CATENA_PLATFORM * 
CatenaSamd21::GetPlatformForID(
	const UniqueID_buffer_t *pIdBuffer,
	EUI64_buffer_t *pSysEui,
	uint32_t *pOperatingFlags
	)
	{
	const CPUID_PLATFORM_MAP *pMap;

        gLog.printf(gLog.kAlways, "CatenaSamd21::GetPlatformForID\n");

	pMap = &vCpuIdToPlatform[0];
	for (size_t i = 0; i < nvCpuIdToPlatform; ++i, ++pMap)
		{
		if (memcmp(pMap->CpuID.b, pIdBuffer->b, 
			   sizeof(pMap->CpuID.b)) == 0)
			{
			const CATENA_PLATFORM *pPlatform;

			memcpy(pSysEui->b, pMap->SysEUI.b,
				sizeof(pMap->SysEUI.b));

			pPlatform = pMap->pPlatform;
			if (pOperatingFlags)
				{
				uint32_t Flags;

				Flags = pPlatform->OperatingFlags;
				Flags &= ~pMap->OperatingFlagsClear;
				Flags |= pMap->OperatingFlagsSet;

				*pOperatingFlags = Flags;
				}

			return pPlatform;
			}
		}

	memset(pSysEui->b, 0, sizeof(pSysEui->b));
	if (pOperatingFlags)
		*pOperatingFlags = 0;
	return (CATENA_PLATFORM *) NULL;
	}

#endif // ARDUINO_ARCH_SAMD
