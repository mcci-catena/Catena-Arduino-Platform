/*

Module:  Catena523x_begin.cpp

Function:
        Catena523x::Catena523x() and Catena523x::begin()

Copyright notice:
        See accompanying LICENSE file.

Author:
        Murali, MCCI Corporation	Sep 2025

*/

#if defined(ARDUINO_ARCH_STM32) && defined(ARDUINO_MCCI_CATENA_5230)

#include "Catena523x.h"

#include "Catena_Log.h"

using namespace McciCatena;
using namespace McciCatenaNpm1300;

namespace {
// the single nPM1300 PMIC instance for this board, reached through
// Catena523x::getPmic().
McciCatenaNpm1300::cNPM1300 gNpm1300Storage {WirePMIC};
}

Catena523x::Catena523x()
	: m_pNpm1300(&gNpm1300Storage)
	{}

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

	gLog.begin(cLog::DebugFlags(gLog.kError | gLog.kBug));

	// The PMIC's VOUT1 and load switches power the on-board sensors and
	// other peripherals, so begin() fails if the PMIC doesn't start. The
	// 5230 has no battery thermistor (the nPM1300's NTC pin is grounded).
	// The log can't reach the console this early (USB isn't up); the
	// sketch reports the failure from begin()'s result.
	if (! this->m_pNpm1300->begin(cNPM1300::Ntc::NotFitted))
		{
		gLog.printf(
			gLog.kError,
			"?Catena523x::begin: PMIC begin() failed: %s\n",
			this->m_pNpm1300->getLastErrorName()
			);
		return false;
		}

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

#endif // ARDUINO_ARCH_STM32 && ARDUINO_MCCI_CATENA_5230

/**** end of Catena523x_begin.cpp ****/
