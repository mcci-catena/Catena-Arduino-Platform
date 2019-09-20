/*

Module:  Catena_Fram_fm24cl16b.cpp

Function:
       class cFram_FM24CL16B: 2K I2C FRAM

Copyright notice:
        See accompanying LICENSE file.

Author:
        ChaeHee Won, MCCI Corporation	September 2019

*/

#include <stdlib.h>
#include <math.h>

#include "Catena_Fram_fm24cl16b.h"
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

cFram_FM24CL16B::cFram_FM24CL16B(void)
	{
	this->m_Initialized = false;
	}

void cFram_FM24CL16B::prepIO(void) const
	{
	/* this->m_pWire->setClock(1000000);	*/
	}

uint8_t cFram_FM24CL16B::getDeviceAddress(uint16_t framAddr) const
	{
	return this->m_DeviceAddress + ((framAddr >> 8) & 0x7);
	}

/*
|| Public functions
*/

/*

Name:	cFram_FM24CL16B::begin

Function:
	Begin method

Definition:
	boolean cFram_FM24CL16B::begin(
		uint8_t DeviceAddress,
		TwoWire *pWire
		);

Description:
	This function initializes I2C and configures the chip.

Returns:
	true if success

*/

boolean cFram_FM24CL16B::begin(
	uint8_t DeviceAddress,
	TwoWire *pWire
	)
	{
	uint8_t	i;
	uint8_t	uError;

	/* scrub and save the address */
	if (DeviceAddress == 0)
		DeviceAddress = CATENA_FM24CL16B_ADDRESS;

	this->m_DeviceAddress = DeviceAddress & ~0x7;
	this->m_pWire = pWire;

	pWire->begin();

	/* Make sure we're actually connected */
	this->prepIO();

	for (i = 0; i < 8; ++i)
		{
		pWire->beginTransmission(this->m_DeviceAddress + i);
		uError = pWire->endTransmission();
		if (uError != 0)
			{
			// device didn't ack
			return false;
			}
		}

	/* Everything seems to be properly initialised and connected */
	this->m_Initialized = true;

	return true;
	}

/*

Name:	cFram_FM24CL16B::write8

Function:
	Writes a byte at the specific FRAM address

Definition:
	void cFram_FM24CL16B::write8(
		uint16_t framAddr,
		uint8_t value
		);

Description:
	This function writes a byte at the specific FRAM address.

Returns:
	No explicit result.

*/

void cFram_FM24CL16B::write8(
	uint16_t framAddr,
	uint8_t value
	)
	{
	uint8_t const DeviceAddress = this->getDeviceAddress(framAddr);

	this->prepIO();
	this->m_pWire->beginTransmission(DeviceAddress);
	this->m_pWire->write(framAddr & 0xFF);
	this->m_pWire->write(value);
	this->m_pWire->endTransmission();
	}

/*

Name:	cFram_FM24CL16B::write

Function:
	Writes a buffer to the specific FRAM address

Definition:
	void cFram_FM24CL16B::write(
		uint16_t framAddr,
		const uint8_t *pBuffer,
		size_t nBuffer
		);

Description:
	This function writes a buffer to the specific FRAM address

Returns:
	No explicit result.

*/

void cFram_FM24CL16B::write(
	uint16_t framAddr,
	const uint8_t *pBuffer,
	size_t nBuffer
	)
	{
	uint8_t const DeviceAddress = this->getDeviceAddress(framAddr);

	this->prepIO();
	this->m_pWire->beginTransmission(DeviceAddress);
	this->m_pWire->write(framAddr & 0xFF);
	this->m_pWire->write(pBuffer, nBuffer);
	this->m_pWire->endTransmission();
	}

/*

Name:	cFram_FM24CL16B::read8

Function:
	Reads an 8 bit value from the specified FRAM address

Definition:
	uint8_t cFram_FM24CL16B::read8(
		uint16_t framAddr
		);

Description:
	This function reads an 8 bit value from the specified FRAM address.

Returns:
	The 8-bit value retrieved at framAddr.

*/

uint8_t cFram_FM24CL16B::read8(
	uint16_t framAddr
	)
	{
	uint8_t const DeviceAddress = this->getDeviceAddress(framAddr);

	this->prepIO();
	this->m_pWire->beginTransmission(DeviceAddress);
	this->m_pWire->write(framAddr & 0xFF);
	this->m_pWire->endTransmission();

	this->m_pWire->requestFrom(DeviceAddress, (uint8_t)1);
	while (! this->m_pWire->available())
		/* loop */;

	return this->m_pWire->read();
	}

/*

Name:	cFram_FM24CL16B::read

Function:
	Reads a buffer from the specified FRAM address

Definition:
	size_t cFram_FM24CL16B::read(
		uint16_t framAddr,
		uint8_t *pBuffer,
		size_t nBuffer
		);

Description:
	This function reads a buffer from the specified FRAM address.

Returns:
	The number of bytes retrieved at framAddr.

*/

size_t cFram_FM24CL16B::read(
	uint16_t framAddr,
	uint8_t *pBuffer,
	size_t nBuffer
	)
	{
	size_t const save_nBuffer = nBuffer;

	this->prepIO();
	while (nBuffer > 0)
		{
		uint8_t const DeviceAddress = this->getDeviceAddress(framAddr);
		uint8_t	nRead;

		this->m_pWire->beginTransmission(DeviceAddress);
		this->m_pWire->write(framAddr & 0xFF);
		this->m_pWire->endTransmission();

		nRead = nBuffer > 128 ? 128 : (uint8_t) nBuffer;
		nRead = this->m_pWire->requestFrom(
				DeviceAddress,
				nRead
				);
		if (nRead == 0)
			{
			break;
			}

		framAddr += nRead;
		nBuffer -= nRead;
		while (this->m_pWire->available() && nRead > 0)
			{
			*pBuffer++ = this->m_pWire->read();
			--nRead;
			}
		}

	return save_nBuffer - nBuffer;
	}

/*

Name:	cFram_FM24CL16B::readId

Function:
	Reads a buffer from the specified FRAM address

Definition:
	void cFram_FM24CL16B::readId(
		uint16_t *pManufactureId,
		uint16_t *pProductId
		);

Description:
	This function reads a buffer from the specified FRAM address.

Returns:
	No explicit result.

*/

void cFram_FM24CL16B::readId(
	uint16_t *pManufactureId,
	uint16_t *pProductId
	)
	{
	uint8_t	data;

	this->prepIO();
	this->m_pWire->beginTransmission(CATENA_FM24CL16B_SLAVE_ID);
	this->m_pWire->write(this->m_DeviceAddress << 1);
	this->m_pWire->endTransmission(false);

	data = this->m_pWire->requestFrom(
			CATENA_FM24CL16B_SLAVE_ID,
			3
			);
	if (data == 0)
		{
		Serial.println("FRAM readId() failed");
		}

	data = this->m_pWire->read();
	*pManufactureId = data << 4;
	data = this->m_pWire->read();
	*pManufactureId |= data >> 4;
	*pProductId = (data & 0x0Fu) << 8;
	data = this->m_pWire->read();
	*pProductId |= data;
	}

/**** end of Catena_Fram_fm24cl16b.cpp ****/
