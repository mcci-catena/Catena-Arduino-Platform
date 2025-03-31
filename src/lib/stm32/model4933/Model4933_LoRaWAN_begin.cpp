/*

Module:  Model4933_LoRaWAN_begin.cpp

Function:
        Model4933::LoRaWAN::begin()

Copyright notice:
        See accompanying LICENSE file.

Author:
        Pranau R, MCCI Corporation	October 2023

*/

#ifdef ARDUINO_ARCH_STM32

#include "Model4933.h"

#include "Catena_Log.h"
#include "mcciadk_baselib.h"

using namespace McciCatena;

/*

Name:   Model4933::LoRaWAN::begin()

Function:
        Record linkage to main Catena object and set up LoRaWAN.

Definition:
        bool Model4933::LoRaWAN::begin(
        Model4933 *pParent
        );
Description:
        We record parent pointers, and other useful things for later.
Returns:
        true for success, false for failure.
*/

bool
Model4933::LoRaWAN::begin(
	Model4933 *pParent
	)
	{
	gLog.printf(gLog.kTrace, "+Model4933::LoRaWAN::begin()\n");

	/* call the base begin */
	if (! this->Super::begin(pParent))
		{
		gLog.printf(
			gLog.kBug,
			"?Model4933::LoRaWAN::begin: Super::begin() failed\n"
			);
		return false;
		}

	/* indicate success to the client */
	return true;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Model4933_LoRaWAN_begin.cpp ****/