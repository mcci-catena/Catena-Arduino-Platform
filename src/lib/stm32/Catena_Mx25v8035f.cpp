/* Catena_Mx25v8035f.cpp	Wed Mar 27 2019 11:07:13 chwon */

/*

Module:  Catena_Mx25v8035f.cpp

Function:
	Class for Catena_Mx25v8035f.

Version:
	V0.14.0	Wed Mar 27 2019 11:07:13 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2017, 2019 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	ChaeHee Won, MCCI Corporation	October 2017

Revision history:
   0.6.0  Mon Oct 30 2017 14:52:40  chwon
	Module created.

   0.6.0  Fri Dec 01 2017 13:56:52  chwon
	Add debug message and fix reading status register.

   0.14.0  Wed Mar 27 2019 11:07:13  chwon
	Make CS pin to input when power down.

*/

#ifdef ARDUINO_ARCH_STM32

#include <Catena_Mx25v8035f.h>

#include <stdlib.h>
#include <math.h>

#include <Catena_Flash.h>

#include "CatenaBase.h"
using namespace McciCatena;

/****************************************************************************\
|
|	Manifest constants & typedefs.
|
\****************************************************************************/

static void flashDelay(uint32_t howLong);

/****************************************************************************\
|
|	Read-only data.
|
\****************************************************************************/


/****************************************************************************\
|
|	Variables.
|
\****************************************************************************/


/*

Name:	Catena_Mx25v8035f::begin

Function:
	Begin method

Definition:
	boolean Catena_Mx25v8035f::begin(
		SPIClass *pSpi,
		uint8_t ChipSelectPin
		);

Description:
	This function initializes SPI and configures the chip.

Returns:
	true if success

*/

bool Catena_Mx25v8035f::begin(
	SPIClass *pSpi,
	uint8_t ChipSelectPin
	)
	{
	uint8_t ManufacturerId;
	uint16_t DeviceId;
	SPISettings SpiSettings(CATENA_MX25V8035F_SCLK, MSBFIRST, SPI_MODE0);

	if (pSpi == NULL)
		{
		// invalid parameter
		Serial.println("pSpi is NULL");
		return false;
		}

	this->m_pSpi = pSpi;
	this->m_CS = ChipSelectPin;

	pSpi->beginTransaction(this->m_CS, SpiSettings);

	/* force to put stand-by mode, just in case */
	this->m_PowerDown = true;
	this->powerUp();

	/* reset flash chip */
	this->reset();

	/* Make sure we're actually connected */
	this->readId(&ManufacturerId, &DeviceId);
	if (ManufacturerId != MX25V8035F_MANUFACTURER_ID)
		{
		// invalid manufacturer id
		Serial.print("Invalid flash manufacture id=");
		Serial.println(ManufacturerId, HEX);
		return false;
		}

	/* Everything seems to be properly initialised and connected */
	this->m_Initialized = true;

	/* we're not registering as a pollable object yet, so don't set m_Registered */
	return true;
	}

/*

Name:	Catena_Mx25v8035f::end

Function:
	End method

Definition:
	void Catena_Mx25v8035f::end(
		void
		);

Description:
	This function ends transaction.

Returns:
	No explicit result.

*/

void Catena_Mx25v8035f::end(
	void
	)
	{
	this->m_pSpi->endTransaction(this->m_CS);

	pinMode(this->m_CS, INPUT);
	digitalWrite(this->m_CS, 0);
	}

/*

Name:	flashDelay()

Function:
	Delay, driving the poll loop.

Definition:
	void flashDelay(
		uint32_t howLong
		);

Description:
	The current thread is delayed for `howLong` milliseconds.
	After each time the clock is checked, CatenaBase::pCatenaBase->poll() is
	called to ensure that other activities have a chance to
	run.

Returns:
	No explicit result.

*/

static void flashDelay(uint32_t howLong)
	{
	auto const tNow = millis();

	while (millis() - tNow < howLong)
		{
		CatenaBase::pCatenaBase->poll();
		}
	}

