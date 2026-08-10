/*

Module:  Catena5220_ReadVoltage.cpp

Function:
        Catena5220::ReadVbat() and Catena5220::ReadVbus()

Copyright notice:
        See accompanying LICENSE file.

Author:
        Pranau Ravikumar, MCCI Corporation	August 2026

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena5220.h"
#include "Catena_Log.h"

#include <Arduino.h>
using namespace McciCatena;

/****************************************************************************\
|
|	Manifest constants & typedefs.
|
\****************************************************************************/



/****************************************************************************\
|
|	Read-only data.
|
\****************************************************************************/



/****************************************************************************\
|
|	Variables.
|
\****************************************************************************/

float
Catena5220::ReadVbat(void) const
	{
	float volt = this->ReadAnalog(Catena522x::ANALOG_CHANNEL_VBAT, 1, 3);
	return volt / 1000;
	}

float
Catena5220::ReadVbus(void) const
	{
	return 0.0;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Catena5220_ReadAnalog.cpp ****/