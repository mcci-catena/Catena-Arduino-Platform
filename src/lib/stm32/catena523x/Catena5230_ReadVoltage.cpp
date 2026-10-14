/*

Module:  Catena5230_ReadVoltage.cpp

Function:
        Catena5230::ReadVbat() and Catena5230::ReadVbus()

Copyright notice:
        See accompanying LICENSE file.

Author:
        Murali, MCCI Corporation	Sep 2025

*/

#if defined(ARDUINO_ARCH_STM32) && defined(ARDUINO_MCCI_CATENA_5230)

#include "Catena5230.h"
#include "Catena_Log.h"

#include <Arduino.h>
#include <MCCI_Catena_nPM1300.h>

using namespace McciCatena;
using namespace McciCatenaNpm1300;

/****************************************************************************\
|
|		Manifest constants & typedefs.
|
|	This is strictly for private types and constants which will not
|	be exported.
|
\****************************************************************************/

/****************************************************************************\
|
|	Read-only data.
|
|	If program is to be ROM-able, these must all be tagged read-only
|	using the ROM storage class; they may be global.
|
\****************************************************************************/



/****************************************************************************\
|
|	VARIABLES:
|
|	If program is to be ROM-able, these must be initialized
|	using the BSS keyword.  (This allows for compilers that require
|	every variable to have an initializer.)  Note that only those
|	variables owned by this module should be declared here, using the BSS
|	keyword; this allows for linkers that dislike multiple declarations
|	of objects.
|
\****************************************************************************/

float
Catena5230::ReadVbat(void) const
	{
	// Catena523x::begin() fails outright if the PMIC doesn't start (see
	// the comment there), so by the time this is called, m_pNpm1300 is
	// known to be running. measureVbat() can still fail on a given call
	// (I2C error, or the conversion didn't finish in time); ReadVbat()'s
	// signature has no way to report that, so log it and return 0.0,
	// an otherwise-impossible battery voltage.
	float volt;
	if (! this->m_pNpm1300->measureVbat(volt))
		{
		gLog.printf(
			gLog.kError,
			"?Catena5230::ReadVbat: measureVbat() failed: %s\n",
			this->m_pNpm1300->getLastErrorName()
			);
		return 0.0f;
		}
	return volt;
	}

float
Catena5230::ReadVbus(void) const
	{
	// Catena523x::begin() fails outright if the PMIC doesn't start (see
	// the comment there), so by the time this is called, m_pNpm1300 is
	// known to be running. measureVbus() can still fail on a given call
	// (I2C error, or the conversion didn't finish in time); ReadVbus()'s
	// signature has no way to report that, so log it and return 0.0,
	// an otherwise-impossible bus voltage.
	float volt;
	if (! this->m_pNpm1300->measureVbus(volt))
		{
		gLog.printf(
			gLog.kError,
			"?Catena5230::ReadVbus: measureVbus() failed: %s\n",
			this->m_pNpm1300->getLastErrorName()
			);
		return 0.0f;
		}
	return volt;
	}

#endif // ARDUINO_ARCH_STM32 && ARDUINO_MCCI_CATENA_5230

/**** end of Catena5230_ReadVoltage.cpp ****/
