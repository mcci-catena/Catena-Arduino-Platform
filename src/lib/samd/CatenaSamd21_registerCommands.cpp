/* CatenaSamd21_registerCommands.cpp	Sun Mar 19 2017 14:53:33 tmm */

/*

Module:  CatenaSamd21_registerCommands.cpp

Function:
	CatenaSamd21::registerCommands()

Version:
	V0.5.0	Sun Mar 19 2017 14:53:33 tmm	Edit level 1

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
   0.5.0  Sun Mar 19 2017 14:53:33  tmm
	Module created.

*/

#ifdef ARDUINO_ARCH_SAMD

#include "CatenaSamd21.h"

#include "Catena_CommandStream.h"

using namespace McciCatena;

/****************************************************************************\
|
|	Manifest constants
|
\****************************************************************************/

static cCommandStream::CommandFn doCpuid;

/****************************************************************************\
|
|	The command tables
|
\****************************************************************************/

static const cCommandStream::cEntry sDefaultEntries[] =
        {
        { "cpuid", doCpuid },
        };

static cCommandStream::cDispatch 
sDispatch(sDefaultEntries, sizeof(sDefaultEntries));

/****************************************************************************\
|
|	The method function
|
\****************************************************************************/

/* protected virtual */
void
CatenaSamd21::registerCommands()
	{
	this->Super::registerCommands();

	this->addCommands(sDispatch, (void *) this);
	}

/****************************************************************************\
|
|	The commands
|
\****************************************************************************/

static cCommandStream::CommandStatus
doCpuid(
        cCommandStream *pThis,
	void *pContext,
        int argc,
        char **argv
        )
        {
	CatenaSamd21 * const pCatena = static_cast<CatenaSamd21 *>(pContext);

	CatenaSamd21::UniqueID_string_t CpuIDstring;

	pThis->printf("%s\n", pCatena->GetUniqueIDstring(&CpuIDstring));
        return pThis->kSuccess;
        }

#endif // ARDUINO_ARCH_SAMD
