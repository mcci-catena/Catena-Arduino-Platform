/*

Module:  Catena_Mb85rc64ta.h

Function:
       class Catena_Mb85rc64ta: 8K I2C FRAM

Copyright notice:
        See accompanying LICENSE file.

Author:
        ChaeHee Won, MCCI Corporation	October 2017

*/

#ifndef _CATENA_MB85RC64TA_H_		/* prevent multiple includes */
#define _CATENA_MB85RC64TA_H_

#pragma once

#include <Arduino.h>
#include <Wire.h>

namespace McciCatena {

class Catena_Mb85rc64ta
	{
public:
	Catena_Mb85rc64ta(void);

        static constexpr uint8_t CATENA_MB85RC64TA_ADDRESS = 0x50;
        static constexpr int CATENA_MB85RC64TA_SLAVE_ID = (0xF8 >> 1);

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
