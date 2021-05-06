/*

Name:	Catena_Flash.h

Function:
        Abstract flash base class for McciCatena.

Copyright:
        See accompanying LICENSE file.

Author:
        Terry Moore, MCCI Corporation	April 2021

*/

#pragma once

#ifndef _catena_flash_h_
#define _catena_flash_h_

#include <Arduino.h>
#include <SPI.h>

namespace McciCatena {

class cFlash
    {
public:
    // default constructor
    cFlash() {}
    // neither copyable nor movable
    cFlash(const cFlash&) = delete;
    cFlash& operator=(const cFlash&) = delete;
    cFlash(const cFlash&&) = delete;
    cFlash& operator=(const cFlash&&) = delete;

    // public methods

    /// \brief initialize flash system
    virtual bool begin(SPIClass *pSpi, uint8_t ChipSelectPin) = 0;

    /// \brief stop the flash system
    virtual void end() = 0;

	/// \brief reset chip
	virtual void reset(void) = 0;

	/// \brief read JEDEC id
	virtual void readId(uint8_t *pManufacturerId, uint16_t *pDeviceId) = 0;

	/// \brief chip erase
	virtual void eraseChip(void) = 0;

    /// \brief erase a 4k page
	virtual void eraseSector(uint32_t SectorAddress) = 0;

    /// \brief erase a 32k page
	virtual void eraseBlock32(uint32_t Block32Address) = 0;

    /// \brief erase a 64k page
	virtual void eraseBlock64(uint32_t Block64Address) = 0;

	// set protection -- CATENA_MX25V8035F_PL_xxx
	virtual void setProtection(uint8_t protectionLevel) = 0;

    /// \brief read buffer
	virtual void read(uint32_t Address, uint8_t *pBuffer, size_t nBuffer) = 0;

    /// \brief program buffer
	virtual void program(uint32_t Address, const uint8_t *pBuffer, size_t nBuffer) = 0;

    /// \brief program at most one page.
    /// \returns number of bytes not consumed from buffer.
	virtual size_t programPage(uint32_t Address, const uint8_t *pBuffer, size_t nBuffer) = 0;

	/// \brief power flash down
	virtual void powerDown(void) = 0;

	/// \brief power flash ups
	virtual void powerUp(void) = 0;
    };

} // namespace McciCatena

#endif /* _catena_flash_h_ */
