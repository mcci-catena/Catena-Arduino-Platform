/*

Module:  Model4917_ReadVoltage.cpp

Function:
        Model4917::ReadVbat() and Model4917::ReadVbus()

Copyright notice:
        See accompanying LICENSE file.

Author:
        Pranau R, MCCI Corporation	November 2022

*/

#ifdef ARDUINO_ARCH_STM32

#include "Model4917.h"
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
Model4917::ReadVbat(void) const
	{
	float volt = this->ReadAnalog(Model4917::ANALOG_CHANNEL_VBAT, 1, 1);
	return volt / 1000;
	}

float
Model4917::ReadVbus(void) const
	{
	float volt = this->ReadAnalog(Model4917::ANALOG_CHANNEL_VBUS, 6, 3);
	return volt / 1000;
	}

#if defined(ARDUINO_MCCI_MODEL_4917) && defined(USBD_LL_ConnectionState_WEAK)

extern "C" {

uint32_t USBD_LL_ConnectionState(void)
	{
	uint32_t vBus;
	bool fStatus;

	fStatus = CatenaStm32L0_ReadAnalog(
			Model4917::ANALOG_CHANNEL_VBUS, 6, 3, &vBus
			);
	return (fStatus && vBus < 3000) ? 0 : 1;
	}

}

#endif // ARDUINO_MCCI_MODEL_4917

#endif // ARDUINO_ARCH_STM32

/**** end of Model4917_ReadVoltage.cpp ****/