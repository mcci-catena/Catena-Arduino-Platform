/* Catena_Si1133.cpp	Fri Oct 13 2017 15:19:30 chwon */

/*

Module:  Catena_Si1133.cpp

Function:
	Catena_Si1133 class

Version:
	V0.6.0	Fri Oct 13 2017 15:19:30 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2017 by

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

*/

#include <Catena_Si1133.h>
#include <Arduino.h>
using namespace McciCatena;

/****************************************************************************\
|
|		Manifest constants & typedefs.
|
|	This is strictly for private types and constants which will not
|	be exported.
|
\****************************************************************************/

/* Si1133 ADC config ADCMUX values */
#define	SI1133_ADCMUX_SMALL_IR		0x00u
#define	SI1133_ADCMUX_MEDIUM_IR		0x01u
#define	SI1133_ADCMUX_LARGE_IR		0x02u
#define	SI1133_ADCMUX_WHITE		0x0Bu
#define	SI1133_ADCMUX_LARGE_WHITE	0x0Du
#define	SI1133_ADCMUX_UV		0x18u
#define	SI1133_ADCMUX_UV_DEEP		0x19u


/****************************************************************************\
|
|	Read-only data.
|
|	If program is to be ROM-able, these must all be tagged read-only
|	using the ROM storage class; they may be global.
|
\****************************************************************************/

static const uint8_t	sk_Si1133AdcConfig[][4] =
	{
		{ SI1133_ADCMUX_SMALL_IR,    0x00, 0x00, 0x40 },
		{ SI1133_ADCMUX_MEDIUM_IR,   0x00, 0x08, 0x40 },
		{ SI1133_ADCMUX_LARGE_IR,    0x00, 0x10, 0x40 },
		{ SI1133_ADCMUX_WHITE,       0x00, 0x00, 0x80 },
		{ SI1133_ADCMUX_LARGE_WHITE, 0x00, 0x08, 0x80 },
		{ SI1133_ADCMUX_UV,          0x0b, 0x00, 0xC0 },
		{ SI1133_ADCMUX_UV_DEEP,     0x0b, 0x00, 0xC0 },
	};


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


Catena_Si1133::Catena_Si1133(void)
	{
	this->m_Initialized = false;
	this->m_ChannelEnabled = 0;
	}

boolean Catena_Si1133::begin(uint8_t DeviceAddress, TwoWire *pWire)
	{
	uint8_t	uPartId;
	uint8_t	uHwId;
	uint8_t uResponse;

	this->m_DeviceAddress = DeviceAddress;
	this->m_pWire = pWire;

	uPartId = this->readReg(SI1133_REG_PART_ID);
	uHwId = this->readReg(SI1133_REG_HW_ID);
	if (uPartId != 0x33 || (uHwId & 0x0F) != 0x03)
		{
		Serial.print("begin() error, PART_ID=");
		Serial.print(uPartId, HEX);
		Serial.print(" HW_ID=");
		Serial.println(uHwId, HEX);
		return false;
		}

	do	{
		this->writeReg(SI1133_REG_COMMAND, SI1133_CMD_RESET_SW);
		uResponse = this->getResponse();
		} while (uResponse & SI1133_RSP_CMD_ERR);

	this->writeParam(
		SI1133_PARAM_MEASRATE_H,
		CATENA_SI1133_MEASUREMENT_RATE >> 8
		);
	this->writeParam(
		SI1133_PARAM_MEASRATE_L,
		CATENA_SI1133_MEASUREMENT_RATE & 0xFF
		);

	this->writeParam(
		SI1133_PARAM_MEASCOUNT0,
		CATENA_SI1133_MEASUREMENT_COUNT
		);
	this->writeParam(
		SI1133_PARAM_MEASCOUNT1,
		CATENA_SI1133_MEASUREMENT_COUNT
		);
	this->writeParam(
		SI1133_PARAM_MEASCOUNT2,
		CATENA_SI1133_MEASUREMENT_COUNT
		);

	this->m_Initialized = true;
        return true;
	}

