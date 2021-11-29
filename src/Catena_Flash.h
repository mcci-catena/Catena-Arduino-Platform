/*

Name:	Catena_Flash.h

Function:
        Abstract flash base class for McciCatena.

Copyright:
        See accompanying LICENSE file.

Author:
        Terry Moore, MCCI Corporation	April 2021

*/

/// \file

#pragma once

#ifndef _catena_flash_h_
/// \cond
#define _catena_flash_h_
/// \endcond

#include <Arduino.h>
#include <SPI.h>

namespace McciCatena {

///
/// \brief Abstract class for SPI flash devices.
///
/// \details
///     This base class is used as the interface for all flash drivers.
///     At the moment, we don't allow for a lot of code sharing between
///     various driver types, but there's no reason this couldn't be
///     extended to support that in the future.
///
class cFlash
    {
public:
    /// \brief default constructor
    cFlash() {}

    ///
    /// \brief the default destructor is public and virtual, as recommended
    /// by cppreference.com under [desctructors](https://en.cppreference.com/w/cpp/language/destructor).
    ///
    /// This allows destruction via a pointer to the base class.
    ///
    virtual ~cFlash() {}

    // neither copyable nor movable
    cFlash(const cFlash&) = delete;
    cFlash& operator=(const cFlash&) = delete;
    cFlash(const cFlash&&) = delete;
    cFlash& operator=(const cFlash&&) = delete;

    // public methods

    ///
    /// \brief initialize flash driver with pin.
    ///
    /// \param[in] pSpi pointer to SPI bus for this flash device.
    /// \param[in] ChipSelectPin pin to be used as chip-select for this flash device, or
    ///             -1 to defer assignment.
    ///
    virtual bool begin(SPIClass *pSpi, int16_t ChipSelectPin) = 0;

    ///
    /// \brief initialize flash driver using default chip select.
    ///
    /// \param[in] pSpi pointer to SPI bus for this flash device.
    ///
    /// The default chip select pin from instantiation is used (or the call returns failure if
    /// no pin was provided).
    ///
    virtual bool begin(SPIClass *pSpi) = 0;

    ///
    /// \brief get chip-select pin
    ///
    virtual uint8_t getChipSelectPin() const = 0;

    ///
    /// \brief stop the flash driver
    ///
    virtual void end() = 0;

	/// \brief reset flash chip
	virtual void reset(void) = 0;

    ///
	/// \brief read JEDEC id
    ///
    /// \param[out] pManufacturerId is set to the manufacturer ID read from the flash device.
    /// \param[out] pDeviceId is set to the device ID.
    ///
	virtual void readId(uint8_t *pManufacturerId, uint16_t *pDeviceId) = 0;

    ///
	/// \brief Issue chip erase.
    ///
	virtual bool eraseChip(void) = 0;

    ///
    /// \brief Erase a 4k page.
    ///
    /// \param[in] SectorAddress base byte address of the page to be erased.
    ///
	virtual bool eraseSector(uint32_t SectorAddress) = 0;

    ///
    /// \brief erase a 32k page
    ///
    /// \param[in] Block32Address base byte address of the block to be erased.
    ///
	virtual bool eraseBlock32(uint32_t Block32Address) = 0;

    ///
    /// \brief erase a 64k page
	virtual bool eraseBlock64(uint32_t Block64Address) = 0;

	// set protection is not portable,not part of API
	// virtual void setProtection(uint8_t protectionLevel) = 0;

    /// \brief read buffer
	virtual void read(uint32_t Address, uint8_t *pBuffer, size_t nBuffer) = 0;

    /// \brief program buffer
	virtual bool program(uint32_t Address, const uint8_t *pBuffer, size_t nBuffer) = 0;

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
