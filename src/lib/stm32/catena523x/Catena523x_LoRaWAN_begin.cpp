/*

Module:  Catena523x_LoRaWAN_begin.cpp

Function:
        class Catena5230: CatenaBase Platform to represent a Catena 5230
        (such as the 5230).

Copyright notice:
        See accompanying LICENSE file.

Author:
        Murali, MCCI Corporation	Sep 2025

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena523x.h"

#include "Catena_Log.h"
#include "mcciadk_baselib.h"

using namespace McciCatena;

/*

Name:	Catena523x::LoRaWAN::begin()

Function:
	Record linkage to main Catena object and set up LoRaWAN.

Definition:
	bool Catena523x::LoRaWAN::begin(
		Catena523x *pParent
		);

Description:
	We record parent pointers, and other useful things for later.

Returns:
	true for success, false for failure.

*/

bool
Catena523x::LoRaWAN::begin(
	Catena523x *pParent
	)
	{
	gLog.printf(gLog.kTrace, "+Catena523x::LoRaWAN::begin()\n");

	/* call the base begin */
	if (! this->Super::begin(pParent))
		{
		gLog.printf(
			gLog.kBug,
			"?Catena523x::LoRaWAN::begin: Super::begin() failed\n"
			);
		return false;
		}

	/* indicate success to the client */
	return true;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Catena523x_LoRaWAN_begin.cpp ****/
