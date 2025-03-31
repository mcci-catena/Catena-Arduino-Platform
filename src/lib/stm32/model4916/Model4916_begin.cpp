/*

Module:  Model4916_begin.cpp

Function:
        Model4916::begin().

Copyright notice:
        See accompanying LICENSE file.

Author:
        Pranau R, MCCI Corporation	November 2022

*/

#ifdef ARDUINO_ARCH_STM32

#include "Model4916.h"

#include "Catena_Log.h"

using namespace McciCatena;

/*

Name:	Model4916::begin()

Function:
	Set up all the well-known board peripherals.

Definition:
	bool Model4916::begin();

Description:
	Issues begin() for all the Model4916 things.

Returns:
	true for success, false for failure.

*/

bool Model4916::begin()
	{
	Serial.begin(115200);
	Wire.begin();
	delay(1000);
	gLog.begin(cLog::DebugFlags(gLog.kError | gLog.kBug));
	gLog.printf(
		gLog.kTrace,
		"\n+Model4916::begin() for %s\n",
		CatenaName()
		);

	// do the platform selection.
	if (! this->Super::begin())
		return false;

	return true;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Model4916_begin.cpp ****/