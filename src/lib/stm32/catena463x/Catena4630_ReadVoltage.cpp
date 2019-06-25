/*

Module:  Catena4630_ReadAnalog.cpp

Function:
        Catena4630::ReadVbat() and Catena4630::ReadVbus()

Copyright notice:
        See accompanying LICENSE file.

Author:
        Dhinesh Kumar Pitchai, MCCI Corporation	April 2019

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena4630.h"
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
Catena4630::ReadVbat(void) const
	{
	float volt = this->ReadAnalog(Catena463x::ANALOG_CHANNEL_VBAT, 1, 2);
	return volt / 1000;
	}

float
Catena4630::ReadVbus(void) const
	{
	float volt = this->ReadAnalog(Catena463x::ANALOG_CHANNEL_VBUS, 1, 3);
	return volt / 1000;
	}

#if defined(ARDUINO_MCCI_CATENA_4630) && defined(USBD_LL_ConnectionState_WEAK)

extern "C" {

uint32_t USBD_LL_ConnectionState(void)
	{
	uint32_t vBus;
	bool fStatus;

	fStatus = CatenaStm32L0_ReadAnalog(
			Catena463x::ANALOG_CHANNEL_VBUS, 1, 3, &vBus
			);
	return (fStatus && vBus < 3000) ? 0 : 1;
	}

}

#endif // ARDUINO_MCCI_CATENA_4630

#endif // ARDUINO_ARCH_STM32

/**** end of Catena4630_ReadAnalog.cpp ****/
