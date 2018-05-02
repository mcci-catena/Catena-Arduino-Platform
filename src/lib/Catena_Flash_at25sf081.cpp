/*

Module:  Catena_Flash_at25sf081.h

Function:
        class Catena_Flash_at25sf081

Copyright notice and License:
        See LICENSE file accompanying this project.

Author:
        Terry Moore, MCCI Corporation	May 2018

*/

#include <Catena_Flash_at25sf081.h>
#include <Catena_Log.h>

using namespace McciCatena;

/*

Name:	cFlash_AT25SF081::begin()

Function:
        Start operations for an at25sf081 object.

Definition:
        bool cFlash_AT25SF081::begin(
                SPIClass *pSpi,
                uint8_t ChipSelectPin
                );

Description:
        This function prepares this instance for doing I/O operations.
        It locates the device, probes it, and sets up the framework
        for communications.

Returns:
        true for success, false if any problems.

*/

bool cFlash_AT25SF081::begin(SPIClass *pSpi, uint8_t ChipSelectPin)
        {
        uint8_t ManufacturerId;
        uint16_t DeviceId;
        bool fResult;

        this->m_Settings = SPISettings { this->SPICLOCK(this->MAX_SCLK), MSBFIRST, SPI_MODE0 };

        if (pSpi == NULL)
                {
                // invalid parameter
                gLog.printf(gLog.kError, "pSpi is NULL\n");
                return false;
                }

        this->m_pSpi = pSpi;
        this->m_CS = ChipSelectPin;
        digitalWrite(ChipSelectPin, HIGH);
        pinMode(ChipSelectPin, OUTPUT);

        /* force out of deep-sleep mode, just in case */
        this->m_PowerDown = true;
        this->powerUp();

        /* reset flash chip */
        this->reset();

        /* Make sure we're actually connected */
        this->readId(&ManufacturerId, &DeviceId);
        fResult = true;
        if (ManufacturerId != this->MANUFACTURER_ID)
                {
                // invalid manufacturer id
                gLog.printf(
                        gLog.kError,
                        "Invalid flash manufacturer id=%#02x\n",
                        ManufacturerId
                        );
                fResult = false;
                }
        if (DeviceId != this->DEVICE_ID)
                {
                // invalid manufacturer id
                gLog.printf(
                        gLog.kError,
                        "Invalid flash device id=%#04x\n",
                        DeviceId
                        );
                fResult = false;
                }

        /* Everything seems to be properly initialised and connected */
        if (fResult)
                {
                this->m_Initialized = true;
                // this->powerDown();
                }

        return fResult;
        }


/*

Name:	cFlash_AT25SF081::reset

Function:
        Reset MX25V8035F chip

Definition:
        void cFlash_AT25SF081::reset(
                void
                );

Description:
        This function resets the MX25V8035F chip.

Returns:
        No explicit result.

*/

void cFlash_AT25SF081::reset(
        void
        )
        {
        // nothing to do.
        }

/*

Name:	cFlash_AT25SF081::readId

Function:
        Read AT25SF081 chip ID

Definition:
        void cFlash_AT25SF081::readId(
                uint8_t *pManufacturerId,
                uint16_t *pDeviceId
                );

Description:
        This function reads the AT25SF081 chip identification.

Returns:
        No explicit result.

*/

void cFlash_AT25SF081::readId(
        uint8_t *pManufacturerId,
        uint16_t *pDeviceId
        )
        {
        SPIClass * const pSpi = this->m_pSpi;
        uint8_t data[4];

        this->powerUp();

        data[0] = this->CMD_RDID;
        this->transfer(data, sizeof(data));

        if (pManufacturerId != NULL)
                *pManufacturerId = data[1];
        if (pDeviceId != NULL)
                *pDeviceId = (data[2] << 8) | data[3];
        }

/*

Name:	cFlash_AT25SF081::eraseChip

Function:
        Erase MX25V8035F chip

Definition:
        bool cFlash_AT25SF081::eraseChip(
                void
                );

Description:
        This function erases MX25V8035F chip.

Returns:
        true for succcess, false for failure.

*/

bool cFlash_AT25SF081::eraseChip(
        void
        )
        {
        uint8_t	status[2];

        this->powerUp();

        if (! this->writeEnable())
                return false;

        this->transfer(this->CMD_CE);
        return this->writeWait(Timing::tCHPE);
        }

