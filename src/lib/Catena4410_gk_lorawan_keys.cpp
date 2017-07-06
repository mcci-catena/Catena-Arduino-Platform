/* gk_lorawan_keys.cpp	Tue Nov  1 2016 13:46:08 tmm */

/*

Module:  gk_lorawan_keys.cpp

Function:
	pointer to the real provisioning keys

Version:
	V0.3.0	Tue Nov  1 2016 13:46:08 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2016 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	November 2016

Revision history:
   0.3.0  Tue Nov  1 2016 13:46:08  tmm
	Module created.

*/

#include <Catena4410.h>
#include <Catena4410_project_config.h>

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
#if CATENA4410_USE_STATIC_KEYS
        #include "../../../catena-lorawan-provisioning/extra/gk_lorawan_keys.project.cpp"
#endif
        };

const Arduino_LoRaWAN::ProvisioningTable Catena4410::gk_LoRaWAN_Keys =
	{
	pInfo: skProvisioningInfo,
	nInfo: MCCIADK_LENOF(skProvisioningInfo)
	};
