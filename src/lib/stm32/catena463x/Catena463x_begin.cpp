/*

Module:  Catena463x_begin.cpp

Function:
        Catena463x::begin().

Copyright notice:
        See accompanying LICENSE file.

Author:
        Dhinesh Kumar Pitchai, MCCI Corporation	April 2019

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena463x.h"

#include "Catena_Log.h"

using namespace McciCatena;

/*

Name:	Catena463x::begin()

Function:
	Set up all the well-known board peripherals.

Definition:
	bool Catena463x::begin();

Description:
	Issues begin() for all the Catena463x things.

Returns:
	true for success, false for failure.

*/

bool Catena463x::begin()
	{
	Serial.begin(115200);
	Wire.begin();
	delay(1000);
	gLog.begin(cLog::DebugFlags(gLog.kError | gLog.kBug));
	gLog.printf(
		gLog.kTrace,
		"\n+Catena463x::begin() for %s\n",
		CatenaName()
		);

	// do the platform selection.
	if (! this->Super::begin())
		return false;

	return true;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Catena463x_begin.cpp ****/
