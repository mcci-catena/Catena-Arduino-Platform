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

// this table has one row per mode, and 4 columns:
//		  ADCCONFIGx	ADCSENSx	ADCPOSTx	MEASCONGIGx
static const Catena_Si1133::ChannelConfiguration_t sk_Config[] =
	{
	Catena_Si1133::ChannelConfiguration_t()
		.setAdcMux(Catena_Si1133::InputLed_t::SmallIR)
		.setCounter(Catena_Si1133::ChannelConfiguration_t::CounterSelect_t::MeasCount1),
	Catena_Si1133::ChannelConfiguration_t()
		.setAdcMux(Catena_Si1133::InputLed_t::MediumIR)
		.setPostShift(1)
		.setCounter(Catena_Si1133::ChannelConfiguration_t::CounterSelect_t::MeasCount1),
	Catena_Si1133::ChannelConfiguration_t()
		.setAdcMux(Catena_Si1133::InputLed_t::LargeIR)
		.setPostShift(2)
		.setCounter(Catena_Si1133::ChannelConfiguration_t::CounterSelect_t::MeasCount1),
	Catena_Si1133::ChannelConfiguration_t()
		.setAdcMux(Catena_Si1133::InputLed_t::White)
		.setPostShift(0)
		.setCounter(Catena_Si1133::ChannelConfiguration_t::CounterSelect_t::MeasCount2),
	Catena_Si1133::ChannelConfiguration_t()
		.setAdcMux(Catena_Si1133::InputLed_t::LargeWhite)
		.setSwGainCode(7)
		.setHwGainCode(4)
		.setPostShift(1)
		.set24bit(1)
		.setCounter(Catena_Si1133::ChannelConfiguration_t::CounterSelect_t::MeasCount2),
	Catena_Si1133::ChannelConfiguration_t()
		.setAdcMux(Catena_Si1133::InputLed_t::UV)
		.setSwGainCode(0)
		.setHwGainCode(9)
		.setPostShift(0)
		.setCounter(Catena_Si1133::ChannelConfiguration_t::CounterSelect_t::MeasCount3),
	Catena_Si1133::ChannelConfiguration_t()
		.setAdcMux(Catena_Si1133::InputLed_t::UVdeep)
		.setSwGainCode(0)
		.setHwGainCode(11)
		.setPostShift(0)
		.setCounter(Catena_Si1133::ChannelConfiguration_t::CounterSelect_t::MeasCount3),
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

	// per sample code, we need a 10ms delay
	delay(10);

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

bool  Catena_Si1133::configure(
	uint8_t	uChannel,
	Catena_Si1133::ChannelConfiguration_t config,
	uint8_t uMeasurementCount
	)
	{
	if (uChannel >= SI1133_NUM_CHANNEL)
		return false;

	if (! isValid(config.getAdcMux()))
		return false;

	// if measurement count is non-zero, config must select a counter.
	if (uMeasurementCount != 0 && config.getCounter() == ChannelConfiguration_t::CounterSelect_t::None)
		return false;

	// if measurement count is zero, set counter to none
	if (uMeasurementCount == 0)
		config.setCounter(ChannelConfiguration_t::CounterSelect_t::None);

	// store the configuration and invalidate the data offsets
	this->m_config[uChannel] = config;
	this->m_fDataRegValid = false;

	// allow disabling as well as enabling channels
	if (config.getAdcMux() == InputLed_t::Disabled)
		{
		this->m_ChannelEnabled &= ~(1u << uChannel);
		return true;
		}
	else
		{
		uint8_t uChannelBase;
		uint32_t value = config.getValue();

		this->m_ChannelEnabled |= (1u << uChannel);

		uChannelBase = SI1133_PARAM_ADCCONFIG0 + (uChannel * 4);
		this->writeParam(uChannelBase,   uint8_t((value >> 0) & 0xFFu));
		this->writeParam(uChannelBase+1, uint8_t((value >> 8) & 0xFFu));
		this->writeParam(uChannelBase+2, uint8_t((value >> 16) & 0xFFu));
		this->writeParam(uChannelBase+3, uint8_t((value >> 24) & 0xFFu));

		if (uMeasurementCount != 0)
			{
			uChannelBase =	(SI1133_PARAM_MEASCOUNT0 - 1) +
					uint8_t(config.getCounter());
			this->writeParam(uChannelBase, uMeasurementCount);
			}
		return true;
		}
	}

// configure the channel (legacy). if uMeasurementCount is zero, we set up for forced mode,
// otherwise configure autonomous mode.
boolean Catena_Si1133::configure(
	uint8_t uChannel,
	uint8_t uMode,
	uint8_t uMeasurementCount
	)
	{
	uint8_t	uChannelBase;

	if (uMode > sizeof(sk_Config) / sizeof(sk_Config[0]))
		return false;

	return this->configure(uChannel, sk_Config[uMode], uMeasurementCount);
	}

boolean Catena_Si1133::start(boolean fOneTime)
	{
	uint8_t	uResponse;

	if (this->m_ChannelEnabled == 0)
		return false;

	if (! this->m_fDataRegValid)
		{
		uint8_t	uChannelBase;

		uChannelBase = SI1133_REG_HOSTOUT0;
		for (uint32_t i = 0; i < SI1133_NUM_CHANNEL; ++i)
			{
			if (this->m_ChannelEnabled & (1u << i))
				{
				this->m_ChannelDataReg[i] = uChannelBase;
				uChannelBase += 2 + this->m_config[i].get24bit();	// assumes 16-bit mode. add 3 for 24-bit mode.
				}
			else
				{
				this->m_ChannelDataReg[i] = 0;
				}
			}

		this->m_fDataRegValid = true;
		}

	this->writeParam(SI1133_PARAM_CHAN_LIST, this->m_ChannelEnabled);
	if (fOneTime)
		{
		// read and discard any interrupts
		(void) this->readReg(SI1133_REG_IRQ_STATUS);
		// no channels completed
		this->m_ChannelCompleted = 0;
		// interupts enabled from those channels
		this->writeReg(SI1133_REG_IRQ_ENABLE, this->m_ChannelEnabled);
		// do a forced acquisition
		this->writeReg(SI1133_REG_COMMAND, SI1133_CMD_FORCE);
		}
	else
		{
		// start asynchronous operation.
		this->writeReg(
			SI1133_REG_COMMAND, SI1133_CMD_START
			);
		}

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

	this->m_StartTime = this->m_LastPollTime = millis();
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

uint32_t Catena_Si1133::readChannelData(uint8_t uChannel)
	{
	uint32_t uValue;

	if (uChannel >= SI1133_NUM_CHANNEL ||
	    (this->m_ChannelEnabled & (1u << uChannel)) == 0)
		return 0;

	bool const fIs24bit = this->m_config[uChannel].get24bit();

	this->m_pWire->beginTransmission(this->m_DeviceAddress);
	if (this->m_pWire->write(this->m_ChannelDataReg[uChannel]) != 1)
		return 0;
	if (this->m_pWire->endTransmission() != 0)
		return 0;
	if (this->m_pWire->requestFrom(this->m_DeviceAddress, 2u + fIs24bit) != 2u + fIs24bit)
		return 0;

	uValue = 0;

	if (fIs24bit)
		{
		uValue = this->m_pWire->read() << 16;
		}

	uValue = this->m_pWire->read() << 8;

	uValue |= this->m_pWire->read();

	return uValue;
	}

bool Catena_Si1133::readMultiChannelData(uint32_t *pChannelData, uint32_t nChannel) const
	{
	uint8_t	nRead;

	if (this->m_ChannelEnabled == 0 || pChannelData == NULL || nChannel == 0 || nChannel > SI1133_NUM_CHANNEL)
		return false;

	/* count number of 24-bit channels */
	nRead = (uint8_t)(nChannel * 2u);
	for (uint32_t i = 0; i < nChannel; ++i)
		{
		if (! (this->m_ChannelEnabled & (1 << i)))
			continue;

		nRead += this->m_config[i].get24bit();
		}

	this->m_pWire->beginTransmission(this->m_DeviceAddress);
	if (this->m_pWire->write(SI1133_REG_HOSTOUT0) == 0)
		return false;
	if (this->m_pWire->endTransmission() != 0)
		return false;
	if (this->m_pWire->requestFrom(this->m_DeviceAddress, nRead) != nRead)
		return false;

	for (uint32_t i = 0; i < nChannel; ++pChannelData, ++i)
		{
		// we can't have "negative light" so we treat
		// the 24-bit and 16-bit numbers as unsigned.
		uint32_t channelData = 0;

		if (this->m_config[i].get24bit())
			{
			channelData = this->m_pWire->read() << 16;
			}

		channelData |= this->m_pWire->read() << 8;
		channelData |= this->m_pWire->read();
		*pChannelData = channelData;
		}
	return true;
	}

bool Catena_Si1133::readMultiChannelData(uint16_t *pChannelData, uint32_t nChannel) const
	{
	// it's much easier to just allocate an extra array and do a regular read
	uint32_t tempChannel[SI1133_NUM_CHANNEL];
	bool result;

	if (nChannel > SI1133_NUM_CHANNEL)
		nChannel = SI1133_NUM_CHANNEL;

	result = this->readMultiChannelData(tempChannel, nChannel);

	if (! result)
		{
		memset(tempChannel, 0, sizeof(tempChannel));
		}

	for (uint32_t i = 0; i < nChannel; ++i)
		pChannelData[i] = uint16_t(tempChannel[i]);

	return result;
	}

bool Catena_Si1133::isOneTimeReady()
	{
	if (this->m_fOneTime)
		{
		std::uint32_t now = millis();

		if ((now - this->m_LastPollTime) < kPollDelayMs ||
		    (now - this->m_StartTime) < kStartDelayMs)
		    	{
			// not enough time has passed
			return false;
			}

		this->m_ChannelCompleted |= this->readReg(SI1133_REG_IRQ_STATUS);
		this->m_LastPollTime = now;
		return (this->m_ChannelCompleted & this->m_ChannelEnabled) == this->m_ChannelEnabled;
		}
	else
		return false;
	}

uint8_t Catena_Si1133::readReg(uint8_t uReg)
	{
	this->m_pWire->beginTransmission(this->m_DeviceAddress);
	if (this->m_pWire->write(uReg) != 1)
		return 0;
	if (this->m_pWire->endTransmission() != 0)
		return 0;
	if (this->m_pWire->requestFrom(this->m_DeviceAddress, 1u) != 1)
		return 0;
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
