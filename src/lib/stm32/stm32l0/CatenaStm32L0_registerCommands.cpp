/* CatenaStm32L0_registerCommands.cpp	Wed Dec 05 2018 14:34:20 chwon */

/*

Module:  CatenaStm32L0_registerCommands.cpp

Function:
	CatenaStm32L0::registerCommands()

Version:
	V0.12.0	Wed Dec 05 2018 14:34:21 chwon	Edit level 2

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

   0.12.0  Wed Dec 05 2018 14:34:21  chwon
	Move adding commands to CatenaBase class.

*/

#ifdef ARDUINO_ARCH_STM32

#include "CatenaStm32L0.h"

#include "Catena_CommandStream.h"
#include "Catena_Log.h"

using namespace McciCatena;

/****************************************************************************\
|
|	Manifest constants
|
\****************************************************************************/

static cCommandStream::CommandFn doCalibration;

static const cCommandStream::cEntry sStm32L0Entries[] =
	{
	{ "calibrate", doCalibration },
	};

static cCommandStream::cDispatch
sStm32L0Dispatch(sStm32L0Entries, sizeof(sStm32L0Entries), "system");

/****************************************************************************\
|
|	The command tables
|
\****************************************************************************/


/****************************************************************************\
|
|	The method function
|
\****************************************************************************/

/* protected virtual */
void
CatenaStm32L0::registerCommands()
	{
	this->Super::registerCommands();
	if (this->usesLseClock())
		{
		this->addCommands(
			sStm32L0Dispatch,
			static_cast<void *>(this)
			);
		}
	}

static cCommandStream::CommandStatus
doCalibration(
	cCommandStream *pThis,
	void *pContext,
	int argc,
	char **argv
	)
	{
	CatenaStm32L0 * const pCatena = static_cast<CatenaStm32L0 *>(pContext);

	if (argc <= 1)
		{
		uint32_t Calib;

		Calib = pCatena->CalibrateSystemClock();
		pThis->printf("calibration result: %u\n", Calib);

		return cCommandStream::CommandStatus::kSuccess;
		}
	else
		{
		return cCommandStream::CommandStatus::kInvalidParameter;
		}

	}

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32L0_registerCommands.cpp ****/
