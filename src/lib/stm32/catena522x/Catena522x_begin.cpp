/*

Module:  Catena522x_begin.cpp

Function:
        Catena522x::begin().

Copyright notice:
        See accompanying LICENSE file.

Author:
        Pranau Ravikumar, MCCI Corporation	August 2026

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena522x.h"

#include "Catena_Log.h"

using namespace McciCatena;

/*

Name:	Catena522x::begin()

Function:
	Set up all the well-known board peripherals.

Definition:
	bool Catena522x::begin();

Description:
	Issues begin() for all the Catena522x things.

Returns:
	true for success, false for failure.

*/

bool Catena522x::begin()
	{
	Serial.begin(115200);
	Wire.begin();
	delay(1000);
	gLog.begin(cLog::DebugFlags(gLog.kError | gLog.kBug));
	gLog.printf(
		gLog.kTrace,
		"\n+Catena522x::begin() for %s\n",
		CatenaName()
		);

	// do the platform selection.
	if (! this->Super::begin())
		return false;

	return true;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Catena522x_begin.cpp ****/