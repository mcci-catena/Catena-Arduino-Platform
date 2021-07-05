/* CatenaStm32L0_LoRaWAN_begin.cpp	Mon Jan 07 2019 11:36:38 chwon */

/*

Module:  CatenaStm32L0_LoRaWAN_begin.cpp

Function:
	CatenaStm32L0::LoRaWAN::begin()

Version:
	V0.13.0	Mon Jan 07 2019 11:36:38 chwon	Edit level 5

Copyright notice:
	This file copyright (C) 2017-2018 by

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

   0.10.0  Wed May 09 2018 12:15:37  chwon
	Use PIN_SX1276_ANT_SWITCH_TX_BOOST and PIN_SX1276_ANT_SWITCH_TX_RFO.

   0.12.0  Mon Nov 26 2018 15:48:10  chwon
	Remvoe PIN_SX1276_ANT_SWITCH_TX_BOOST and PIN_SX1276_ANT_SWITCH_TX_RFO
	pin initialization.

   0.12.0  Wed Dec 05 2018 14:27:41  chwon
	Use Catena addLoRaWanCommands().

   0.13.0  Mon Jan 07 2019 11:36:39  chwon
	Need to set up LoRaWan command before call Arduino_LoRaWAN::begin().

*/

#ifdef ARDUINO_ARCH_STM32

#include <CatenaStm32L0.h>

#include <Catena_Log.h>
#include <mcciadk_baselib.h>
#include <arduino_lmic.h>

using namespace McciCatena;

/****************************************************************************\
|
|		Manifest constants & typedefs.
|
|	This is strictly for private types and constants which will not
|	be exported.
|
\****************************************************************************/



/****************************************************************************\
|
|	Read-only data.
|
|	If program is to be ROM-able, these must all be tagged read-only
|	using the ROM storage class; they may be global.
|
\****************************************************************************/


/****************************************************************************\
|
|	VARIABLES:
|
|	If program is to be ROM-able, these must be initialized
|	using the BSS keyword.  (This allows for compilers that require
|	every variable to have an initializer.)  Note that only those
|	variables owned by this module should be declared here, using the BSS
|	keyword; this allows for linkers that dislike multiple declarations
|	of objects.
|
\****************************************************************************/

bool CatenaStm32L0::LoRaWAN::begin(
	CatenaStm32L0 *pParent
	)
	{
	gLog.printf(gLog.kTrace, "+CatenaStm32L0::LoRaWAN::begin()\n");

	this->m_pCatena = pParent;
	this->m_ulDebugMask |= LOG_VERBOSE | LOG_ERRORS | LOG_BASIC;

        /* first set up command processor -- just in case begin() failed */
        pParent->addLoRaWanCommands();

	/* call the base begin */
	if (! this->Arduino_LoRaWAN::begin())
		{
		gLog.printf(
			gLog.kError,
			"?CatenaStm32L0::LoRaWAN::begin:"
			" Arduino_LoRaWAN:begin failed\n"
			);
		return false;
		}

	if (pParent->GetSystemClockRate() < 16000000)
		{
	        // set clock error as high as possible
		LMIC_setClockError(5 * MAX_CLOCK_ERROR / 100);
		}
	else
		{
	        // set clock error to 0.4%; the HSI clock can be that far off,
		// and it can kill joins if we don't set this.
		LMIC_setClockError(4 * MAX_CLOCK_ERROR / 1000);
		}

	/* indicate success to the client */
	return true;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32L0_LoRaWAN_begin.cpp ****/
