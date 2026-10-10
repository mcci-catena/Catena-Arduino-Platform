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
	float volt = gNpm1300.measureVbat();
	return volt;
	}

float
Catena5230::ReadVbus(void) const
	{
	float volt = gNpm1300.measureVbus();
	return volt;
	}

#endif // ARDUINO_ARCH_STM32 && ARDUINO_MCCI_CATENA_5230

/**** end of Catena5230_ReadVoltage.cpp ****/
