/* Catena461x_LoRaWAN_begin.cpp	Thu Nov 15 2018 14:54:47 chwon */

/*

Module:  Catena461x_LoRaWAN_begin.cpp

Function:
	Catena461x::LoRaWAN::begin()

Version:
	V0.11.0	Thu Nov 15 2018 14:54:47 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	November 2018

Revision history:
   0.11.0  Thu Nov 15 2018 14:54:47  chwon
	Module created.

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena461x.h"

#include "Catena_Log.h"
#include "mcciadk_baselib.h"

using namespace McciCatena;

/*

Name:	Catena461x::LoRaWAN::begin()

Function:
	Record linkage to main Catena object and set up LoRaWAN.

Definition:
	bool Catena461x::LoRaWAN::begin(
		Catena461x *pParent
		);

Description:
	We record parent pointers, and other useful things for later.

Returns:
	true for success, false for failure.

*/

bool
Catena461x::LoRaWAN::begin(
	Catena461x *pParent
	)
	{
	gLog.printf(gLog.kTrace, "+Catena461x::LoRaWAN::begin()\n");

	/* call the base begin */
	if (! this->Super::begin(pParent))
		{
		gLog.printf(
			gLog.kBug,
			"?Catena461x::LoRaWAN::begin: Super::begin() failed\n"
			);
		return false;
		}

	/* indicate success to the client */
	return true;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Catena461x_LoRaWAN_begin.cpp ****/
