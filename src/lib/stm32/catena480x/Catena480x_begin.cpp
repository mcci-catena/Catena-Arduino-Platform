/* Catena480x_begin.cpp	Thu Nov 15 2018 14:52:31 chwon */

/*

Module:  Catena480x_begin.cpp

Function:
	Catena480x::begin()

Version:
	V0.11.0	Thu Nov 15 2018 14:52:31 chwon	Edit level 1

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
   0.11.0  Thu Nov 15 2018 14:52:31  chwon
	Module created.

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena480x.h"

#include "Catena_Log.h"

using namespace McciCatena;

/*

Name:	Catena480x::begin()

Function:
	Set up all the well-known board peripherals.

Definition:
	bool Catena480x::begin();

Description:
	Issues begin() for all the Catena480x things.

Returns:
	true for success, false for failure.

*/

bool Catena480x::begin()
	{
	Serial.begin(115200);
	Wire.begin();
	delay(1000);
	gLog.begin(cLog::DebugFlags(gLog.kError | gLog.kBug));
	gLog.printf(
		gLog.kTrace,
		"\n+Catena480x::begin() for %s\n",
		CatenaName()
		);

	// do the platform selection.
	if (! this->Super::begin())
		return false;

	return true;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Catena480x_begin.cpp ****/
