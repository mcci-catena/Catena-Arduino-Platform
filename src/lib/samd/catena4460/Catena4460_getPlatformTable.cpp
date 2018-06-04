/* Catena4460_getPlatformTable.cpp	Wed Apr 12 2017 13:26:14 tmm */

/*

Module:  Catena4460_getPlatformTable.cpp

Function:
	Catena4460::getPlatformTable()

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

#include "Catena4460.h"

#include "Catena_Log.h"
#include "Catena_Platforms.h"

using namespace McciCatena;

const CATENA_PLATFORM (* const Catena4460::vPlatforms[]) =
	{
	// entry 0 is the default
	&gkPlatformCatena4460,
	&gkPlatformCatena4460_m101,
	&gkPlatformCatena4460_m102,
	&gkPlatformCatena4460_m103,
	&gkPlatformCatena4460_m104,
	};

const size_t Catena4460::nvPlatforms = sizeof(Catena4460::vPlatforms) / sizeof(Catena4460::vPlatforms[0]);

/*

Name:	Catena4460::getPlatformTable()

Function:
	Get the known platform table.

Definition:
	public: virtual 
		void Catena4460::getPlatformTable(
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
Catena4460::getPlatformTable(
        const CATENA_PLATFORM * const * &result_vPlatforms,
        size_t &result_nvPlatforms
        )
	{
        result_vPlatforms = vPlatforms;
        result_nvPlatforms = nvPlatforms;
	}

#endif // ARDUINO_ARCH_SAMD
