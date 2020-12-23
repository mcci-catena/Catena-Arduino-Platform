/* Catena455x_LoRaWAN_begin.cpp	Tue Nov 20 2018 13:55:23 chwon */

/*

Module:  Catena455x_LoRaWAN_begin.cpp

Function:
	Catena455x::LoRaWAN::begin()

Version:
	V0.11.0	Tue Nov 20 2018 13:55:23 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2017-2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	ChaeHee Won, MCCI Corporation	October 2017

Revision history:
   0.6.0  Fri Oct 13 2017 15:19:30  chwon
	Module created.

   0.11.0  Tue Nov 20 2018 13:55:23  chwon
	Move common code to CatenaStm32L0::LoRaWAN::begin().

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena455x.h"

#include "Catena_Log.h"
#include "mcciadk_baselib.h"

using namespace McciCatena;

/*

Name:	Catena455x::LoRaWAN::begin()

Function:
	Record linkage to main Catena object and set up LoRaWAN.

Definition:
	bool Catena455x::LoRaWAN::begin(
		Catena455x *pParent
		);

Description:
	We record parent pointers, and other useful things for later.

Returns:
	true for success, false for failure.

*/

bool
Catena455x::LoRaWAN::begin(
	Catena455x *pParent
	)
	{
	gLog.printf(gLog.kTrace, "+Catena455x::LoRaWAN::begin()\n");

	/* call the base begin */
	if (! this->Super::begin(pParent))
		{
		gLog.printf(
			gLog.kBug,
			"?Catena455x::LoRaWAN::begin: Super::begin() failed\n"
			);
		return false;
		}

	/* indicate success to the client */
	return true;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Catena455x_LoRaWAN_begin.cpp ****/
