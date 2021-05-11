/*

Module:  Catena455x_Sigfox_begin.cpp

Function:
        Catena455x::Sigfox::begin()

Copyright notice:
        See accompanying LICENSE file.

Author:
	Dhinesh Kumar Pitchai, MCCI Corporation	January 2020

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena455x.h"

#include "Catena_Log.h"
#include "mcciadk_baselib.h"

using namespace McciCatena;

/*

Name:	Catena455x::Sigfox::begin()

Function:
	Record linkage to main Catena object and set up Sigfox.

Definition:
	bool Catena455x::Sigfox::begin(
		Catena455x *pParent
		);

Description:
	We record parent pointers, and other useful things for later.

Returns:
	true for success, false for failure.

*/

bool
Catena455x::Sigfox::begin(
	Catena455x *pParent
	)
	{
	gLog.printf(gLog.kTrace, "+Catena455x::Sigfox::begin()\n");

	/* call the base begin */
	if (! this->Super::begin(pParent))
		{
		gLog.printf(
			gLog.kBug,
			"?Catena455x::Sigfox::begin: Super::begin() failed\n"
			);
		return false;
		}

	/* indicate success to the client */
	return true;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Catena455x_Sigfox_begin.cpp ****/
