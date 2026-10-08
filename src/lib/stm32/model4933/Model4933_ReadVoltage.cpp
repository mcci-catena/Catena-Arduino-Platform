/*

Module:  Model4933_ReadVoltage.cpp

Function:
        Model4933::ReadVbat() and Model4933::ReadVbus()

Copyright notice:
        See accompanying LICENSE file.

Author:
        Pranau R, MCCI Corporation	October 2023

*/

#ifdef ARDUINO_ARCH_STM32

#include "Model4933.h"
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
Model4933::ReadVbat(void) const
	{
	float volt = this->ReadAnalog(Model4933::ANALOG_CHANNEL_VBAT, 1, 2);
	return volt / 1000;
	}

float
Model4933::ReadVbus(void) const
	{
	float volt = this->ReadAnalog(Model4933::ANALOG_CHANNEL_VBUS, 1, 3);
	return volt / 1000;
	}

#if defined(ARDUINO_MCCI_MODEL_4933) && defined(USBD_LL_ConnectionState_WEAK)

extern "C" {

uint32_t USBD_LL_ConnectionState(void)
	{
	uint32_t vBus;
	bool fStatus;

	fStatus = CatenaStm32L0_ReadAnalog(
			Model4933::ANALOG_CHANNEL_VBUS, 1, 3, &vBus
			);
	return (fStatus && vBus < 3000) ? 0 : 1;
	}

}

#endif // ARDUINO_MCCI_MODEL_4933

#endif // ARDUINO_ARCH_STM32

/**** end of Model4933_ReadVoltage.cpp ****/