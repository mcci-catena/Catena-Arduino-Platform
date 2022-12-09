/*

Module:  Catena491x_begin.cpp

Function:
        Catena491x::begin().

Copyright notice:
        See accompanying LICENSE file.

Author:
        Pranau R, MCCI Corporation	November 2022

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena491x.h"

#include "Catena_Log.h"

using namespace McciCatena;

/*

Name:	Catena491x::begin()

Function:
	Set up all the well-known board peripherals.

Definition:
	bool Catena491x::begin();

Description:
	Issues begin() for all the Catena491x things.

Returns:
	true for success, false for failure.

*/

bool Catena491x::begin()
	{
	Serial.begin(115200);
	Wire.begin();
	delay(1000);
	gLog.begin(cLog::DebugFlags(gLog.kError | gLog.kBug));
	gLog.printf(
		gLog.kTrace,
		"\n+Catena491x::begin() for %s\n",
		CatenaName()
		);

	// do the platform selection.
	if (! this->Super::begin())
		return false;

	return true;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Catena491x_begin.cpp ****/