boolean Catena_Si1133::configure(
	uint8_t uChannel,
	uint8_t uMode,
	uint8_t uMeasurementCount
	)
	{
	uint8_t	uChannelBase;

	if (uChannel >= SI1133_NUM_CHANNEL || uMode > CATENA_SI1133_MODE_UVDeep)
		return false;

	if (this->m_ChannelEnabled & (1u << uChannel))
		return false;

	this->m_ChannelEnabled |= (1u << uChannel);

	uChannelBase = SI1133_REG_HOSTOUT0;
	for (uint32_t i = 0; i < SI1133_NUM_CHANNEL; ++i)
		{
		if (this->m_ChannelEnabled & (1u << i))
			{
			this->m_ChannelDataReg[i] = uChannelBase;
			uChannelBase += 2;
			}
		else
			{
			this->m_ChannelDataReg[i] = 0;
			}
		}

	uChannelBase = SI1133_PARAM_ADCCONFIG0 + (uChannel * 4);
	this->writeParam(uChannelBase,   sk_Si1133AdcConfig[uMode][0]);
	this->writeParam(uChannelBase+1, sk_Si1133AdcConfig[uMode][1]);
	this->writeParam(uChannelBase+2, sk_Si1133AdcConfig[uMode][2]);
	this->writeParam(uChannelBase+3, sk_Si1133AdcConfig[uMode][3]);

	if (uMeasurementCount != 0)
		{
		uChannelBase =	(SI1133_PARAM_MEASCOUNT0 - 1) +
				(sk_Si1133AdcConfig[uMode][3] >> 6);
		this->writeParam(uChannelBase, uMeasurementCount);
		}
        return true;
	}

boolean Catena_Si1133::start(boolean fOneTime)
	{
	uint8_t	uResponse;

	if (this->m_ChannelEnabled == 0)
		return false;

	this->writeParam(SI1133_PARAM_CHAN_LIST, this->m_ChannelEnabled);
	this->writeReg(
		SI1133_REG_COMMAND,
		fOneTime ? SI1133_CMD_FORCE : SI1133_CMD_START
		);
	uResponse = this->readReg(SI1133_REG_RESPONSE0);
	if (uResponse & SI1133_RSP_CMD_ERR)
		{
		Serial.print("start() error ");
		Serial.println(uResponse, HEX);
		this->writeReg(
			SI1133_REG_COMMAND,
			SI1133_CMD_RESET_CMD_CTR
			);
		return false;
		}

	this->m_fOneTime = fOneTime;
        return true;
	}

boolean Catena_Si1133::stop(void)
	{
	uint8_t	uResponse;

	if (this->m_ChannelEnabled == 0 || this->m_fOneTime)
		return true;

	this->writeReg(SI1133_REG_COMMAND, SI1133_CMD_PAUSE);
	uResponse = this->readReg(SI1133_REG_RESPONSE0);
	if (uResponse & SI1133_RSP_CMD_ERR)
		{
		Serial.print("stop() error ");
		Serial.println(uResponse, HEX);
		this->writeReg(
			SI1133_REG_COMMAND,
			SI1133_CMD_RESET_CMD_CTR
			);
		return false;
		}

        return true;
	}

uint16_t Catena_Si1133::readChannelData(uint8_t uChannel)
	{
	uint16_t uValue;

	if (uChannel >= SI1133_NUM_CHANNEL ||
	    (this->m_ChannelEnabled & (1u << uChannel)) == 0)
		return 0;

	this->m_pWire->beginTransmission(this->m_DeviceAddress);
	this->m_pWire->write(this->m_ChannelDataReg[uChannel]);
	this->m_pWire->endTransmission();
	this->m_pWire->requestFrom(this->m_DeviceAddress, 2u);

	while (! this->m_pWire->available())
		/* loop */;
	uValue = this->m_pWire->read() << 8;

	while (! this->m_pWire->available())
		/* loop */;
	uValue |= this->m_pWire->read();

	return uValue;
	}

