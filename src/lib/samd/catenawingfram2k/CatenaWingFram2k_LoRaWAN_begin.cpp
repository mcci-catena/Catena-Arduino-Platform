/* CatenaWingFram2k_LoRaWAN_begin.cpp	Sun Mar 12 2017 19:31:15 tmm */

/*

Module:  CatenaWingFram2k_LoRaWAN_begin.cpp

Function:
	CatenaWingFram2k::LoRaWAN::begin()

Version:
	V0.5.0	Sun Mar 12 2017 19:31:15 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2017 by

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
        static const char FUNCTION[] = "CatenaWingFram2k::LoRaWAN::begin";

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

#endif // ARDUINO_ARCH_SAMD
