/* Catena4450_LoRaWAN_begin.cpp	Sun Mar 12 2017 19:31:15 tmm */

/*

Module:  Catena4450_LoRaWAN_begin.cpp

Function:
	Catena4450::LoRaWAN::begin()

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

#include <Catena4450.h>

#include <mcciadk_baselib.h>

/*

Name:	Catena4450::LoRaWAN::begin()

Function:
	Record linkage to main Catena object and set up LoRaWAN.

Definition:
	bool Catena4450::LoRaWAN::begin(
		Catena4450 *pParent
		);

Description:
	We record parent pointers, and other useful things for later.

Returns:
	true for success, false for failure.

*/

bool 
Catena4450::LoRaWAN::begin(
	Catena4450 *pParent
	)
	{
	/* set things up */
	this->m_pCatena = pParent;
	this->m_ulDebugMask |= LOG_VERBOSE | LOG_ERRORS | LOG_BASIC;

	/* call the base begin */
	if (! this->Arduino_LoRaWAN::begin())
		return false;

	/* do any required post-processing */

	/* indicate success to the client */
	return true;
	}