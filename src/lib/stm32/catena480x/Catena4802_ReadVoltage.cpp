/*

Module:  Catena4802_ReadVoltage.cpp

Function:
        Catena4802::ReadVin()

Copyright notice:
        See accompanying LICENSE file.

Author:
        Dhinesh Kumar Pitchai, MCCI Corporation	November 2020

Revision history:
        See https://github.com/mcci-catena/Catena-Arduino-Platform

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena4802.h"

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
Catena4802::ReadVin(void) const
	{
	float volt = this->ReadAnalog(Catena480x::ANALOG_CHANNEL_VBAT, 1, 3);
	return volt / 1000;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Catena4802_ReadVoltage.cpp ****/
