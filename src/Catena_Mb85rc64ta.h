/* Catena_Mb85rc64ta.h	Wed Dec 06 2017 15:30:56 chwon */

/*

Module:  Catena_Mb85rc64ta.h

Function:
	class Catena_Mb85rc64ta

Version:
	V0.6.0	Wed Dec 06 2017 15:30:56 chwon	Edit level 2

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

   0.6.0  Wed Dec 06 2017 15:30:56  chwon
	Add readId and power control.

*/

#ifndef _CATENA_MB85RC64TA_H_		/* prevent multiple includes */
#define _CATENA_MB85RC64TA_H_

#pragma once

#include <Arduino.h>
#include <Wire.h>

/* TODO: change these to enums and constepxrs in the McciCatena namespace */

#define CATENA_MB85RC64TA_ADDRESS	(0x50)
#define CATENA_MB85RC64TA_SLAVE_ID	(0xF8 >> 1)

namespace McciCatena {

class Catena_Mb85rc64ta
	{
public:
	Catena_Mb85rc64ta(void);

	// set up and probe device
	boolean begin(uint8_t DeviceAddress = CATENA_MB85RC64TA_ADDRESS,
		      TwoWire *pWire = &Wire);

	// write a single byte
	void write8(uint16_t framAddr, uint8_t value);

	// write a buffer
	void write(uint16_t framAddr, uint8_t const *pBuffer, size_t nBuffer);

	// read a single byte
	uint8_t read8(uint16_t framAddr);

	// read a buffer
	size_t read(uint16_t framAddr, uint8_t *pBuffer, size_t nBuffer);

	// read ID
	void readId(uint16_t *pManufactureId, uint16_t *pProductId);

	// power management
	void powerDown(void);
	void powerUp(void);

private:
	uint8_t		m_DeviceAddress;
	boolean		m_Initialized;
	boolean		m_PowerDown;
	TwoWire *	m_pWire;

	void prepIO(void) const;
	};

} // namespace McciCatena

/**** end of Catena_Mb85rc64ta.h ****/
#endif /* _CATENA_MB85RC64TA_H_ */
