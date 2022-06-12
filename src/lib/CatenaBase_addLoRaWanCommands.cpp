/*

Module:  CatenaBase_addLoRaWanCommands.cpp

Function:
	The command engine for lorawan commands on the Catena base platform

Copyright notice:
	See accompanying LICENSE file.

Author:
	Terry Moore, MCCI Corporation	July 2017
	ChaeHee Won, MCCI Corporation	December 2018

*/

#include "CatenaBase.h"

#include <Arduino_LoRaWAN_lmic.h>

#include "Catena_Log.h"

#include <cstring>

using namespace McciCatena;

/****************************************************************************\
|
|	The command table
|
\****************************************************************************/

static cCommandStream::CommandFn doConfigure;
static cCommandStream::CommandFn doJoin;

static const cCommandStream::cEntry sDispatchEntries[] =
	{
	{ "configure", doConfigure },
	{ "join", doJoin },
	};

static cCommandStream::cDispatch
sDispatch(sDispatchEntries, sizeof(sDispatchEntries), "lorawan");


struct KeyMap
	{
	const char *pName;
	cFramStorage::StandardKeys uKey;
	unsigned fieldOffset;
	unsigned fieldSize;
	};

static KeyMap sKeyMap[] =
	{
	{ "deveui", cFramStorage::StandardKeys::kDevEUI },
	{ "appeui", cFramStorage::StandardKeys::kAppEUI },
	{ "appkey", cFramStorage::StandardKeys::kAppKey, },
	{ "nwkskey", cFramStorage::StandardKeys::kNwkSKey, },
	{ "appskey", cFramStorage::StandardKeys::kAppSKey, },
	{ "devaddr", cFramStorage::StandardKeys::kDevAddr, },
	{ "netid", cFramStorage::StandardKeys::kNetID, },
	{ "fcntup", cFramStorage::StandardKeys::kLmicSessionState,
		offsetof(Arduino_LoRaWAN::SessionStateV1, FCntUp),
		sizeof(Arduino_LoRaWAN::SessionStateV1::FCntUp)
		},
	{ "fcntdown", cFramStorage::StandardKeys::kLmicSessionState,
		offsetof(Arduino_LoRaWAN::SessionStateV1, FCntDown),
		sizeof(Arduino_LoRaWAN::SessionStateV1::FCntDown)
		},
	{ "join", cFramStorage::StandardKeys::kJoin, },
	};

/*

Name:	CatenaBase::addLoRaWanCommands()

Function:
	Add the lorawan commands to the Catena command table.

Definition:
	private: bool CatenaBase::addLoRaWanCommands(void);

Description:
	All the commands are added to the system command table.

Returns:
	true for success.

*/

bool
CatenaBase::addLoRaWanCommands(
	void
	)
	{
	if (this->getFram() == nullptr)
		{
		gLog.printf(
			gLog.kError,
			"?CatenaBase::addLoRaWanCommands(): no FRAM\n"
			);
		return false;
		}

	gLog.printf(
		gLog.kTrace,
		"CatenaBase::addLoRaWanCommands(): adding\n"
		);
	this->addCommands(
		sDispatch,
		static_cast<void *>(this)
		);
	return true;
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
	CatenaBase * const pCatena = static_cast<CatenaBase *>(pContext);
	cFram::Cursor cursor(pCatena->getFram());
	static constexpr unsigned databuf_size = 256;

	auto printValue = [](cCommandStream *pThis, cFram::Cursor &cursor, const KeyMap &p) -> cCommandStream::CommandStatus
		{
		uint8_t databuf[databuf_size];
		if (! cursor.islocated())
			{
			pThis->printf("%s: not initialized\n", p.pName);
			return cCommandStream::CommandStatus::kNotInitialized;
			}
		else
			{
			char strbuf[64];
			size_t size;

			size = cursor.getitemsize();
			if (size > sizeof(databuf))
				size = sizeof(databuf);


			if (! cursor.get(databuf, size))
				{
				pThis->printf("%s: read error\n", p.pName);
				return cCommandStream::CommandStatus::kReadError;
				}

			if (cursor.getKey() == cFramStorage::StandardKeys::kLmicSessionState)
				{
				// it's a state variable
				cursor.formatfield(strbuf, sizeof(strbuf), 0,
					&databuf[p.fieldOffset],
					p.fieldSize,
					/* is number? */ true
					);
				}
			else
				{
				cursor.formatvalue(
					strbuf, sizeof(strbuf), 0,
					databuf, size
					);
				}

			pThis->printf("%s\n", strbuf);
			return cCommandStream::CommandStatus::kSuccess;
			}
		};

	if (argc < 2)
		{
		// TODO(tmm@mcci.com) display values
		for (auto const & p : sKeyMap)
			{
			cursor.locate(p.uKey);
			if (! cursor.isbound())
				continue;

			pThis->printf("lorawan %s %s ", argv[0], p.pName);
			(void) printValue(pThis, cursor, p);
			}

		return cCommandStream::CommandStatus::kSuccess;
		}

	const char * const pName = argv[1];
	const KeyMap *p = nullptr;

	for (auto const & Key : sKeyMap)
		{
		if (strcasecmp(Key.pName, pName) == 0)
			{
			// matched!
			cursor.locate(Key.uKey);
			p = &Key;
			}
		}

	if (! cursor.isbound())
		{
		pThis->printf(
			"%s: %s: unknown\n",
			__func__,
			pName
			);
		return cCommandStream::CommandStatus::kInvalidParameter;
		}

	// display
	if (argc <= 2)
		{
		return printValue(pThis, cursor, *p);
		}
	else
		{
		const char * const pValue = argv[2];
		uint8_t databuf[databuf_size];
		size_t size;
		bool fParseOk;

		/*
		|| fcntup and fcntdown are in a bigger field, so things
		|| get sophisticated.
		*/
		size = cursor.getitemsize();
		if (size > sizeof(databuf))
			size = sizeof(databuf);

		// parse the argument according to the cursor (which
		// specifies what will receive it
		if (cursor.getKey() == cFramStorage::StandardKeys::kLmicSessionState)
			{
			if (! cursor.get(databuf, size))
				{
				pThis->printf("%s: could not create entry\n",
					pName
					);

				return cCommandStream::CommandStatus::kCreateError;
				}
			fParseOk = cursor.parsefield(
					pValue,
					databuf + p->fieldOffset,
					p->fieldSize,
					true
					);
			}
		else
			fParseOk = cursor.parsevalue(
					pValue,
					databuf,
					size
					);
		if (! fParseOk)
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

/*

Name:	doJoin()

Function:
	Implement the LoRaWAN join command

Definition:
	static cCommandStream::CommandFn doJoin;

	static cCommandStream::CommandStatus
		doJoin(
			cCommandStream *pThis,
			void *pContext,
			int argc,
			char **argv
			);

Description:
	This function checks arguments, then forces an unjoin, followed by
	a join. If the device is not configured for OTAA mode, it
	does nothing. FRAM is not reset unless the join succeeds.
	A `lorawan configure reset` may be more along the lines
	of what you want, if you want to ship a device.

	The parsed syntax:

	lorawan join

Returns:
	Command status

*/

static cCommandStream::CommandStatus
doJoin(
	cCommandStream *pThis,
	void *pContext,
	int argc,
	char **argv
	)
	{

	if (argc > 1)
		return cCommandStream::CommandStatus::kInvalidParameter;

	LMIC_unjoin();
	LMIC_startJoining();
	return cCommandStream::CommandStatus::kSuccess;
	}

/**** end of CatenaBase_addLoRaWanCommands.cpp ****/
