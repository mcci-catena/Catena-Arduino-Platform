/* GetPlatformForID.cpp	Mon Oct 17 2016 04:16:14 tmm */

/*

Module:  GetPlatformForID.cpp

Function:
	Catena4410::GetPlatformForId()

Version:
	V0.1.0	Mon Oct 17 2016 04:16:14 tmm	Edit level 1

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

#include "Catena4410.h"

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
Catena4410::GetPlatformForID(
	const UniqueID_buffer_t pIdBuffer,
	EUI64_buffer_t pSysEui
	)
	{
	
	for (size_t i = 0; i < nvCpuIdToPlatform; ++i)
		{
		if (memcmp(vCpuIdToPlatform[i].CpuID, pIdBuffer, 
			   sizeof(vCpuIdToPlatform[i].CpuID)) == 0)
			{
			memcpy(pSysEui, vCpuIdToPlatform[i].SysEUI,
				sizeof(vCpuIdToPlatform[i].SysEUI));
			return vCpuIdToPlatform[i].pPlatform;
			}
		}

	memset(pSysEui, sizeof(EUI64_buffer_t), 0);
	return (CATENA_PLATFORM *) NULL;
	}