/*

Name:	Catena_Mx25v8035f::setWel

Function:
	Set WEL in flash chip.

Definition:
	void Catena_Mx25v8035f::setWel(
		void
		);

Description:
	This function sets the WEL latch in the MX25V8035F chip.

Returns:
	No explicit result.

*/

void Catena_Mx25v8035f::setWel(void)
	{
	uint8_t	status[2];
	auto const pSpi = this->m_pSpi;

	while (true)
		{
		pSpi->transfer(this->m_CS, MX25V8035F_CMD_WREN);
		status[0] = MX25V8035F_CMD_RDSR;
		pSpi->transfer(this->m_CS, status, sizeof(status));
		if ((status[1] & MX25V8035F_STS_WEL) != 0)
			break;
		CatenaBase::pCatenaBase->poll();
		}
	}

/*

Name:	Catena_Mx25v8035f::reset

Function:
	Reset MX25V8035F chip

Definition:
	void Catena_Mx25v8035f::reset(
		void
		);

Description:
	This function resets the MX25V8035F chip.

Returns:
	No explicit result.

*/

void Catena_Mx25v8035f::reset(
	void
	)
	{
	SPIClass * const pSpi = this->m_pSpi;

	pSpi->transfer(this->m_CS, MX25V8035F_CMD_RSTEN, SPI_LAST);
	pSpi->transfer(this->m_CS, MX25V8035F_CMD_RST, SPI_LAST);

	/* Reset recovery time: Chip Erase operation = 100ms */
	flashDelay(100);
	}

/*

Name:	Catena_Mx25v8035f::readId

Function:
	Read MX25V8035F chip ID

Definition:
	void Catena_Mx25v8035f::readId(
		uint8_t *pManufacturerId,
		uint16_t *pDeviceId
		);

Description:
	This function reads the MX25V8035F chip identification.

Returns:
	No explicit result.

*/

void Catena_Mx25v8035f::readId(
	uint8_t *pManufacturerId,
	uint16_t *pDeviceId
	)
	{
	SPIClass * const pSpi = this->m_pSpi;
	uint8_t data[4];

	data[0] = MX25V8035F_CMD_RDID;
	pSpi->transfer(this->m_CS, data, sizeof(data));

	if (pManufacturerId != NULL)
		*pManufacturerId = data[1];
	if (pDeviceId != NULL)
		*pDeviceId = (data[2] << 8) | data[3];
	}

/*

Name:	Catena_Mx25v8035f::eraseChip

Function:
	Erase MX25V8035F chip

Definition:
	void Catena_Mx25v8035f::eraseChip(
		void
		);

Description:
	This function erases MX25V8035F chip.

Returns:
	No explicit result.

*/

void Catena_Mx25v8035f::eraseChip(
	void
	)
	{
	SPIClass * const pSpi = this->m_pSpi;
	uint8_t	status[2];

	this->setWel();
	pSpi->transfer(this->m_CS, MX25V8035F_CMD_CE);

	this->pollWip(100);
	}

/*

Name:	Catena_Mx25v8035f::pollWip()

Function:
	Loop waiting for the WIP bit to go clear, with controlled poll interval

Definition:
	void Catena_Mx25v8035f::pollWip(
		uint32_t tPollMs
		);

Description:
	Periodically read the status register and check the WIP bit.
	Return as soon as it is observed to be clear. tPollMs sets the
	interval between polls, in milliseconds.

Returns:
	No explicit result.

Notes:
	There's no timeout; if WIP never clears, this will hang forever.

*/

void Catena_Mx25v8035f::pollWip(
	uint32_t tPollMs
	)
	{
	uint8_t status[2];
	auto const pSpi = this->m_pSpi;

	do	{
		flashDelay(tPollMs);
		status[0] = MX25V8035F_CMD_RDSR;
		pSpi->transfer(this->m_CS, status, sizeof(status));
		} while (status[1] & MX25V8035F_STS_WIP);
	}

/*

Name:	Catena_Mx25v8035f::erase

Function:
	Erase common function

Definition:
	void Catena_Mx25v8035f::erase(
		uint32_t Address,
		uint8_t Command,
		uint32_t Delay
		);

Description:
	This function is common function to erases MX25V8035F flash.

Returns:
	No explicit result.

*/