/*

Name:	cFlash_AT25SF081::erase

Function:
        Erase common function

Definition:
        bool cFlash_AT25SF081::erase(
                uint32_t Address,
                uint8_t Command,
                Timing timeout
                );

Description:
        This function is common function to erases MX25V8035F flash.

Returns:
        No explicit result.

*/

bool cFlash_AT25SF081::erase(
        uint32_t Address,
        uint8_t Command,
        Timing Delay
        )
        {
        uint8_t	status[2];
        uint8_t data[4];

        this->powerUp();

        data[0] = Command;
        data[1] = (Address >> 16) & 0xFF;
        data[2] = (Address >> 8) & 0xFF;
        data[3] = Address & 0xFF;

        if (! this->writeEnable())
                return false;

        this->transfer(data, sizeof(data));

        return this->writeWait(Delay);
        }

/*

Name:	cFlash_AT25SF081::eraseSector

Function:
        Erase MX25V8035F sector (4KB)

Definition:
        bool cFlash_AT25SF081::eraseSector(
                uint32_t SectorAddress
                );

Description:
        This function erases a sector (4KB).

Returns:
        true for success, false otherwise.

*/

bool cFlash_AT25SF081::eraseSector(
        uint32_t SectorAddress
        )
        {
        return this->erase(SectorAddress, CMD_SE, Timing::tBE_4k);
        }

/*

Name:	cFlash_AT25SF081::eraseBlock32

Function:
        Erase MX25V8035F block (32KB)

Definition:
        bool cFlash_AT25SF081::eraseBlock32(
                uint32_t Block32Address
                );

Description:
        This function erases a block (32KB).

Returns:
        true for success, false for failure.

*/

bool cFlash_AT25SF081::eraseBlock32(
        uint32_t Block32Address
        )
        {
        return this->erase(Block32Address, CMD_BE_32K, Timing::tBE_32k);
        }

/*

Name:	cFlash_AT25SF081::eraseBlock64

Function:
        Erase MX25V8035F block (64KB)

Definition:
        bool cFlash_AT25SF081::eraseBlock64(
                uint32_t Block64Address
                );

Description:
        This function erases a block (64KB).

Returns:
        true for success, false for failure.

*/

bool cFlash_AT25SF081::eraseBlock64(
        uint32_t Block64Address
        )
        {
        this->erase(Block64Address, CMD_BE, Timing::tBE_64k);
        }

/*

Name:	cFlash_AT25SF081::setProtection

Function:
        Set protection registers of flash chip

Definition:
        bool cFlash_AT25SF081::setProtection(
                ProtectionBits protectionLevel
                );

Description:
        This function sets the protection bits for the chip.

Returns:
        true for success, false for failure

*/

bool cFlash_AT25SF081::setProtection(
        ProtectionBits protectionLevel
        )
        {
        uint8_t	status[2];
        uint8_t data[3];

        this->powerUp();

        data[0] = CMD_WRSR;
        data[1] = static_cast<uint16_t>(protectionLevel) & 0xFF;
        data[2] = static_cast<uint16_t>(protectionLevel) >> 8;

        if (! this->writeEnable())
                return false;

        this->transfer(data, sizeof(data));

        return this->writeWait(Timing::tWRSR);
        }

/*

Name:	cFlash_AT25SF081::read

Function:
        Read a buffer from the specified flash address

Definition:
        void cFlash_AT25SF081::read(
                uint32_t Address,
                uint8_t *pBuffer,
                size_t nBuffer
                );

Description:
        This function reads a buffer from the specified flash address.

Returns:
        No explicit result.

*/

void cFlash_AT25SF081::read(
        uint32_t Address,
        uint8_t *pBuffer,
        size_t nBuffer
        )
        {
        uint8_t data[4];

        this->powerUp();

        data[0] = CMD_READ;
        data[1] = (Address >> 16) & 0xFF;
        data[2] = (Address >> 8) & 0xFF;
        data[3] = Address & 0xFF;

        this->transferOutIn(
                data, sizeof(data),
                pBuffer, nBuffer
                );
        }

/*

Name:	cFlash_AT25SF081::programPage

Function:
        Program a buffer to the specified flash address

Definition:
        size_t cFlash_AT25SF081::programPage(
                uint32_t Address,
                const uint8_t *pBuffer,
                size_t nBuffer
                );

Description:
        This function programs a buffer to the specified flash address.

Returns:
        The number of bytes programmed at Address. Zero for failure.

*/

