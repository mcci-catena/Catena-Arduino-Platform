/* Catena_Mb85rc64ta.cpp	Wed Dec 06 2017 15:33:28 chwon */

/*

Module:  Catena_Mb85rc64ta.cpp

Function:
	Class for Catena_Mb85rc64ta.

Version:
	V0.6.0	Wed Dec 06 2017 15:33:28 chwon	Edit level 2

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

   0.6.0  Wed Dec 06 2017 15:33:28  chwon
	Add readId and power control.

*/

#include <stdlib.h>
#include <math.h>

#include "Catena_Mb85rc64ta.h"
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

Catena_Mb85rc64ta::Catena_Mb85rc64ta(void)
	{
	this->m_Initialized = false;
	}

void Catena_Mb85rc64ta::prepIO(void) const
	{
	/* this->m_pWire->setClock(1000000);	*/
	}

/*
|| Public functions
*/

/*

Name:	Catena_Mb85rc64ta::begin

Function:
	Begin method

Definition:
	bool Catena_Mb85rc64ta::begin(
		uint8_t DeviceAddress,
		TwoWire *pWire
		);

Description:
	This function initializes I2C and configures the chip.

Returns:
	true if success

*/

bool Catena_Mb85rc64ta::begin(
	uint8_t DeviceAddress,
	TwoWire *pWire
	)
	{
	uint8_t	i;
	uint8_t	uError;

	/* scrub and save the address */
	if (DeviceAddress == 0)
		DeviceAddress = CATENA_MB85RC64TA_ADDRESS;

	this->m_DeviceAddress = DeviceAddress & ~0x7;
	this->m_pWire = pWire;

	pWire->begin();

	/* Make sure we're actually connected */
	this->prepIO();

	for (i = 0; i < 8; ++this->m_DeviceAddress, ++i)
		{
		/* force to put stand-by mode, just in case */
		this->m_PowerDown = true;
		this->powerUp();

		pWire->beginTransmission(this->m_DeviceAddress);
		uError = pWire->endTransmission();
		if (uError == 0)
			{
			break;
			}
		
		}

	if (i == 8)
		{
		// device didn't ack
		return false;
		}

	/* Everything seems to be properly initialised and connected */
	this->m_Initialized = true;

	return true;
	}

/*

Name:	Catena_Mb85rc64ta::write8

Function:
	Writes a byte at the specific FRAM address

Definition:
	bool Catena_Mb85rc64ta::write8(
		uint16_t framAddr,
		uint8_t value
		);

Description:
	This function writes a byte at the specific FRAM address.

Returns:
	True for success, false for error.

*/

bool Catena_Mb85rc64ta::write8(
	uint16_t framAddr,
	uint8_t value
	)
	{
	this->prepIO();
	this->m_pWire->beginTransmission(this->m_DeviceAddress);
	if (this->m_pWire->write(framAddr >> 8) == 1 &&
	    this->m_pWire->write(framAddr & 0xFF) == 1 &&
	    this->m_pWire->write(value) == 1 &&
	    this->m_pWire->endTransmission() == 0)
	    	return true;
	else
		return false;
	}

/*

Name:	Catena_Mb85rc64ta::write

Function:
	Writes a buffer to the specific FRAM address

Definition:
	size+t Catena_Mb85rc64ta::write(
		uint16_t framAddr,
		const uint8_t *pBuffer,
		size_t nBuffer
		);

Description:
	This function writes a buffer to the specific FRAM address

Returns:
	Number of bytes written, which must be either nBuffer for
	success or some number less than nBuffer for failure.

*/

size_t Catena_Mb85rc64ta::write(
	uint16_t framAddr,
	const uint8_t *pBuffer,
	size_t nBuffer
	)
	{
	// There are numerous limitations on TwoWire buffer size. On 8-bit
	// Arduinos, the buffer size is limited to 32 bytes. On STM32 Arduinos,
	// the buffer size is not limited, but depends on malloc(), which is
	// not really reliable on systems with small memory. So... do this in
	// chunks.
	constexpr size_t nChunk = 32;
	const auto nOriginal = nBuffer;

	this->prepIO();

	while (nBuffer != 0)
		{
		const auto nThis = nBuffer > nChunk ? nChunk : nBuffer;

		this->m_pWire->beginTransmission(this->m_DeviceAddress);
		if (this->m_pWire->write(framAddr >> 8) &&
		    this->m_pWire->write(framAddr & 0xFF) &&
		    this->m_pWire->write(pBuffer, nThis) == nThis &&
		    this->m_pWire->endTransmission() == 0)
		    	{
			nBuffer -= nThis;
			pBuffer += nThis;
			framAddr += nThis;
		    	}
		else
			// error!
			break;
		}

	// return count of bytes written.
	return nOriginal - nBuffer;
	}

/*

Name:	Catena_Mb85rc64ta::read8

Function:
	Reads an 8 bit value from the specified FRAM address

Definition:
	uint8_t Catena_Mb85rc64ta::read8(
		uint16_t framAddr
		);

Description:
	This function reads an 8 bit value from the specified FRAM address.

Returns:
	The 8-bit value retrieved at framAddr.

*/

