/*

Module:  Model4917_begin.cpp

Function:
        Model4917::begin().

Copyright notice:
        See accompanying LICENSE file.

Author:
        Pranau R, MCCI Corporation	November 2022

*/

#ifdef ARDUINO_ARCH_STM32

#include "Model4917.h"

#include "Catena_Log.h"

using namespace McciCatena;

/*

Name:	Model4917::begin()

Function:
	Set up all the well-known board peripherals.

Definition:
	bool Model4917::begin();

Description:
	Issues begin() for all the Model4917 things.

Returns:
	true for success, false for failure.

*/

bool Model4917::begin()
	{
	Serial.begin(115200);
	Wire.begin();
	delay(1000);
	gLog.begin(cLog::DebugFlags(gLog.kError | gLog.kBug));
	gLog.printf(
		gLog.kTrace,
		"\n+Model4917::begin() for %s\n",
		CatenaName()
		);

	// do the platform selection.
	if (! this->Super::begin())
		return false;

	return true;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Model4917_begin.cpp ****/