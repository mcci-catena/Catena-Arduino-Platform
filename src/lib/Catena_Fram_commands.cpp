/* Catena_Fram_commands.cpp	Wed Mar 22 2017 23:02:50 tmm */

/*

Module:  Catena_Fram_commands.cpp

Function:
	McciCatena::cFram::addCommands() and command processors.

Version:
	V0.5.0	Wed Mar 22 2017 23:02:50 tmm	Edit level 1

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
   0.5.0  Wed Mar 22 2017 23:02:50  tmm
	Module created.

*/

#include "Catena_Fram.h"

#include "CatenaBase.h"
#include <cstring>
#include "mcciadk_baselib.h"

using namespace McciCatena;

/****************************************************************************\
|
|	The command processors
|
\****************************************************************************/


/****************************************************************************\
|
|	The command table
|
\****************************************************************************/

static const cCommandStream::cEntry sDefaultEntries[] =
	{
	{ "dump", cFram::doDump },
        { "reset", cFram::doReset },
	};

static cCommandStream::cDispatch
sDispatch(sDefaultEntries, sizeof(sDefaultEntries), "fram");

/*

Name:	McciCatena::cFram::addCommands()

Function:
	Add the FRAM commands to the Catena command table.

Definition:
	bool McciCatena::cFram::addCommands();

Description:
	All the commands are added to the system command table.

Returns:
	true for success.

*/

bool 
McciCatena::cFram::addCommands()
	{
	CatenaBase::pCatenaBase->addCommands(
		sDispatch, static_cast<void *>(this)
		);
	}

/****************************************************************************\
|
|	The command handlers
|
\****************************************************************************/

static cCommandStream::CommandStatus
getuint32(
	int argc,
	char **argv,
	int iArg,
	uint32_t& result,
	uint32_t uDefault
	)
	{
	bool fOverflow;

	// substitute default if needed
	if (iArg >= argc)
		{
		result = uDefault;
		return cCommandStream::CommandStatus::kSuccess;
		}

	const char * const pArg = argv[iArg];
	size_t nArg = std::strlen(pArg);

	size_t const nc = McciAdkLib_BufferToUint32(
				pArg,
				nArg,
				16,
				&result,
				&fOverflow
				);

	if (nc == 0 || nc != nArg || fOverflow)
		return cCommandStream::CommandStatus::kError;
	else
		return cCommandStream::CommandStatus::kSuccess;
	}

cCommandStream::CommandStatus 
McciCatena::cFram::doDump(
	cCommandStream *pThis,
	void *pContext,
	int argc, 
	char **argv
	)
	{
	cFram * const pFram = static_cast<cFram *>(pContext);
	uint32_t uLength;
	uint32_t uBase;
	cCommandStream::CommandStatus status;

	// get arg 2 as length; default is 32 bytes
	status = getuint32(argc, argv, 2, uLength, 32);

	if (status != cCommandStream::CommandStatus::kSuccess)
		return status;

	// get arg 1 as base; default is 0
	status = getuint32(argc, argv, 1, uBase, 0);

	if (status != cCommandStream::CommandStatus::kSuccess)
		return status;

	// dump the FRAM
	uint8_t buffer[16];
	for (uint32_t here = 0; here < uLength; here += sizeof(buffer))
		{
		char line[80];
		size_t n;

		n = uLength - here;
		if (n > sizeof(buffer))
			n = sizeof(buffer);

		std::memset(buffer, 0, n);
		pFram->read(uBase + here, buffer, n);

		McciAdkLib_FormatDumpLine(line, sizeof(line), 0, uBase + here, buffer, n);
		pThis->printf("%s\n", line);
		}

	return status;
	}

cCommandStream::CommandStatus 
McciCatena::cFram::doReset(
	cCommandStream *pThis,
	void *pContext,
	int argc, 
	char **argv
	)
	{
	cFram * const pFram = static_cast<cFram *>(pContext);
        cCommandStream::CommandStatus status;
        bool fResult;

        if (argc > 2)
                return cCommandStream::CommandStatus::kInvalidParameter;
        if (argc == 2 && strcmp(argv[1], "hard") != 0)
                return cCommandStream::CommandStatus::kInvalidParameter;

        if (argc == 2)
                fResult = pFram->reset();
        else
                fResult = pFram->initialize();

        return fResult ? cCommandStream::CommandStatus::kSuccess
                       : cCommandStream::CommandStatus::kError
                       ;
        }