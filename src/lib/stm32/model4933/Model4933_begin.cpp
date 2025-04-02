/*

Module:  Model4933_begin.cpp

Function:
        Model4933::begin().

Copyright notice:
        See accompanying LICENSE file.

Author:
        Pranau R, MCCI Corporation	October 2023

*/

#ifdef ARDUINO_ARCH_STM32

#include "Model4933.h"

#include "Catena_Log.h"

using namespace McciCatena;

/*

Name:   Model4933::begin()

Function:
        Set up all the well-known board peripherals.

Definition:
        bool Model4933::begin();

Description:
        Issues begin() for all the Model4933 things.

Returns:
        true for success, false for failure.

*/

bool Model4933::begin()
	{
	Serial.begin(115200);
	Wire.begin();
	delay(1000);
	gLog.begin(cLog::DebugFlags(gLog.kError | gLog.kBug));
	gLog.printf(
		gLog.kTrace,
		"\n+Model4933::begin() for %s\n",
		CatenaName()
		);

	// do the platform selection.
	if (! this->Super::begin())
		return false;

	return true;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Model4933_begin.cpp ****/