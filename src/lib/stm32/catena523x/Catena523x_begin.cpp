/*

Module:  Catena523x_begin.cpp

Function:
        class Catena5230: CatenaBase Platform to represent a Catena 5230
        (such as the 5230).

Copyright notice:
        See accompanying LICENSE file.

Author:
        Murali, MCCI Corporation	Sep 2025

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena523x.h"

#include "Catena_Log.h"
#include <MCCI_Catena_nPM1300.h>

using namespace McciCatena;
using namespace McciCatenaNpm1300;

McciCatenaNpm1300::cNPM1300 gNpm1300 {WirePMIC};

/*

Name:	Catena523x::begin()

Function:
	Set up all the well-known board peripherals.

Definition:
	bool Catena523x::begin();

Description:
	Issues begin() for all the Catena523x things.

Returns:
	true for success, false for failure.

*/

bool Catena523x::begin()
	{
	Serial.begin(115200);
	Wire.begin();
	delay(1000);
	// PMIC I2C begin
	WirePMIC.begin();
	delay(1000);
	// begin PMIC module
	gNpm1300.begin();
	gLog.begin(cLog::DebugFlags(gLog.kError | gLog.kBug));
	gLog.printf(
		gLog.kTrace,
		"\n+Catena523x::begin() for %s\n",
		CatenaName()
		);

	// do the platform selection.
	if (! this->Super::begin())
		return false;

	return true;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Catena523x_begin.cpp ****/
