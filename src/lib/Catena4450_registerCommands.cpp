/* Catena4450_registerCommands.cpp	Sun Mar 19 2017 14:53:33 tmm */

/*

Module:  Catena4450_registerCommands.cpp

Function:
	Catena4450::registerCommands()

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

#include "Catena4450.h"

#include "Catena_CommandStream.h"

using namespace McciCatena;

/****************************************************************************\
|
|	Manifest constants
|
\****************************************************************************/

static cCommandStream::CommandFn doPlatformGUID;
static cCommandStream::CommandFn doSysEUI;

/****************************************************************************\
|
|	The command tables
|
\****************************************************************************/

static cCommandStream::CommandFn doConfigure;

static const cCommandStream::cEntry sDispatchEntries[] =
	{
	{ "configure", doConfigure },
	};

static cCommandStream::cDispatch
sDispatch(sDispatchEntries, sizeof(sDispatchEntries), "system");

struct KeyMap
	{
	const char *pName;
	cFramStorage::StandardKeys uKey;
	};

static KeyMap sKeyMap[] =
	{
	{ "PlatformGUID", cFramStorage::StandardKeys::kPlatformGuid },
	{ "SysEUI", cFramStorage::StandardKeys::kSysEUI },
        { "OperatingFlags", cFramStorage::StandardKeys::kOperatingFlags },
        };


/****************************************************************************\
|
|	The method function
|
\****************************************************************************/

/* protected virtual */
void
Catena4450::registerCommands()
	{
	this->Super::registerCommands();

	this->addCommands(sDispatch, (void *) this);
	}

/****************************************************************************\
|
|	The commands
|
\****************************************************************************/


/*

Name:	doConfigure()

Function:
	Implement the system value set/get commands

Definition:
	static cCommandStream::CommandFn doConfigure;

	static cCommandStream::CommandStatus
		doConfigure(
			cCommandStream *pThis,
			void *pContext,
			int argc,
			char **argv
			);

Description:
	This function dispatches the various commands, parsing the input
	parameters if any to set the corresponding value, or displaying
	the values.

	The parsed syntax:

	lorawan configure [ {param} [ {value} ] ]

Returns:
	Command status

*/

static cCommandStream::CommandStatus
doConfigure(
	cCommandStream *pThis,
	void *pContext,
	int argc,
	char **argv
	)
	{
	Catena4450 * const pCatena = static_cast<Catena4450 *>(pContext);
	uint8_t databuf[16];

	if (argc < 2)
		{
		// TODO(tmm@mcci.com) display values
		return cCommandStream::CommandStatus::kInvalidParameter;
		}

	const char * const pName = argv[1];
	cFram::Cursor cursor(pCatena->getFram());

	for (auto const & p : sKeyMap)
		{
		if (stricmp(p.pName, pName) == 0)
			{
			// matched!
			cursor.locate(p.uKey);
			}
		}
	
	if (! cursor.isbound())
		{
		pThis->printf(
			"%s: unknown",
			__func__,
			pName
			);
		return cCommandStream::CommandStatus::kInvalidParameter;
		}

	size_t size;

	size = cursor.getitemsize();
	if (size > sizeof(databuf))
		size = sizeof(databuf);

	// display
	if (argc <= 2)
		{
		if (! cursor.islocated())
			{
			pThis->printf("%s: not initialized\n", pName);
			return cCommandStream::CommandStatus::kNotInitialized;
			}
		else
			{
			char strbuf[64];

			if (! cursor.get(databuf, size))
				{
				pThis->printf("%s: read error\n", pName);
				return cCommandStream::CommandStatus::kReadError;
				}

			cursor.formatvalue(
				strbuf, sizeof(strbuf), 0,
				databuf, size
				);

			pThis->printf("%s\n", strbuf);
			return cCommandStream::CommandStatus::kSuccess;
			}
		}
	else
		{
		const char * const pValue = argv[2];

		// parse the argument according to the cursor (which
		// specifies what will receive it
		if (! cursor.parsevalue(
				pValue,
				databuf,
				size
				))
			{
			pThis->printf("%s: invalid parameter: %s\n",
				pName, pValue
				);
			return cCommandStream::CommandStatus::kInvalidParameter;
			}
		else if (! cursor.create())
			{
			pThis->printf("%s: could not create entry\n",
				pName
				);
			return cCommandStream::CommandStatus::kCreateError;
			}
		else
			{
			return cursor.put(databuf, size)
				? cCommandStream::CommandStatus::kSuccess
				: cCommandStream::CommandStatus::kWriteError
				;
			}
		}
	}