void Catena_Si1133::readMultiChannelData(uint16_t *pChannelData, uint32_t nChannel)
	{
	uint8_t	nRead;

	if (this->m_ChannelEnabled == 0 || pChannelData == NULL || nChannel == 0)
		return;

	nRead = (uint8_t)(nChannel * 2u);
	this->m_pWire->beginTransmission(this->m_DeviceAddress);
	this->m_pWire->write(SI1133_REG_HOSTOUT0);
	this->m_pWire->endTransmission();
	this->m_pWire->requestFrom(this->m_DeviceAddress, nRead);

	for (uint32_t i = 0; i < nChannel; ++pChannelData, ++i)
		{
		while (! this->m_pWire->available())
			/* loop */;
		*pChannelData = this->m_pWire->read() << 8;

		while (! this->m_pWire->available())
			/* loop */;
		*pChannelData |= this->m_pWire->read();
		}
	}

uint8_t Catena_Si1133::readReg(uint8_t uReg)
	{
	this->m_pWire->beginTransmission(this->m_DeviceAddress);
	this->m_pWire->write(uReg);
	this->m_pWire->endTransmission();
	this->m_pWire->requestFrom(this->m_DeviceAddress, 1u);
	while (! this->m_pWire->available())
		/* loop */;
	return this->m_pWire->read();
	}

void Catena_Si1133::writeReg(uint8_t uReg, uint8_t uData)
	{
	this->m_pWire->beginTransmission(this->m_DeviceAddress);
	this->m_pWire->write(uReg);
	this->m_pWire->write(uData);
	this->m_pWire->endTransmission();
	}

uint8_t Catena_Si1133::getResponse(void)
	{
	uint8_t uResponse;

	uResponse = this->readReg(SI1133_REG_RESPONSE0);
	if (uResponse & SI1133_RSP_CMD_ERR)
		{
		Serial.print("getResponse() error ");
		Serial.println(uResponse, HEX);
		this->writeReg(SI1133_REG_COMMAND, SI1133_CMD_RESET_CMD_CTR);
		return this->getResponse();
		}
	return uResponse;
	}

uint8_t Catena_Si1133::waitResponse(uint8_t uResponse)
	{
	uint8_t uNewResponse;

	uResponse &= SI1133_RSP_CMD_CTR_MASK;

	do	{
		uNewResponse = this->readReg(SI1133_REG_RESPONSE0);
		if (uNewResponse & SI1133_RSP_CMD_ERR)
			{
			Serial.print("waitResponse() error ");
			Serial.println(uNewResponse, HEX);
			this->writeReg(
				SI1133_REG_COMMAND,
				SI1133_CMD_RESET_CMD_CTR
				);
			break;
			}
		} while ((uNewResponse & SI1133_RSP_CMD_CTR_MASK) == uResponse);

	return uNewResponse;
	}

uint8_t Catena_Si1133::readParam(uint8_t uParam)
	{
	uint8_t uResponse;
	
	uResponse = this->getResponse();

	this->writeReg(SI1133_REG_COMMAND, SI1133_CMD_PARAM_QUERY + uParam);
	
	uResponse = this->waitResponse(uResponse);
	if (uResponse & SI1133_RSP_CMD_ERR)
		{
		/* XXX: print error message */
		Serial.print("readParam() error ");
		Serial.println(uResponse, HEX);
		return 0;
		}

	return this->readReg(SI1133_REG_RESPONSE1);
	}

void Catena_Si1133::writeParam(uint8_t uParam, uint8_t uData)
	{
	uint8_t uResponse;
	
	uResponse = this->getResponse();

	this->m_pWire->beginTransmission(this->m_DeviceAddress);
	this->m_pWire->write(SI1133_REG_HOSTIN0);
	this->m_pWire->write(uData);	/* write to HOSTIN0 */
	this->m_pWire->write(SI1133_CMD_PARAM_SET + uParam);
					/* write PARAM_SET+uParam to COMMAND */
	this->m_pWire->endTransmission();

	uResponse = this->waitResponse(uResponse);
	if (uResponse & SI1133_RSP_CMD_ERR)
		{
		Serial.print("writeParam() error ");
		Serial.println(uResponse, HEX);
		/* XXX: print error message */
		}
	}

/**** end of Catena_Si1133.cpp ****/
