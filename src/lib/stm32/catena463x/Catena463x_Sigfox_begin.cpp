/*

Module:  Catena463x_LoRaWAN_begin.cpp

Function:
        Catena463x::LoRaWAN::begin()

Copyright notice:
        See accompanying LICENSE file.

Author:
        Dhinesh Kumar Pitchai, MCCI Corporation	April 2019

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena463x.h"

#include "Catena_Log.h"
#include "mcciadk_baselib.h"

using namespace McciCatena;

/*

Name:	Catena463x::LoRaWAN::begin()

Function:
	Record linkage to main Catena object and set up LoRaWAN.

Definition:
	bool Catena463x::LoRaWAN::begin(
		Catena463x *pParent
		);

Description:
	We record parent pointers, and other useful things for later.

Returns:
	true for success, false for failure.

*/

bool
Catena463x::LoRaWAN::begin(
	Catena463x *pParent
	)
	{
	gLog.printf(gLog.kTrace, "+Catena463x::LoRaWAN::begin()\n");

	/* call the base begin */
	if (! this->Super::begin(pParent))
		{
		gLog.printf(
			gLog.kBug,
			"?Catena463x::LoRaWAN::begin: Super::begin() failed\n"
			);
		return false;
		}

	/* indicate success to the client */
	return true;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Catena463x_LoRaWAN_begin.cpp ****/
