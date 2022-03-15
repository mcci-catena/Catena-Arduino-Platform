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
#include "Catena.h"
#include "Catena_Log.h"

#include <cstring>
#include <mcciadk_baselib.h>

using namespace McciCatena;

/****************************************************************************\
|
|	The command table
|
\****************************************************************************/

static cCommandStream::CommandFn doConfigure;
static cCommandStream::CommandFn doJoin;
static cCommandStream::CommandFn doStatus;
static cCommandStream::CommandFn doSubband;

static const cCommandStream::cEntry sDispatchEntries[] =
	{
	{ "configure", doConfigure },
	{ "join", doJoin },
	{ "status", doStatus },
	{ "subband", doSubband },
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
			"%s: unknown\n",
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

/*

Name:	doStatus()

Function:
	Implement the LoRaWAN status command

Definition:
	static cCommandStream::CommandFn doStatus;

	static cCommandStream::CommandStatus
		doStatus(
			cCommandStream *pThis,
			void *pContext,
			int argc,
			char **argv
			);

Description:
	This function checks arguments, then displays some things that
	are set by the MAC.

	The parsed syntax:

	lorawan status

Returns:
	Command status

*/

static cCommandStream::CommandStatus
doStatus(
	cCommandStream *pThis,
	void *pContext,
	int argc,
	char **argv
	)
	{

	if (argc > 1)
		return cCommandStream::CommandStatus::kInvalidParameter;

	do	{
		extern Catena::LoRaWAN gLoRaWAN;

		char sRegion[16];
		pThis->printf("Target network: %s / %s\n",
						gLoRaWAN.GetNetworkName(),
						gLoRaWAN.GetRegionString(sRegion, sizeof(sRegion))
						);
		} while (0);

	pThis->printf("Configured subband: %d\n", ARDUINO_LMIC_CFG_SUBBAND);

	pThis->printf("Tx Channel: %u\n", LMIC.txChnl);
	pThis->printf("Data rate: %d\n", LMIC.datarate);

#if CFG_LMIC_EU_like
	pThis->printf("EU-like configuration\n");
#elif CFG_LMIC_US_like
	pThis->printf("US-like channel mask: ");
	for (auto const & mask : LMIC.channelMap)
		{
		pThis->printf("%02x %02x ", mask & 0xFF, (mask >> 8) & 0xff);
		}
	pThis->printf("\n");

#endif
	pThis->printf("Uplink repeat: %d\n", LMIC.upRepeat);
	pThis->printf("Rx1 data rate offset: %d\n", LMIC.rx1DrOffset);
	pThis->printf("Rx2 data rate %u\n", LMIC.dn2Dr);
	pThis->printf("Rx1 window delay (secs): %u\n", LMIC.rxDelay);

	size_t nData = LMIC.dataBeg + LMIC.dataLen;
	char buffer[80];
	if (LMIC.dataBeg > 0)
		{
		pThis->printf("Rx Buffer MAC portion:\n");
		for (uint32_t i = 0; i < LMIC.dataBeg; i += 16)
			{
			uint32_t n = LMIC.dataBeg - i;
			if (n > 16)
				n = 16;
			McciAdkLib_FormatDumpLine(buffer, sizeof(buffer), 0, i, LMIC.frame + i, n);
			pThis->printf("%s\n", buffer);
			}
		}
	if (LMIC.dataLen > 0)
		{
		pThis->printf("Rx Buffer data:\n");
		for (uint32_t i = 0; i < LMIC.dataLen; i += 16)
			{
			uint32_t n = LMIC.dataLen - i;
			if (n > 16)
				n = 16;
			McciAdkLib_FormatDumpLine(
				buffer, sizeof(buffer), 0,
				i, LMIC.frame + LMIC.dataBeg + i, n
				);
			pThis->printf("%s\n", buffer);
			}
		}

	return cCommandStream::CommandStatus::kSuccess;
	}

/*

Name:	doSubband()

Function:
	Implement the LoRaWAN subband command

Definition:
	static cCommandStream::CommandFn doSubband;

	static cCommandStream::CommandStatus
		doSubband(
			cCommandStream *pThis,
			void *pContext,
			int argc,
			char **argv
			);

Description:
	This function checks arguments, then sets the subband.

	The parsed syntax:

	lorawan subband #

	If # is -1, then the subband is reset (so that all channels 
	are enabled); otherwise the subband must be a small number in
	[0..7], and is used to set the current subband mask.

Returns:
	Command status

*/

static cCommandStream::CommandStatus
doSubband(
	cCommandStream *pThis,
	void *pContext,
	int argc,
	char **argv
	)
	{
#if !CFG_LMIC_US_like
	pThis->printf("%s not supported in this region\n", argv[0]);
	return cCommandStream::CommandStatus::kError;
#else
	uint32_t uSubband;
	bool fError;
	int32_t sb;
	// TODO(tmm@mcci.com) get this from the LMIC, rather than using
	// the bitmap size.
	constexpr unsigned kNumSubbands = sizeof(LMIC.channelMap) * 8 - 16;

	if (argc != 2)
		return cCommandStream::CommandStatus::kInvalidParameter;

	auto const nc = McciAdkLib_BufferToUint32(argv[1], strlen(argv[1]), 0, &uSubband, &fError);
	if (fError || argv[1][nc] != '\0')
		return cCommandStream::CommandStatus::kInvalidParameter;

	if (int32_t(uSubband) == -1)
		{
		sb = -1;
		}
	else if (uSubband >= kNumSubbands)
		{
		return cCommandStream::CommandStatus::kInvalidParameter;
		}
	else
		{
		sb = int32_t(uSubband);
		}

	if (sb < 0)
		{
		// enable all subbands
		for (uint8_t i = 0; i < kNumSubbands; ++i)
			LMIC_enableSubBand(i);
		}
	else
		{
		LMIC_selectSubBand(sb);
		}

	return cCommandStream::CommandStatus::kSuccess;
#endif
	}

/**** end of CatenaBase_addLoRaWanCommands.cpp ****/
