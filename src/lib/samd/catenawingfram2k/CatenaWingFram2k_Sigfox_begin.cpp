/* CatenaWingFram2k_LoRaWAN_begin.cpp	Wed Jan 09 2019 14:24:20 chwon */

/*

Module:  CatenaWingFram2k_LoRaWAN_begin.cpp

Function:
	CatenaWingFram2k::LoRaWAN::begin()

Version:
	V0.13.0	Wed Jan 09 2019 14:24:20 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2017-2019 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	March 2017

Revision history:
   0.5.0  Sun Mar 12 2017 19:31:15  tmm
	Module created.

   0.12.0  Wed Dec 05 2018 14:27:52  chwon
	Call Arduino_LoRaWAN::begin() and use Catena addLoRaWanCommands().

   0.13.0  Wed Jan 09 2019 14:24:20  chwon
	Need to set up LoRaWan command before call Arduino_LoRaWAN::begin().

*/

#ifdef ARDUINO_ARCH_SAMD

#include "CatenaWingFram2k.h"

#include "Catena_Log.h"
#include "mcciadk_baselib.h"

using namespace McciCatena;

/*

Name:	CatenaWingFram2k::LoRaWAN::begin()

Function:
	Record linkage to main Catena object and set up LoRaWAN.

Definition:
	bool CatenaWingFram2k::LoRaWAN::begin(
		CatenaWingFram2k *pParent
		);

Description:
	We record parent pointers, and other useful things for later.

Returns:
	true for success, false for failure.

*/

bool 
CatenaWingFram2k::LoRaWAN::begin(
	CatenaWingFram2k *pParent
	)
	{
	gLog.printf(gLog.kTrace, "+CatenaWingFram2k::LoRaWAN::begin()\n");

	this->m_pCatena = pParent;
	this->m_ulDebugMask |= LOG_VERBOSE | LOG_ERRORS | LOG_BASIC;

        /* first set up command processor -- just in case begin() failed */
        pParent->addLoRaWanCommands();

	/* call the base begin */
	if (! this->Arduino_LoRaWAN::begin())
		{
		gLog.printf(
			gLog.kError,
			"?CatenaWingFram2k::LoRaWAN::begin:"
			" Arduino_LoRaWAN:begin failed\n"
			);
		return false;
		}

	/* indicate success to the client */
	return true;
	}

#endif // ARDUINO_ARCH_SAMD
