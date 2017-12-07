/* Catena_Mx25v8035f.cpp	Fri Dec 01 2017 13:56:52 chwon */

/*

Module:  Catena_Mx25v8035f.cpp

Function:
	Class for Catena_Mx25v8035f.

Version:
	V0.6.0	Fri Dec 01 2017 13:56:52 chwon	Edit level 2

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
   0.6.0  Mon Oct 30 2017 14:52:40  chwon
	Module created.

   0.6.0  Fri Dec 01 2017 13:56:52  chwon
	Add debug message and fix reading status register.

*/

#ifdef ARDUINO_ARCH_STM32

#include <stdlib.h>
#include <math.h>

#include "Catena_Mx25v8035f.h"
using namespace McciCatena;

/****************************************************************************\
|
|		Manifest constants & typedefs.
|
|	This is strictly for private types and constants which will not
|	be exported.
|
\****************************************************************************/



/****************************************************************************\
|
|	Read-only data.
|
|	If program is to be ROM-able, these must all be tagged read-only
|	using the ROM storage class; they may be global.
|
\****************************************************************************/



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

/*
|| Constructors
*/

Catena_Mx25v8035f::Catena_Mx25v8035f(void)
	{
	this->m_Initialized = false;
	}

/*
|| Public functions
*/

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

boolean Catena_Mx25v8035f::begin(
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
	delay(100);	/* Reset recovery time: Chip Erase operation = 100ms */
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

	do	{
		pSpi->transfer(this->m_CS, MX25V8035F_CMD_WREN);
		status[0] = MX25V8035F_CMD_RDSR;
		pSpi->transfer(this->m_CS, status, sizeof(status));
		} while ((status[1] & MX25V8035F_STS_WEL) == 0);

	pSpi->transfer(this->m_CS, MX25V8035F_CMD_CE);

	do	{
		delay(500);
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

	do	{
		pSpi->transfer(this->m_CS, MX25V8035F_CMD_WREN);
		status[0] = MX25V8035F_CMD_RDSR;
		pSpi->transfer(this->m_CS, status, sizeof(status));
		} while ((status[1] & MX25V8035F_STS_WEL) == 0);

	pSpi->transfer(this->m_CS, data, sizeof(data));

	do	{
		delay(Delay);
		status[0] = MX25V8035F_CMD_RDSR;
		pSpi->transfer(this->m_CS, status, sizeof(status));
		} while (status[1] & MX25V8035F_STS_WIP);
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
	this->erase(SectorAddress, MX25V8035F_CMD_SE, 20);
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
	this->erase(Block32Address, MX25V8035F_CMD_BE_32K, 100);
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
	this->erase(Block64Address, MX25V8035F_CMD_BE, 200);
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

	do	{
		pSpi->transfer(this->m_CS, MX25V8035F_CMD_WREN);
		status[0] = MX25V8035F_CMD_RDSR;
		pSpi->transfer(this->m_CS, status, sizeof(status));
		} while ((status[1] & MX25V8035F_STS_WEL) == 0);

	pSpi->transfer(this->m_CS, data, sizeof(data));

	do	{
		delay(1);
		status[0] = MX25V8035F_CMD_RDSR;
		pSpi->transfer(this->m_CS, status, sizeof(status));
		} while (status[1] & MX25V8035F_STS_WIP);
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
	uint8_t *pBuffer,
	size_t nBuffer
	)
	{
	SPIClass * const pSpi = this->m_pSpi;
	uint8_t	status[2];
	uint8_t data[4];
	size_t	programSize;

	data[0] = MX25V8035F_CMD_PP;
	data[1] = (Address >> 16) & 0xFF;
	data[2] = (Address >> 8) & 0xFF;
	data[3] = Address & 0xFF;

	programSize = MX25V8035F_PAGE_SIZE - data[3];
	if (programSize > nBuffer)
		programSize = nBuffer;

	do	{
		pSpi->transfer(this->m_CS, MX25V8035F_CMD_WREN);
		status[0] = MX25V8035F_CMD_RDSR;
		pSpi->transfer(this->m_CS, status, sizeof(status));
		} while ((status[1] & MX25V8035F_STS_WEL) == 0);

	pSpi->transfer(this->m_CS, data, sizeof(data), SPI_CONTINUE);
	pSpi->transfer(this->m_CS, pBuffer, programSize, SPI_LAST);

	do	{
		delay(1);
		status[0] = MX25V8035F_CMD_RDSR;
		pSpi->transfer(this->m_CS, status, sizeof(status));
		} while (status[1] & MX25V8035F_STS_WIP);

	return programSize;
	}

/*

Name:	Catena_Mx25v8035f::program

Function:
	Program a buffer to the specified flash address

Definition:
	void Catena_Mx25v8035f::program(
		uint32_t Address,
		uint8_t *pBuffer,
		size_t nBuffer
		);

Description:
	This function programs a buffer to the specified flash address.

Returns:
	No explicit result.

*/

void Catena_Mx25v8035f::program(
	uint32_t Address,
	uint8_t *pBuffer,
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

	pSpi->transfer(this->m_CS, MX25V8035F_CMD_DP);

	/* tDP == Max 10us */
	uSec = micros();
	this->m_PowerDown = true;
	while ((micros() - uSec) < 10);
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
