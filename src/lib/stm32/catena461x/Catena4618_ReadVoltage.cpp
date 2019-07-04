/*

Module:  Catena4618_ReadAnalog.cpp

Function:
        Catena4618::ReadVbat() and Catena4618::ReadVbus()

Copyright notice:
        See accompanying LICENSE file.

Author:
        Lakshmi Priya Natarajan, MCCI Corporation	June 2019

Revision history:
        See https://github.com/mcci-catena/Catena-Arduino-Platform

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena4618.h"
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
Catena4618::ReadVbat(void) const
	{
	float volt = this->ReadAnalog(Catena461x::ANALOG_CHANNEL_VBAT, 1, 1);
	return volt / 1000;
	}

float
Catena4618::ReadVbus(void) const
	{
	// on the Catena 4618, the Vbus divider impedance is very high,
	// so charge transfer from the previous channel causes problems.
	// Therefore, let's read one extra time; the prvious ADC will
	// have had some time to bleed off.
	float volt = this->ReadAnalog(Catena461x::ANALOG_CHANNEL_VBUS, 6, 3);
	return volt / 1000;
	}

#if defined(ARDUINO_MCCI_CATENA_4618) && defined(USBD_LL_ConnectionState_WEAK)

extern "C" {

uint32_t USBD_LL_ConnectionState(void)
	{
	uint32_t vBus;
	bool fStatus;

	fStatus = CatenaStm32L0_ReadAnalog(
			Catena461x::ANALOG_CHANNEL_VBUS, 6, 3, &vBus
			);
	return (fStatus && vBus < 3000) ? 0 : 1;
	}

}

#endif // ARDUINO_MCCI_CATENA_4618

#endif // ARDUINO_ARCH_STM32

/**** end of Catena4618_ReadAnalog.cpp ****/
