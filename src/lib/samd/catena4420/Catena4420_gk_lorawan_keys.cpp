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
function _makeentry {
./ttnctl devices info $1 | awk '
function revhex(s,     r, i) 
   { # r, i are local variables
   r = ""; 
   for (i = 1; i < length(s); i += 2) 
      { 
      r = "0x" substr(s, i, 2) ", " r; 
      } 
   # strip off the trailing ", ", and then convert to the 
   # declaration needed by arduino-lmic sketches.
   return "{ " substr(r, 1, length(r)-2) " }"; 
   } 
function friendlyHex(s,  r, i)
   { # r, i are local variables
   r = ""; 
   for (i = 1; i < length(s); i += 2) 
      {
      if (r != "")
         r = r "-" substr(s, i, 2);
      else
         r = substr(s, i, 2);
      }
  return r;
  }
 /AppEUI/ { AppEUI = revhex($2) } 
 /DevEUI/ { DevEUI = revhex($2); DevEUIfriendly = $2; } 
 /AppKey/ { getline; AppKey = substr($0, 11) }
 END {
     printf("%8s{  // %s\n", "", friendlyHex(DevEUIfriendly));
     printf("%8sStyle:  Arduino_LoRaWAN::ProvisioningStyle::kOTAA,\n", "");
     printf("%8sAbpInfo: {},\n", "");
     printf("%8sOtaaInfo:\n", "");
     printf("%16s{\n", "");
     printf("%16sAppKey: %s,\n", "", AppKey);
     printf("%16sDevEUI: %s,\n", "", DevEUI);
     printf("%16sAppEUI: %s,\n", "", AppEUI);
     printf("%16s},\n", "");
     printf("%8s},\n", "");
     }
 '
}
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
