/* Catena455x_begin.cpp	Tue Nov 20 2018 13:53:31 chwon */

/*

Module:  Catena455x_begin.cpp

Function:
	Catena455x::begin()

Version:
	V0.11.0	Tue Nov 20 2018 13:53:31 chwon	Edit level 3

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

   0.6.0  Wed Dec 06 2017 15:31:37  chwon
	Set debug flag.

   0.11.0  Tue Nov 20 2018 13:53:31  chwon
	Move common code to CatenaStm32L0::begin().

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena455x.h"

#include "Catena_Log.h"

using namespace McciCatena;

/*

Name:	Catena455x::begin()

Function:
	Set up all the well-known board peripherals.

Definition:
	bool Catena455x::begin();

Description:
	Issues begin() for all the Catena455x things.

Returns:
	true for success, false for failure.

*/

bool Catena455x::begin()
	{
	Serial.begin(115200);
	Wire.begin();
	delay(1000);
	gLog.begin(cLog::DebugFlags(gLog.kError | gLog.kBug));
	gLog.printf(
		gLog.kTrace,
		"\n+Catena455x::begin() for %s\n",
		CatenaName()
		);

	// do the platform selection.
	if (! this->Super::begin())
		return false;

	return true;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Catena455x_begin.cpp ****/
