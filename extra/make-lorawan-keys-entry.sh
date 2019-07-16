#!/bin/bash

##############################################################################
#
# Module:  make-lorawan-keys-entry.sh
#
# Function:
# 	Generate lorawan key entries from ttnctl info
#
# Copyright notice and License:
# 	See accompanying LICENSE file at 
#	https://github.com/mcci-catena/Catena-Arduino-Platform
#
# Author:
#	Terry Moore, MCCI Corporation	July 2019
#
##############################################################################

readonly PNAME="$(basename "$0")"

if [[ "$1" = "--help" ]] || [[ "$1" = "-h" ]]; then
	less <<.
Name:	$PNAME

Function:
	Generate key table entries for Catena4410_gk_lorawan_keys.cpp or
	Catena4420_gk_lorawan_keys.cpp by invoking The Things Network API.

Synopsis:
	ttnctl application select {appID}
	$PNAME [-h | --help] {device-ID} [ttnctl args...]

Environment variables:
	TTNCTL	if set, gives the full pathname of the 'ttnctl' executable
		image. If not set, ttnctl must be found in the current
		PATH.

	TTNCTL_HANDLER_ID may be set to the handler for the currently-
		selected application.

Exit status:
	The exit status will only be zero if both ttnctl and post-processing
	succeed.
.
	exit 0
fi

if [ X"$TTNCTL" = X ]; then
	if command -v ttnctl > /dev/null ; then 
		TTNCTL=ttnctl
	else
		echo "ttnctl not found; add to path, or set TTNCTL to full name of executable"
		exit 1
	fi
fi

# read output into a variable; we'll echo it if ttnctl succeeds
# and consume it otherwise.
if OUTPUT="$("$TTNCTL" devices info "$@")" ; then
	echo "$OUTPUT" | awk '
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
function fwdhex(s,     r, i)
   { # r, i are local variables
   r = "";
   for (i = 1; i < length(s); i += 2)
      {
      r = r ", " "0x" substr(s, i, 2);
      }
   # strip off the leading ", ", and then convert to the
   # declaration needed by arduino-lmic sketches.
   return "{ " substr(r, 3, length(r)-2) " }";
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
 $1 == "AppEUI:" { AppEUI = $2; }
 $1 == "DevEUI:" { DevEUI = $2; DevEUIfriendly = $2; }
 $1 == "AppKey:" { AppKey = $2; }
 END {
     printf("%8s{  // %s\n", "", friendlyHex(DevEUIfriendly));
     printf("%8sStyle:  Arduino_LoRaWAN::ProvisioningStyle::kOTAA,\n", "");
     printf("%8sAbpInfo: {},\n", "");
     printf("%8sOtaaInfo:\n", "");
     printf("%16s{\n", "");
     printf("%16sAppKey: %s,\n", "", fwdhex(AppKey));
     printf("%16sDevEUI: %s,\n", "", revhex(DevEUI));
     printf("%16sAppEUI: %s,\n", "", revhex(AppEUI));
     printf("%16s},\n", "");
     printf("%8s},\n", "");
     }
 '
else
	echo "ttnctl failed"
	echo "$OUTPUT"
	false
fi