void Catena_Mx25v8035f::erase(
	uint32_t Address,
	uint8_t Command,
	uint32_t Delay
	)
	{
	SPIClass * const pSpi = this->m_pSpi;
	uint8_t	status[2];
	uint8_t data[4];

	data[0] = Command;
	data[1] = (Address >> 16) & 0xFF;
	data[2] = (Address >> 8) & 0xFF;
	data[3] = Address & 0xFF;

	this->setWel();

	pSpi->transfer(this->m_CS, data, sizeof(data));

	this->pollWip(Delay);
	}

/*

Name:	Catena_Mx25v8035f::eraseSector

Function:
	Erase MX25V8035F sector (4KB)

Definition:
	void Catena_Mx25v8035f::eraseSector(
		uint32_t SectorAddress
		);

Description:
	This function erases MX25V8035F sector (4KB).

Returns:
	No explicit result.

*/

void Catena_Mx25v8035f::eraseSector(
	uint32_t SectorAddress
	)
	{
	this->erase(SectorAddress, MX25V8035F_CMD_SE, 20 /*ms*/);
	}

/*

Name:	Catena_Mx25v8035f::eraseBlock32

Function:
	Erase MX25V8035F block (32KB)

Definition:
	void Catena_Mx25v8035f::eraseBlock32(
		uint32_t Block32Address
		);

Description:
	This function erases MX25V8035F block (32KB).

Returns:
	No explicit result.

*/

void Catena_Mx25v8035f::eraseBlock32(
	uint32_t Block32Address
	)
	{
	this->erase(Block32Address, MX25V8035F_CMD_BE_32K, 100 /*ms*/);
	}

/*

Name:	Catena_Mx25v8035f::eraseBlock64

Function:
	Erase MX25V8035F block (64KB)

Definition:
	void Catena_Mx25v8035f::eraseBlock64(
		uint32_t Block64Address
		);

Description:
	This function erases MX25V8035F block (64KB).

Returns:
	No explicit result.

*/

void Catena_Mx25v8035f::eraseBlock64(
	uint32_t Block64Address
	)
	{
	this->erase(Block64Address, MX25V8035F_CMD_BE, 200 /*ms*/);
	}

/*

Name:	Catena_Mx25v8035f::setProtection

Function:
	Erase MX25V8035F chip

Definition:
	void Catena_Mx25v8035f::setProtection(
		uint8_t protectionLevel
		);

Description:
	This function erases MX25V8035F chip.

Returns:
	No explicit result.

*/

void Catena_Mx25v8035f::setProtection(
	uint8_t protectionLevel
	)
	{
	SPIClass * const pSpi = this->m_pSpi;
	uint8_t	status[2];
	uint8_t data[3];

	data[0] = MX25V8035F_CMD_WRSR;
	data[1] = protectionLevel << 2;
	if (protectionLevel & 0x80)
		data[2] = MX25V8035F_CFG_TB;
	else
		data[2] = 0;

	this->setWel();

	pSpi->transfer(this->m_CS, data, sizeof(data));

	this->pollWip(1 /*ms*/);
	}

/*

Name:	Catena_Mx25v8035f::read

Function:
	Read a buffer from the specified flash address

Definition:
	void Catena_Mx25v8035f::read(
		uint32_t Address,
		uint8_t *pBuffer,
		size_t nBuffer
		);

Description:
	This function reads a buffer from the specified flash address.

Returns:
	No explicit result.

*/

void Catena_Mx25v8035f::read(
	uint32_t Address,
	uint8_t *pBuffer,
	size_t nBuffer
	)
	{
	SPIClass * const pSpi = this->m_pSpi;
	uint8_t data[4];

	data[0] = MX25V8035F_CMD_READ;
	data[1] = (Address >> 16) & 0xFF;
	data[2] = (Address >> 8) & 0xFF;
	data[3] = Address & 0xFF;

	pSpi->transfer(this->m_CS, data, sizeof(data), SPI_CONTINUE);
	pSpi->transfer(this->m_CS, pBuffer, nBuffer, SPI_LAST);
	}

