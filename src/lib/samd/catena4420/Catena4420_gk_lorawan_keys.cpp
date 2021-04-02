/* Catena4420_gk_lorawan_keys.cpp	Wed Dec 06 2017 19:27:10 tmm */

/*

Module:  Catena4420_gk_lorawan_keys.cpp

Function:
	Catena4420::gk_LoRaWAN_Keys;

Version:
	V0.7.0	Wed Dec 06 2017 19:27:10 tmm	Edit level 2

Copyright notice:
	This file copyright (C) 2016-2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

Author:
	Terry Moore, MCCI Corporation	December 2016

Revision history:
   0.4.0  Sun Dec  4 2016 23:00:37  tmm
	Module created.

   0.7.0  Wed Dec 06 2017 19:27:10  tmm
        Refactored for Catena 4551 support.

*/

#ifdef ARDUINO_ARCH_SAMD

#include <Catena4420.h>
#include <Catena4420_project_config.h>

using namespace McciCatena;


/*

Use the script extra/make-lorawan-keys-entry.sh to generate
the entries in this table.

*/

static const Arduino_LoRaWAN::ProvisioningInfo
skProvisioningInfo[] =
        {
        /* to make it easier to manage the keys, we store the real keys elsewhere */
#if CATENA4420_USE_STATIC_KEYS
        #include "../../../../../catena-lorawan-provisioning/extra/gk_lorawan_keys.4420.cpp"
#endif
        };

const Arduino_LoRaWAN::ProvisioningTable Catena4420::gk_LoRaWAN_Keys =
	{
	pInfo: skProvisioningInfo,
	nInfo: MCCIADK_LENOF(skProvisioningInfo)
	};

#endif // ARDUINO_ARCH_SAMD
