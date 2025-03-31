/*

Module:  Model4916_ReadVoltage.cpp

Function:
        Model4916::ReadVbat() and Model4916::ReadVbus()

Copyright notice:
        See accompanying LICENSE file.

Author:
        Pranau R, MCCI Corporation	November 2022

*/

#ifdef ARDUINO_ARCH_STM32

#include "Model4916.h"
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
Model4916::ReadVbat(void) const
	{
	float volt = this->ReadAnalog(Model4916::ANALOG_CHANNEL_VBAT, 1, 2);
	return volt / 1000;
	}

float
Model4916::ReadVbus(void) const
	{
	return 0;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Model4916_ReadVoltage.cpp ****/