/*

Name:	Catena_Mx25v8035f::programPage

Function:
	Program a buffer to the specified flash address

Definition:
	size_t Catena_Mx25v8035f::programPage(
		uint32_t Address,
		uint8_t *pBuffer,
		size_t nBuffer
		);

Description:
	This function programs a buffer to the specified flash address.

Returns:
	The number of bytes programmed at Address.

*/

size_t Catena_Mx25v8035f::programPage(
	uint32_t Address,
	const uint8_t *pBuffer,
	size_t nBuffer
	)
	{
	SPIClass * const pSpi = this->m_pSpi;
	uint8_t	status[2];
	uint8_t data[MX25V8035F_PAGE_SIZE];
	size_t	programSize;

	data[0] = MX25V8035F_CMD_PP;
	data[1] = (Address >> 16) & 0xFF;
	data[2] = (Address >> 8) & 0xFF;
	data[3] = Address & 0xFF;

	programSize = MX25V8035F_PAGE_SIZE - data[3];
	if (programSize > nBuffer)
		programSize = nBuffer;

	this->setWel();

	pSpi->transfer(this->m_CS, data, 4, SPI_CONTINUE);
	memcpy(data, pBuffer, programSize);
	pSpi->transfer(this->m_CS, data, programSize, SPI_LAST);

	this->pollWip(1 /* ms */);

	return programSize;
	}

/*

Name:	Catena_Mx25v8035f::program

Function:
	Program a buffer to the specified flash address

Definition:
	void Catena_Mx25v8035f::program(
		uint32_t Address,
		const uint8_t *pBuffer,
		size_t nBuffer
		);

Description:
	This function programs a buffer to the specified flash address.

Returns:
	No explicit result.

*/

void Catena_Mx25v8035f::program(
	uint32_t Address,
	const uint8_t *pBuffer,
	size_t nBuffer
	)
	{
	while (nBuffer > 0)
		{
		size_t	programSize;

		programSize = this->programPage(Address, pBuffer, nBuffer);
		Address += programSize;
		pBuffer += programSize;
		nBuffer -= programSize;
		}
	}

/*

Name:	Catena_Mx25v8035f::powerDown

Function:
	Power down flash chip

Definition:
	void Catena_Mx25v8035f::powerDown(
		void
		);

Description:
	This function puts flash chip deep power down mode.

Returns:
	No explicit result.

*/

void Catena_Mx25v8035f::powerDown(
	void
	)
	{
	SPIClass * const pSpi = this->m_pSpi;
	uint32_t uSec;

	if (this->m_PowerDown)
		{
		digitalWrite(this->m_CS, 1);
		pinMode(this->m_CS, OUTPUT);

		uSec = micros();
		while ((micros() - uSec) < 10);
		}

	pSpi->transfer(this->m_CS, MX25V8035F_CMD_DP);

	/* tDP == Max 10us */
	uSec = micros();
	this->m_PowerDown = true;
	while ((micros() - uSec) < 10);

	pinMode(this->m_CS, INPUT);
	digitalWrite(this->m_CS, 0);
	}

/*

Name:	Catena_Mx25v8035f::powerUp

Function:
	Power up flash chip

Definition:
	void Catena_Mx25v8035f::powerUp(
		void
		);

Description:
	This function puts flash chip stand-by mode.

Returns:
	No explicit result.

*/

void Catena_Mx25v8035f::powerUp(
	void
	)
	{
	/* tDPDD == Min 30us */
	if (this->m_PowerDown)
		{
		uint32_t uSec;

		pinMode(this->m_CS, OUTPUT);
		digitalWrite(this->m_CS, 0);

		/* tCRDP == Min 20ns */
		this->m_PowerDown = false;
		digitalWrite(this->m_CS, 1);

		/* tRDP == Min 35us */
		uSec = micros();
		while ((micros() - uSec) < 35);
		}
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Catena_Mx25v8035f.cpp ****/
