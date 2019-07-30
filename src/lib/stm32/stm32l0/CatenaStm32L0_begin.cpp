/* CatenaStm32L0_begin.cpp	Wed Dec 05 2018 14:27:07 chwon */

/*

Module:  CatenaStm32L0_begin.cpp

Function:
	Home for CatenaStm32L0::begin()

Version:
	V0.12.0	Wed Dec 05 2018 14:27:07 chwon	Edit level 2

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
   0.11.0  Wed Oct 31 2018 10:57:42  chwon
	Module created.

   0.12.0  Wed Dec 05 2018 14:27:07  chwon
	Move common initialization code to CatenaBase class.

*/

#ifdef ARDUINO_ARCH_STM32

#include "CatenaStm32L0.h"
#include "Catena_Log.h"

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

Name:	CatenaStm32L0::begin()

Function:
	Start up the framework on Stm32L0-based Catenas.

Definition:
	bool CatenaStm32L0::begin(void);

Description:
	These three forms of the STM32L0 startup code all set the operating
	flags according to the masks, and initialize the Stm32L0-specific
	fields of the Catena object according to platform data.

	The super-class begin() is called before ours is called.

Returns:
	true if initialied correctly, false otherwise.

*/

bool CatenaStm32L0::begin()
	{
	gLog.printf(gLog.kTrace, "+CatenaStm32L0::begin()\n");

	if (! this->m_Rtc.begin())
		{
		// error message already printed
		return false;
		}

	// we must initialize the FRAM before we call our parent,
	// because FRAM is used for stable storage of platform info.

	// start the FRAM
	if (!this->m_Fram.begin())
		{
		gLog.printf(
			gLog.kError,
			"CatenaStm32L0::begin: FRAM begin() failed\n"
			);
		return false;
		}

	// check whether the FRAM is valid
	if (! this->m_Fram.isValid())
		{
		gLog.printf(
			gLog.kError,
			"CatenaStm32L0::begin: FRAM contents are not valid, resetting\n"
			);
		this->m_Fram.initialize();
		}

	if (! this->Super::begin())
		{
		gLog.printf(gLog.kError, "?CatenaStm32L0::Super::begin failed\n");
		return false;
		}

	// register all commands in this stack
	this->registerCommands();

	// and register the FRAM commands
	if (!this->m_Fram.addCommands())
		{
		gLog.printf(
			gLog.kError,
			"?CatenaStm32L0::begin: addCommands() failed\n"
			);
		return false;
		}

	this->m_Fram.getField(
		cFramStorage::StandardKeys::kBootCount,
		this->m_BootCount
		);
	++this->m_BootCount;
	this->m_Fram.saveField(
		cFramStorage::StandardKeys::kBootCount,
		this->m_BootCount
		);

	/* don't calibarate unless the BSP is using LSE clock */
	// Calibrate system clock
	if (this->usesLseClock())
		this->CalibrateSystemClock();

	return true;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32L0_begin.cpp ****/
