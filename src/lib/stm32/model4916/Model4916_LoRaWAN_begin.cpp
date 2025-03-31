/*

Module:  Model4916_LoRaWAN_begin.cpp

Function:
        Model4916::LoRaWAN::begin()

Copyright notice:
        See accompanying LICENSE file.

Author:
        Pranau R, MCCI Corporation	November 2022

*/

#ifdef ARDUINO_ARCH_STM32

#include "Model4916.h"

#include "Catena_Log.h"
#include "mcciadk_baselib.h"

using namespace McciCatena;

/*

Name:	Model4916::LoRaWAN::begin()

Function:
	Record linkage to main Catena object and set up LoRaWAN.

Definition:
	bool Model4916::LoRaWAN::begin(
		Model4916 *pParent
		);

Description:
	We record parent pointers, and other useful things for later.

Returns:
	true for success, false for failure.

*/

bool
Model4916::LoRaWAN::begin(
	Model4916 *pParent
	)
	{
	gLog.printf(gLog.kTrace, "+Model4916::LoRaWAN::begin()\n");

	/* call the base begin */
	if (! this->Super::begin(pParent))
		{
		gLog.printf(
			gLog.kBug,
			"?Model4916::LoRaWAN::begin: Super::begin() failed\n"
			);
		return false;
		}

	/* indicate success to the client */
	return true;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Model4916_LoRaWAN_begin.cpp ****/