uint8_t Catena_Mb85rc64ta::read8(
	uint16_t framAddr
	)
	{
	this->prepIO();
	this->m_pWire->beginTransmission(this->m_DeviceAddress);
	if (this->m_pWire->write(framAddr >> 8) &&
	    this->m_pWire->write(framAddr & 0xFF) &&
	    this->m_pWire->endTransmission() == 0 &&
	    this->m_pWire->requestFrom(this->m_DeviceAddress, (uint8_t)1) == 1)
		return this->m_pWire->read();
	else
		// error!
		return 0;
	}

/*

Name:	Catena_Mb85rc64ta::read()

Function:
	Reads a buffer from the specified FRAM address

Definition:
	size_t Catena_Mb85rc64ta::read(
		uint16_t framAddr,
		uint8_t *pBuffer,
		size_t nBuffer
		);

Description:
	This function reads a buffer from the specified FRAM address.

Returns:
	The number of bytes retrieved at framAddr.

*/

size_t Catena_Mb85rc64ta::read(
	uint16_t framAddr,
	uint8_t *pBuffer,
	size_t nBuffer
	)
	{
	// There are numerous limitations on TwoWire buffer size. On 8-bit
	// Arduinos, the buffer size is limited to 32 bytes. On STM32 Arduinos,
	// the buffer size is not limited, but depends on malloc(), which is
	// not really reliable on systems with small memory. So... do this in
	// chunks.
	constexpr size_t nChunk = 32;
	const auto save_nBuffer = nBuffer;

	this->prepIO();
	while (nBuffer > 0)
		{
		const auto nThis = nBuffer > nChunk ? nChunk : nBuffer;
		uint8_t	nRead;

		this->m_pWire->beginTransmission(this->m_DeviceAddress);
		if (this->m_pWire->write(framAddr >> 8) &&
		    this->m_pWire->write(framAddr & 0xFF) &&
		    this->m_pWire->endTransmission() == 0)
		    	{
			nRead = this->m_pWire->requestFrom(
				this->m_DeviceAddress,
				(uint8_t) nThis
				);

			if (nRead != nThis)
				{
				// error
				break;
				}
			while (nRead > 0)
				{
				*pBuffer++ = this->m_pWire->read();
				--nBuffer;
				--nRead;
				}
			framAddr += nThis;
			}
		else
			// error
			break;
		}

	// return count of bytes read.
	return save_nBuffer - nBuffer;
	}

/*

Name:	Catena_Mb85rc64ta::readId

Function:
	Get the FRAM JEDEC IDs

Definition:
	bool Catena_Mb85rc64ta::readId(
		uint16_t *pManufactureId,
		uint16_t *pProductId
		);

Description:
	This function reads a buffer from the specified FRAM address.

Returns:
	true for success, false for failure.

*/

bool Catena_Mb85rc64ta::readId(
	uint16_t *pManufactureId,
	uint16_t *pProductId
	)
	{
	uint8_t	data;

	this->prepIO();
	this->m_pWire->beginTransmission(CATENA_MB85RC64TA_SLAVE_ID);
	if (this->m_pWire->write(this->m_DeviceAddress << 1) &&
	    this->m_pWire->endTransmission() == 0)
		{
		data = this->m_pWire->requestFrom(
			CATENA_MB85RC64TA_SLAVE_ID,
			3
			);
		}
	else
		data = 0;

	if (data != 3)
		{
		Serial.println("FRAM readId() failed");
		*pManufactureId = 0;
		*pProductId = 0;
		return false;
		}
	else
		{
		data = this->m_pWire->read();
		*pManufactureId = data << 4;
		data = this->m_pWire->read();
		*pManufactureId |= data & 0x0Fu;
		data = this->m_pWire->read();
		*pProductId = data & 0xF0u << 8;
		data = this->m_pWire->read();
		*pProductId |= data;
		return true;
		}
	}

/*

Name:	Catena_Mb85rc64ta::powerDown

Function:
	Put low power mode

Definition:
	void Catena_Mb85rc64ta::powerDown(
		void
		);

Description:
	This function puts low power mode of the FRAM.

Returns:
	No explicit result.

Notes:
	No error checking.

*/

void Catena_Mb85rc64ta::powerDown(
	void
	)
	{
	if (! this->m_PowerDown)
		{
		this->prepIO();
		this->m_pWire->beginTransmission(CATENA_MB85RC64TA_SLAVE_ID);
		this->m_pWire->write(this->m_DeviceAddress << 1);
		this->m_pWire->endTransmission(false);
		this->m_pWire->beginTransmission(0x86 >> 1);
		this->m_pWire->endTransmission();
		this->m_PowerDown = true;
		}
	}

/*

Name:	Catena_Mb85rc64ta::powerUp

Function:
	Exit from low power mode

Definition:
	void Catena_Mb85rc64ta::powerUp(
		void
		);

Description:
	This function exits from low power mode of the FRAM.

Returns:
	No explicit result.

*/

void Catena_Mb85rc64ta::powerUp(
	void
	)
	{
	if (this->m_PowerDown)
		{
		uint32_t uSec;

		this->m_pWire->beginTransmission(this->m_DeviceAddress);
		this->m_pWire->endTransmission(false);
	
		/* tREC == Max 400us */
		uSec = micros();
		this->m_PowerDown = false;
		while ((micros() - uSec) < 400);
		}
	}

/**** end of Catena_Mb85rc64ta.cpp ****/
