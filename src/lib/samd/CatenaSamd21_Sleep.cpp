/* CatenaSamd21_Sleep.cpp	Wed Oct 31 2018 11:46:53 chwon */

/*

Module:  CatenaSamd21_Sleep.cpp

Function:
	Home for CatenaSamd21::Sleep()

Version:
	V3.21h	Wed Oct 31 2018 11:46:53 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	October 2018

Revision history:
   3.21h  Wed Oct 31 2018 11:46:53  chwon
	Module created.

*/

#ifdef ARDUINO_ARCH_SAMD

#include "CatenaSamd21.h"

#include <Arduino.h>
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


/*

Name:	CatenaSamd21::Sleep()

Function:
	Put system in sleep mode

Definition:
	void CatenaSamd21::Sleep(uint32_t howLongInSeconds);

Description:
	This function sets the alarm with given howLongInSeconds and puts the
	system in low power sleep mode. The system will be wake up when alarm
	happen in howLongInSeconds.

Returns:
	No explicit result.

*/

void CatenaSamd21::Sleep(uint32_t howLongInSeconds)
	{
	this->m_Rtc.SetAlarm(howLongInSeconds);
	this->m_Rtc.SleepForAlarm(
			this->m_Rtc.MATCH_HHMMSS,
			this->m_Rtc.SleepMode::DeepSleep
			);

	// add the number of ms that we were asleep to the millisecond timer.
	// we don't need extreme accuracy.
	adjust_millis_forward(howLongInSeconds * 1000);
	}

#endif // ARDUINO_ARCH_SAMD

/**** end of CatenaSamd21_Sleep.cpp ****/