size_t cFlash_AT25SF081::programPage(
        uint32_t Address,
        const uint8_t *pBuffer,
        size_t nBuffer
        )
        {
        SPIClass * const pSpi = this->m_pSpi;
        uint8_t data[4];
        size_t	programSize;

        if (nBuffer == 0)
                return 0;

        data[0] = CMD_PP;
        data[1] = (Address >> 16) & 0xFF;
        data[2] = (Address >> 8) & 0xFF;
        data[3] = Address & 0xFF;

        programSize = PAGE_SIZE - data[3];
        if (programSize > nBuffer)
                programSize = nBuffer;

        this->powerUp();

        if (! this->writeEnable())
                {
                uint8_t sr1, sr2;
                this->readStatus(&sr1, &sr2);
                gLog.printf(
                        gLog.kError,
                        "** writeEnable for program page %#x failed: sr1=%#02x sr2=%#02x **\n",
                        Address, sr1, sr2
                        );
                return 0;
                }

        this->transferOutOut(
                data, sizeof(data),
                pBuffer, programSize
                );

        if (! this->writeWait(programSize <= 1 ? Timing::tBP : Timing::tPP))
                {
                uint8_t sr1, sr2;
                this->readStatus(&sr1, &sr2);
                gLog.printf(
                        gLog.kError,
                        "** writeWait for program page %#x failed tBegin=%u tEnd=%u delta=%u sr1=%#02x sr2=%#02x**\n",
                        Address, this->m_tBegin, this->m_tEnd, this->m_tEnd - this->m_tBegin,
                        sr1, sr2
                        );
                return 0;
                }

        return programSize;
        }

/*

Name:	cFlash_AT25SF081::program

Function:
        Program a buffer to the specified flash address

Definition:
        void cFlash_AT25SF081::program(
                uint32_t Address,
                uint8_t *pBuffer,
                size_t nBuffer
                );

Description:
        This function programs a buffer to the specified flash address.

Returns:
        No explicit result.

*/

bool cFlash_AT25SF081::program(
        uint32_t Address,
        const uint8_t *pBuffer,
        size_t nBuffer
        )
        {
        if (nBuffer == 0)
                return true;

        while (nBuffer > 0)
                {
                size_t	programSize;

                programSize = this->programPage(Address, pBuffer, nBuffer);
                if (programSize == 0)
                        return false;

                Address += programSize;
                pBuffer += programSize;
                nBuffer -= programSize;
                }

        return true;
        }

/*

Name:	cFlash_AT25SF081::powerDown

Function:
        Power down flash chip

Definition:
        void cFlash_AT25SF081::powerDown(
                void
                );

Description:
        This function puts flash chip deep power down mode.

Returns:
        No explicit result.

*/

void cFlash_AT25SF081::powerDown(
        void
        )
        {
        uint32_t uSec;

        this->transfer(CMD_DP);

        /* tDP == Max 1us */
        uSec = micros();
        this->m_PowerDown = true;
        while (! timeout_p(uSec, Timing::tEDPD))
                /*delay*/;
        }

/*

Name:	cFlash_AT25SF081::powerUp

Function:
        Power up flash chip

Definition:
        void cFlash_AT25SF081::powerUp(
                void
                );

Description:
        This function wakes up flash chip to stand-by mode,
        by sending .

Returns:
        No explicit result.

*/

void cFlash_AT25SF081::powerUp(
        void
        )
        {
        /* tDPDD == Min 30us */
        if (this->m_PowerDown)
                {
                uint32_t uSec;

                this->transfer(this->CMD_RES);

                /* tRDPD == Min 5us */
                uSec = micros();

                while (! timeout_p(uSec, Timing::tRDPD))
                        ;
                }
        }

/*

Name:	cFlash_AT25SF081::readStatus

Function:
        Read status register(s)

Definition:
        void cFlash_AT25SF081::readStatus(
                uint8_t *pSr1, uint8_t *pSr2
                );

Description:
        This function reads status register 1 and 2 into the associated
        output parameters. If either is null, the corresponding register
        read is skipped..

Returns:
        No explicit result.

*/


void cFlash_AT25SF081::readStatus(
        uint8_t *pSr1,
        uint8_t *pSr2
        )
        {
        uint8_t status[2];
        this->powerUp();

        if (pSr1 != nullptr)
                {
                status[0] = this->CMD_RDSR;
                this->transfer(status, sizeof(status));

                *pSr1 = status[1];
                }

        if (pSr2 != nullptr)
                {
                status[0] = this->CMD_RDSR2;
                this->transfer(status, sizeof(status));

                *pSr2 = status[1];
                }
        }
