/* Catena4450_getPlatformTable.cpp	Wed Apr 12 2017 13:26:14 tmm */

/*

Module:  Catena4450_getPlatformTable.cpp

Function:
	Catena4450::getPlatformTable()

Version:
	V0.5.0	Wed Apr 12 2017 13:26:14 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	April 2017

Revision history:
   0.5.0  Wed Apr 12 2017 13:26:14  tmm
	Module created.

*/

#ifdef ARDUINO_ARCH_SAMD

#include "Catena4450.h"

#include "Catena_Log.h"
#include "Catena_Platforms.h"

using namespace McciCatena;

const CATENA_PLATFORM (* const Catena4450::vPlatforms[]) =
	{
	// entry 0 is the default
	&gkPlatformCatena4450,
	&gkPlatformCatena4450_m101,
	&gkPlatformCatena4450_m102,
	&gkPlatformCatena4450_m103,
	&gkPlatformCatena4450_m104,
	};

const size_t Catena4450::nvPlatforms = sizeof(Catena4450::vPlatforms) / sizeof(Catena4450::vPlatforms[0]);

/*

Name:	Catena4450::getPlatformTable()

Function:
	Get the known platform table.

Definition:
	public: virtual 
		void Catena4450::getPlatformTable(
                        const CATENA_PLATFORM * const * &vPlatforms,
                        size_t &nvPlatforms
                        ) override;

Description:
	This override for getPlatformTable() returns the vector of platform
        GUIDs for this Catena.

Returns:
	vPlatforms is set to the base of the array of pointers to platform
        stuctures; and nvPlatforms is set to the number of entries in
        the table.

*/

/* public virtual override */
void
Catena4450::getPlatformTable(
        const CATENA_PLATFORM * const * &result_vPlatforms,
        size_t &result_nvPlatforms
        )
	{
        result_vPlatforms = vPlatforms;
        result_nvPlatforms = nvPlatforms;
	}

#endif // ARDUINO_ARCH_SAMD
