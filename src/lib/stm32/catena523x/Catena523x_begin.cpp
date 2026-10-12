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
// the single nPM1300 PMIC instance for this board. Private, file-scope
// storage -- reached only through Catena523x::getPmic(), not a
// library-wide free global.
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

	// begin the PMIC. On the Catena 5230, the PMIC's own regulator
	// outputs (VOUT1, LSOUT2) are what power the FRAM, the flash, and
	// the on-board sensors -- they are not powered directly from the
	// battery/USB rails. So if the PMIC doesn't start, those
	// peripherals have no power either, and the board can't do its
	// job even though the MCU and radio might still be alive. Fail
	// begin() outright rather than limping along with silently-dead
	// peripherals.
	if (! this->m_pNpm1300->begin())
		{
		gLog.printf(
			gLog.kError,
			"?Catena523x::begin: PMIC begin() failed\n"
			);
		return false;
		}

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

#endif // ARDUINO_ARCH_STM32 && ARDUINO_MCCI_CATENA_5230

/**** end of Catena523x_begin.cpp ****/
