/* Catena4450_LoRaWAN_commands.cpp	Sun Mar 26 2017 12:01:36 tmm */

/*

Module:  Catena4450_LoRaWAN_commands.cpp

Function:
	The command engine for lorawan commands on the Catena 4450.

Version:
	V0.5.0	Sun Mar 26 2017 12:01:36 tmm	Edit level 1

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
   0.5.0  Sun Mar 26 2017 12:01:36  tmm
	Module created.

*/

#include "Catena4450.h"

#include "Catena_Log.h"

#include <cstring>

using namespace McciCatena;

/****************************************************************************\
|
|	The command table
|
\****************************************************************************/

static cCommandStream::CommandFn doConfigure;

static const cCommandStream::cEntry sDispatchEntries[] =
	{
	{ "configure", doConfigure },
	};

static cCommandStream::cDispatch
sDispatch(sDispatchEntries, sizeof(sDispatchEntries), "lorawan");


struct KeyMap
	{
	const char *pName;
	cFramStorage::StandardKeys uKey;
	};

static KeyMap sKeyMap[] =
	{
	{ "deveui", cFramStorage::StandardKeys::kDevEUI },
	{ "appeui", cFramStorage::StandardKeys::kAppEUI },
	{ "appkey", cFramStorage::StandardKeys::kAppKey, },
	{ "nwkskey", cFramStorage::StandardKeys::kNwkSKey, },
	{ "appskey", cFramStorage::StandardKeys::kAppSKey, },
	{ "devaddr", cFramStorage::StandardKeys::kDevAddr, },
	{ "nwkid", cFramStorage::StandardKeys::kNwkID, },
	{ "fcntup", cFramStorage::StandardKeys::kFCntUp, },
	{ "fcntdown", cFramStorage::StandardKeys::kFCntDown, },
	{ "join", cFramStorage::StandardKeys::kJoin, },
	};

/*

Name:	Catena4450::LoRaWAN::addCommands()

Function:
	Add the lorawan commands to the Catena command table.

Definition:
	private: bool Catena4450::LoRaWAN::addCommands();

Description:
	All the commands are added to the system command table.

Returns:
	true for success.

*/

bool 
Catena4450::LoRaWAN::addCommands()
	{
	gLog.printf(gLog.kAlways, "Catena4450::LoRaWAN::addCommands(): adding\n");
	this->m_pCatena->addCommands(
		sDispatch, static_cast<void *>(this)
		);
	}

/*

Name:	doConfigure()

Function:
	Implement the LoRaWAN value set/get commands

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
	Catena4450::LoRaWAN * const pLoRaWAN = 
		static_cast<Catena4450::LoRaWAN *>(pContext);
	Catena4450 * const pCatena = pLoRaWAN->getCatena();
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
			char strbuf[32];

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
