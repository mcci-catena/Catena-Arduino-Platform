/* Catena455x_LoRaWAN_begin.cpp	Fri Oct 13 2017 15:19:30 chwon */

/*

Module:  Catena455x_LoRaWAN_begin.cpp

Function:
	Catena455x::LoRaWAN::begin()

Version:
	V0.6.0	Fri Oct 13 2017 15:19:30 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	ChaeHee Won, MCCI Corporation	October 2017

Revision history:
   0.6.0  Fri Oct 13 2017 15:19:30  chwon
	Module created.

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena455x.h"

#include "Catena_Log.h"
#include "mcciadk_baselib.h"

using namespace McciCatena;

/*

Name:	Catena455x::LoRaWAN::begin()

Function:
	Record linkage to main Catena object and set up LoRaWAN.

Definition:
	bool Catena455x::LoRaWAN::begin(
		Catena455x *pParent
		);

Description:
	We record parent pointers, and other useful things for later.

Returns:
	true for success, false for failure.

*/

bool
Catena455x::LoRaWAN::begin(
	Catena455x *pParent
	)
	{
        static const char FUNCTION[] = "Catena455x::LoRaWAN::begin";

	/* set things up */
	this->m_pCatena = pParent;
	this->m_ulDebugMask |= LOG_VERBOSE | LOG_ERRORS | LOG_BASIC;

        /* set up command processor */
        this->addCommands();

	/* call the base begin */
	if (! this->Super::begin(pParent))
                {
                gLog.printf(
                        gLog.kBug,
                        "%s: Super::begin() failed\n",
                        FUNCTION
                        );
		return false;
                }

	/* indicate success to the client */
	return true;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Catena455x_LoRaWAN_begin.cpp ****/
