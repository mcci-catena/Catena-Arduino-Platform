/*

Module:  CatenaBase_registerCommands.cpp

Function:
	CatenaBase::registerCommands()

Copyright notice:
	See accompanying LICENSE file.

Author:
	Terry Moore, MCCI Corporation	2016
	ChaeHee Won, MCCI Corporation	December 2018

*/

#include "CatenaBase.h"

#include "Catena_CommandStream.h"
#include <mcciadk_env.h>
#include <Arduino_LoRaWAN.h>
#include <Arduino_LoRaWAN_lmic.h>

using namespace McciCatena;

/****************************************************************************\
|
|	Manifest constants and forward references
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
static cCommandStream::CommandFn doEcho;
static cCommandStream::CommandFn doReset;
static cCommandStream::CommandFn doVersion;

static const cCommandStream::cEntry sDispatchEntries[] =
	{
	{ "configure", doConfigure },
	{ "echo", doEcho },
	{ "reset", doReset },
	{ "version", doVersion },
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
|	The method function that registers these commands
|
\****************************************************************************/

/* protected virtual */
void
CatenaBase::registerCommands()
	{
	this->addCommands(sDispatch, (void *) this);
	}

/****************************************************************************\
|
|	The commands (called back from parser)
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

	system configure [ {param} [ {value} ] ]

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

/*

Name:	doReset()

Function:
	Implement the system reset command

Definition:
	static cCommandStream::CommandFn doReset;

	static cCommandStream::CommandStatus
		doReset(
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

.		system reset

Returns:
	Command status (always an error if it returns; otherwise the
	system gets rebooted).

*/


static cCommandStream::CommandStatus
doReset(
	cCommandStream *pThis,
	void *pContext,
	int argc,
	char **argv
	)
	{
	if (argc != 1)
		{
		return cCommandStream::CommandStatus::kInvalidParameter;
		}

	NVIC_SystemReset();
	}

/*

Name:	doVersion()

Function:
	Implement the system reset command

Definition:
	static cCommandStream::CommandFn doVersion;

	static cCommandStream::CommandStatus
		doVersion(
			cCommandStream *pThis,
			void *pContext,
			int argc,
			char **argv
			);

Description:
	This function dispatches the command, parsing the input
	parameters if any to set the corresponding value, or displaying
	the values.

	The parsed syntax:

.		system version

	The output:

		Board: name
		Platform-version: X.Y.Z[.local]
		Arduino-LoRaWAN-version: X.Y.Z[.local]
		Arduino-LMIC-version: X.Y.Z[.local]
		MCCIADK-version: X.Y.Z[.local]

Returns:
	Command status.

*/

/* common code to display a version */
static void
printVersion(
	cCommandStream *pThis,
	const char *pLabel,
	uint32_t packedVersion,
	bool fSemantic = false
	)
	{
	const char cSemantic = fSemantic ? '-' : '.';

	pThis->printf("%s-Version: %d.%d.%d%c",
		pLabel,
		(packedVersion >> 24) & 0xFF,
		(packedVersion >> 16) & 0xFF,
		(packedVersion >> 8) & 0xFF,
		(packedVersion & 0xFF) ? cSemantic : '\n'
		);

	if (packedVersion & 0xFF)
		pThis->printf("%d\n", packedVersion & 0xFF);
	}

/* Older versions of MCCI ADK don't have a version. Accomodate that. */
#ifdef MCCIADK_VERSION
static constexpr uint32_t mcciadk_version = MCCIADK_VERSION;
#else
static constexpr uint32_t mcciadk_version = 0;
#endif

static cCommandStream::CommandStatus
doVersion(
	cCommandStream *pThis,
	void *pContext,
	int argc,
	char **argv
	)
	{
	CatenaBase * const pCatena = static_cast<CatenaBase *>(pContext);

	if (argc != 1)
		{
		return cCommandStream::CommandStatus::kInvalidParameter;
		}

	pThis->printf("Board: %s\n", pCatena->CatenaName());
	printVersion(pThis, "Platform", CATENA_ARDUINO_PLATFORM_VERSION, true);
	printVersion(pThis, "Arduino-LoRaWAN", ARDUINO_LORAWAN_VERSION, true);
	printVersion(pThis, "Arduino-LMIC", ARDUINO_LMIC_VERSION);
	printVersion(pThis, "MCCIADK", mcciadk_version);

	/* include the MCCI Arduino version, if known */
	if (pCatena->kMcciArduinoVersion != 0)
		printVersion(pThis, "MCCI-Arduino-BSP", pCatena->kMcciArduinoVersion,
			pCatena->kMcciArduinoVersionIsSemantic
			);

	return cCommandStream::CommandStatus::kSuccess;
	}

static const char sOn[] = "on";
static const char sOff[] = "off";

static cCommandStream::CommandStatus
doEcho(
	cCommandStream *pThis,
	void *pContext,
	int argc,
	char **argv
	)
	{
	CatenaBase * const pCatena = static_cast<CatenaBase *>(pContext);
	auto result = cCommandStream::CommandStatus::kSuccess;

	if (argc > 2)
		{
		result = cCommandStream::CommandStatus::kInvalidParameter;
		}
	else if (argc <= 1)
		{
		pThis->printf("system echo %s\n", pThis->getEcho() ? sOn : sOff);
		}
	else /* argc == 2 */
		{
		if (strcmp(argv[1], sOn) == 0)
			pThis->setEcho(true);
		else if (strcmp(argv[1], sOff) == 0)
			pThis->setEcho(false);
		else
			result = cCommandStream::CommandStatus::kInvalidParameter;
		}

	return result;
	}

/**** end of CatenaBase_registerCommands.cpp ****/
