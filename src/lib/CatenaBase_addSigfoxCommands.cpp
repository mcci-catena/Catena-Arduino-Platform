/*

Module:  CatenaBase_addSigfoxCommands.cpp

Function:
	The command engine for sigfox commands on the Catena base platform

Copyright notice:
	See accompanying LICENSE file.

Author:
	Dhinesh Kumar Pitchai, MCCI Corporation	December 2020

*/

#include "CatenaBase.h"

#include <Catena_Sigfox_wapper.h>

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
sDispatch(sDispatchEntries, sizeof(sDispatchEntries), "sigfox");


struct KeyMap
	{
	const char *pName;
	cFramStorage::StandardKeys uKey;
	};

static KeyMap sKeyMap[] =
	{
	{ "devid", cFramStorage::StandardKeys::kDevID },
	{ "pac", cFramStorage::StandardKeys::kPAC },
	{ "key", cFramStorage::StandardKeys::kKey, },
	{ "region", cFramStorage::StandardKeys::kRegion, },
	{ "encryption", cFramStorage::StandardKeys::kEncryption, },
	};

/*

Name:	CatenaBase::addSigfoxCommands()

Function:
	Add the sigfox commands to the Catena command table.

Definition:
	private: bool CatenaBase::addSigfoxCommands(void);

Description:
	All the commands are added to the system command table.

Returns:
	true for success.

*/

bool
CatenaBase::addSigfoxCommands(
	void
	)
	{
	if (this->getFram() == nullptr)
		{
		gLog.printf(
			gLog.kError,
			"?CatenaBase::addSigfoxCommands(): no FRAM\n"
			);
		return false;
		}

	gLog.printf(
		gLog.kTrace,
		"CatenaBase::addSigfoxCommands(): adding\n"
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
	Implement the Sigfox value set/get commands

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

	sigfox configure [ {param} [ {value} ] ]

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
	static constexpr unsigned databuf_size = 16;

	auto printValue = [](cCommandStream *pThis, cFram::Cursor &cursor, const char *pName) -> cCommandStream::CommandStatus 
		{
		uint8_t databuf[databuf_size];
		if (! cursor.islocated())
			{
			pThis->printf("%s: not initialized\n", pName);
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
		};

	if (argc < 2)
		{
		// TODO(tmm@mcci.com) display values
		for (auto const & p : sKeyMap)
			{
			cursor.locate(p.uKey);
			if (! cursor.isbound())
				continue;

			pThis->printf("sigfox %s %s ", argv[0], p.pName);
			(void) printValue(pThis, cursor, p.pName);
			}

		return cCommandStream::CommandStatus::kSuccess;
		}

	const char * const pName = argv[1];

	for (auto const & p : sKeyMap)
		{
		if (strcasecmp(p.pName, pName) == 0)
			{
			// matched!
			cursor.locate(p.uKey);
			}
		}

	if (! cursor.isbound())
		{
		pThis->printf(
			"%s: unknown\n",
			__func__,
			pName
			);
		return cCommandStream::CommandStatus::kInvalidParameter;
		}

	// display
	if (argc <= 2)
		{
		return printValue(pThis, cursor, pName);
		}
	else
		{
		const char * const pValue = argv[2];
		uint8_t databuf[databuf_size];
		size_t size;

		size = cursor.getitemsize();
		if (size > sizeof(databuf))
			size = sizeof(databuf);

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

/**** end of CatenaBase_addSigfoxCommands.cpp ****/
