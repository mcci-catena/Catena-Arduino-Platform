/* gk_lorawan_keys.cpp	Wed Dec 06 2017 19:27:10 tmm */

/*

Module:  gk_lorawan_keys.cpp

Function:
	pointer to the real provisioning keys

Version:
	V0.7.0	Wed Dec 06 2017 19:27:10	Edit level 12

Copyright notice:
	This file copyright (C) 2016 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

Author:
	Terry Moore, MCCI Corporation	November 2016

Revision history:
   0.3.0  Tue Nov  1 2016 13:46:08  tmm
	Module created.

   0.7.0  Wed Dec 06 2017 19:27:10  tmm
        Refactored for Catena 4551 support.

*/

#ifdef ARDUINO_ARCH_SAMD

#include <Catena4410.h>
#include <Catena4410_project_config.h>

using namespace McciCatena;

/*

Use the script extra/make-lorawan-keys-entry.sh to generate
the entries in this table.

*/

static const Arduino_LoRaWAN::ProvisioningInfo
skProvisioningInfo[] =
        {
        /* to make it easier to manage the keys, we store the real keys elsewhere */
#if CATENA4410_USE_STATIC_KEYS
        #include "../../../../../catena-lorawan-provisioning/extra/gk_lorawan_keys.project.cpp"
#endif
        };

const Arduino_LoRaWAN::ProvisioningTable Catena4410::gk_LoRaWAN_Keys =
	{
	pInfo: skProvisioningInfo,
	nInfo: MCCIADK_LENOF(skProvisioningInfo)
	};

#endif // ARDUINO_ARCH_SAMD
