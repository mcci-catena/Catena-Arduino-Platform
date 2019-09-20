/*

Module:  Catena_Fram_fm24cl16b.h

Function:
       class cFram_FM24CL16B: 2K I2C FRAM

Copyright notice:
        See accompanying LICENSE file.

Author:
        ChaeHee Won, MCCI Corporation	September 2019

*/

#ifndef _CATENA_FRAM_FM24CL16B_H_	/* prevent multiple includes */
#define _CATENA_FRAM_FM24CL16B_H_

#pragma once

#include <Arduino.h>
#include <Wire.h>

namespace McciCatena {

class cFram_FM24CL16B
	{
public:
	cFram_FM24CL16B(void);

        static constexpr uint8_t CATENA_FM24CL16B_ADDRESS = 0x50;
        static constexpr int CATENA_FM24CL16B_SLAVE_ID = (0xF8 >> 1);

	// set up and probe device
	boolean begin(uint8_t DeviceAddress = CATENA_FM24CL16B_ADDRESS,
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

private:
	uint8_t		m_DeviceAddress;
	boolean		m_Initialized;
	boolean		m_PowerDown;
	TwoWire *	m_pWire;

	void prepIO(void) const;
	uint8_t getDeviceAddress(uint16_t framAddr) const;
	};

} // namespace McciCatena

/**** end of Catena_Fram_fm24cl16b.h ****/
#endif /* _CATENA_FRAM_FM24CL16B_H_ */
