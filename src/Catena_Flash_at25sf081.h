/*

Module:  Catena_Flash_at25sf081.h

Function:
        class Catena_Flash_at25sf081

Copyright notice and License:
        See LICENSE file accompanying this project.

Author:
        Terry Moore, MCCI Corporation	May 2018

*/

#ifndef _CATENA_FLASH_AT25SF081_H_		/* prevent multiple includes */
#define _CATENA_FLASH_AT25SF081_H_

#pragma once

#include <Arduino.h>
#include <SPI.h>

namespace McciCatena {

class cFlash_AT25SF081
        {
public:
        enum Commands : uint8_t
                {
                CMD_READ	= 0x03u,        /* regular read */
                CMD_FAST_READ	= 0x0Bu,        /* fast read */
                CMD_DUAL_READ   = 0x3Bu,        /* dual read */
                CMD_DUAL_IO_READ = 0xBBu,       /* dual read I/O */
                CMD_QUAD_READ   = 0x6Bu,        /* quad read */
                CMD_QUAD_IO_READ = 0xEBu,       /* quad read I/O */
                CMD_RRE		= 0xFFu,	/* Release Read Enhanced */
                CMD_PP		= 0x02u,	/* Page Program */
                CMD_SE		= 0x20u,	/* Sector Erase */
                CMD_BE_32K	= 0x52u,	/* Block Erase 32KB */
                CMD_BE		= 0xD8u,	/* Block Erase 64KB */
                CMD_CE		= 0x60u,	/* Chip Erase */
                CMD_WREN	= 0x06u,	/* Write Enable */
                CMD_WRDI	= 0x04u,	/* Write Disable */
                CMD_SECE        = 0x44u,        /* erase security registers */
                CMD_SECP        = 0x42u,        /* program security registers */
                CMD_READ_SEC    = 0x48u,        /* reasd security registers */
                CMD_RDSR	= 0x05u,	/* Read Status Register */
                CMD_RDSR2       = 0x35u,        /* Read Status Register 2 */
                CMD_WRSR	= 0x01u,	/* Write Status Register */
                CMD_WEVSR       = 0x50u,        /* Write Enable for Volatile Status Register */
                CMD_RDID	= 0x9Fu,	/* Read Identification */
                CMD_REMS	= 0x90u,	/* Read Electronic Manu & Dev ID */
                CMD_DP		= 0xB9u,	/* Deep Power Down */
                CMD_RES		= 0xABu,	/* Resume from deep power down */
                };

        enum StatusBits : uint8_t
                {
                STS_WIP		= (1u << 0),	/* Write In Progress */
                STS_WEL		= (1u << 1),	/* Write Enable Latch */
                STS_BP0		= (1u << 2),	/* Level of protected block */
                STS_BP1		= (1u << 3),	/* Level of protected block */
                STS_BP2		= (1u << 4),	/* Level of protected block */
                STS_TB		= (1u << 5),	/* Top/Bottom bit */
                STS_SEC		= (1u << 6),	/* Sector/Block protection  */
                STS_SRP0	= (1u << 7),	/* Status Register Write Protect bit 0 */
                };

        enum StatusBits2 : uint16_t
                {
                STS2_SRP1       = (1u << 0),    /* Status register write protect bit 1 */
                STS2_QE         = (1u << 1),    /* quad enable */
                STS2_RES2       = (1u << 2),    /* reserved bit 2 */
                STS2_LB1        = (1u << 3),    /* lock secureit register page 1 */
                STS2_LB2        = (1u << 4),    /* lock security register page 2 */
                STS2_LB3        = (1u << 5),    /* lock security register page 3 */
                STS2_CMP        = (1u << 6),    /* complement block protection */
                STS2_RES7       = (1u << 7),    /* reserved bit 7 */
                };

        enum ConfigBits : uint8_t
                {
                CFG_TB		= (1u << 3),	/* 0=Top, 1=Bottom area protect */
                };

        enum SecurityBits : uint8_t
                {
                SCUR_OTP	= (1u << 0),	/* 1=Factory lock (2nd 4Kb) */
                SCUR_LDSO	= (1u << 1),	/* 1=Lock-Down (1st 4Kb) */
                SCUR_PSB	= (1u << 2),	/* Program Suspend */
                SCUR_ESB	= (1u << 3),	/* Erase Suspend */
                SCUR_P_FAIL	= (1u << 5),	/* Program Failed */
                SCUR_E_FAIL	= (1u << 6),	/* Erase Failed */
                };

        enum class Timing: uint32_t
                {
                tEDPD           = 1,                    /* microseconds to enter deep power down */
                tRDPD           = 5,                    /* microseconds to exit deep power down */
                tBP             = 5,                    /* byte program time (us) */
                tPP             = 5000,                 /* page program time (us) */
                tBE_4k          = 300 * 1000,           /* 4k block erase time (us) */
                tBE_32k         = 1300 * 1000,          /* 32k block erase */
                tBE_64k         = 3000 * 1000,          /* 64k block erase */
                tCHPE           = 30 * 1000 * 1000,     /* chip erase time */
                tWRSR           = 15 * 1000,            /* write status register time */
                tSRP            = 2500,                 /* Security register program time */
                tSRE            = 15 * 1000,            /* Security register erase time */
                };

        enum : uint8_t
                {
                MANUFACTURER_ID = 0x1F
                };
        enum : uint16_t
                {
                DEVICE_ID = 0x8501
                };

        enum : uint32_t
                {
                PAGE_SIZE		= 256,
                SECTOR_SIZE		= 4 * 1024,
                PAGE_PER_SECTOR		= SECTOR_SIZE / PAGE_SIZE,
                BLOCK32_SIZE		= 32 * 1024,
                SECTOR_PER_BLOCK32	= BLOCK32_SIZE / SECTOR_SIZE,
                PAGE_PER_BLOCK32	= BLOCK32_SIZE / PAGE_SIZE,
                BLOCK64_SIZE		= 64 * 1024,
                SECTOR_PER_BLOCK64	= BLOCK64_SIZE / SECTOR_SIZE,
                PAGE_PER_BLOCK64	= BLOCK64_SIZE / PAGE_SIZE,
		DEVICE_SIZE_BYTES	= 1024 * 1024,
                };

        enum : uint32_t
                {
                MAX_SCLK		= 104 * 1000 * 1000,	/* 104 MHz */
                PLATFORM_MAX_SCLK       =
#                                       if defined(ARDUINO_ARCH_SAMD)
                                                12 * 1000 * 1000
#                                       elif defined(ARDUINO_ARCH_STM32)
                                                80 * 1000 * 1000
#                                       else
                                                MAX_SCLK
#                                       endif
                                        ,
                };

        enum ProtectionBits : uint16_t
                {
                PL_NONE			= 0x0000,		/* none */
                PL_BLOCK_15		= STS_BP0,		/* 1 block */
                PL_BLOCK_14_15		= STS_BP1,		/* 2 blocks */
                PL_BLOCK_12_15		= STS_BP1 | STS_BP0,	/* 4 blocks */
                PL_BLOCK_8_15		= STS_BP2,		/* 8 blocks */
                PL_BLOCK_0		= STS_TB | STS_BP1,	/* 1 block at bottom */
                PL_BLOCK_0_1		= STS_TB | STS_BP2,	/* 2 blocks */
                PL_BLOCK_0_3		= STS_TB | STS_BP1	/* 4 blocks */
                                                 | STS_BP2,
                PL_BLOCK_0_7		= STS_TB | STS_BP2,	/* 8 blocks */
                PL_BLOCK_ALL		= STS_BP2 | STS_BP0,	/* 16 blocks */
                PL_SEC_255		= STS_SEC | STS_BP0,	/* 1 sector */
                PL_SEC_254_255		= STS_SEC | STS_BP1,	/* 2 sector */
                PL_SEC_252_255		= STS_SEC | STS_BP1 | STS_BP0,	/* 4 sectors */
                PL_SEC_248_255		= STS_SEC | STS_BP2,		/* 8 sectors */
                PL_SEC_0		= STS_SEC |
                                          STS_TB | STS_BP1,	/* 1 sectors at bottom */
                PL_SEC_0_1		= STS_SEC |
                                          STS_TB | STS_BP2,	/* 2 sectors */
                PL_SEC_0_3		= STS_SEC |
                                          STS_TB | STS_BP1	/* 4 sectors */
                                                 | STS_BP2,
                PL_SEC_0_7		= STS_SEC |
                                          STS_TB | STS_BP2,	/* 8 sectors */
                PL_BLOCK_0_14		= (STS2_CMP << 8) | STS_BP0,		/* 1 block */
                PL_BLOCK_0_13		= (STS2_CMP << 8) | STS_BP1,		/* 2 blocks */
                PL_BLOCK_0_11		= (STS2_CMP << 8) | STS_BP1 | STS_BP0,	/* 4 blocks */
                PL_BLOCK_1_15		= (STS2_CMP << 8) | STS_TB | STS_BP1,	/* 1 block at bottom */
                PL_BLOCK_2_15		= (STS2_CMP << 8) | STS_TB | STS_BP2,	/* 2 blocks */
                PL_BLOCK_4_15		= (STS2_CMP << 8) | STS_TB | STS_BP1	/* 4 blocks */
                                                          | STS_BP2,
                PL_SEC_0_254		= (STS2_CMP << 8) | STS_SEC | STS_BP0,	/* 1 sector */
                PL_SEC_0_253		= (STS2_CMP << 8) | STS_SEC | STS_BP1,	/* 2 sector */
                PL_SEC_0_251		= (STS2_CMP << 8) | STS_SEC | STS_BP1 | STS_BP0,	/* 4 sectors */
                PL_SEC_0_247		= (STS2_CMP << 8) | STS_SEC | STS_BP2,		/* 8 sectors */
                PL_SEC_1_255		= (STS2_CMP << 8) | STS_SEC |
                                                   STS_TB | STS_BP1,	/* 1 sectors at bottom */
                PL_SEC_2_255		= (STS2_CMP << 8) | STS_SEC |
                                          STS_TB | STS_BP2,	/* 2 sectors */
                PL_SEC_4_255		= (STS2_CMP << 8) |
                                          STS_SEC |
                                          STS_TB | STS_BP1	/* 4 sectors */
                                                 | STS_BP2,
                PL_SEC_8_255		= (STS2_CMP << 8) |
                                          STS_SEC |
                                          STS_TB | STS_BP2,	/* 8 sectors */
                };

        static constexpr uint32_t PAGE_BASE(uint32_t a)
                {
                return a & ~PAGE_SIZE;
                }

        static constexpr uint32_t SECTOR_BASE(uint32_t a)
                {
                return a & ~SECTOR_SIZE;
                }

        static constexpr uint32_t BLOCK32_BASE(uint32_t a)
                {
                return a & ~BLOCK32_SIZE;
                }

        static constexpr uint32_t BLOCK64_BASE(uint32_t a)
                {
                return a & ~BLOCK64_SIZE;
                }

        static constexpr uint32_t SPICLOCK(uint32_t v)
                {
                return v > PLATFORM_MAX_SCLK ? PLATFORM_MAX_SCLK : v;
                }

public:
        cFlash_AT25SF081() {};

        // uses default destructor

        // neither copyable nor movable
        cFlash_AT25SF081(const cFlash_AT25SF081&) = delete;
        cFlash_AT25SF081& operator=(const cFlash_AT25SF081&) = delete;
        cFlash_AT25SF081(const cFlash_AT25SF081&&) = delete;
        cFlash_AT25SF081& operator=(const cFlash_AT25SF081&&) = delete;

        // set up and probe device
        bool begin(SPIClass *pSpi, uint8_t ChipSelectPin = 5);

        // stop using device; use begin() to restart.
        void end(void);

        // reset device
        void reset(void);

        // read ID
        void readId(uint8_t *pManufacturerId, uint16_t *pDeviceId);

        // read data
        void read(uint32_t Address, uint8_t *pBuffer, size_t nBuffer);

        // program
        bool program(uint32_t Address, const uint8_t *pBuffer, size_t nBuffer);

        // program a page
        size_t programPage(uint32_t Address, const uint8_t *pBuffer, size_t nBuffer);

        // internal powerDown
        void powerDown(void);

        // internal powerUp.
        void powerUp(void);

        // chip erase
        bool eraseChip(void);

        // erase a 4k sector
        bool eraseSector(uint32_t SectorAddress);

        // erase a 32k block
        bool eraseBlock32(uint32_t Block32Address);

        // erase a 64k block
        bool eraseBlock64(uint32_t Block64Address);

        // set protection
        bool setProtection(ProtectionBits protectionLevel);

        // read status registers
        void readStatus(uint8_t *pSr1, uint8_t *pSr2);

protected:
        void beginTransaction()
                {
                digitalWrite(this->m_CS, LOW);
                this->m_pSpi->beginTransaction(
                        this->m_Settings
                        );
                }
        void endTransaction(void)
                {
                digitalWrite(this->m_CS, HIGH);
                }
        uint8_t transfer(uint8_t bValue)
                {
                this->beginTransaction();
                uint8_t result = this->m_pSpi->transfer(bValue);
                this->endTransaction();

                return result;
                }
        void transfer(uint8_t *pBuffer, size_t nBuffer)
                {
                SPIClass * const pSpi = this->m_pSpi;

                this->beginTransaction();
                        for (; nBuffer > 0; ++pBuffer, --nBuffer)
                                {
                                *pBuffer = pSpi->transfer(*pBuffer);
                                }
                this->endTransaction();
                }
        void transferOutIn(
                const uint8_t *pOutBuffer, size_t nOutBuffer,
                uint8_t *pInBuffer, size_t nInBuffer
                )
                {
                SPIClass * const pSpi = this->m_pSpi;

                this->beginTransaction();
                        for (; nOutBuffer > 0; ++pOutBuffer, --nOutBuffer)
                                {
                                pSpi->transfer(*pOutBuffer);
                                }
                        for (; nInBuffer > 0; ++pInBuffer, --nInBuffer)
                                {
                                *pInBuffer = pSpi->transfer(0);
                                }
                this->endTransaction();
                }
        void transferOut(const uint8_t *pBuffer, size_t nBuffer)
                {
                SPIClass * const pSpi = this->m_pSpi;

                this->beginTransaction();
                        for (; nBuffer > 0; ++pBuffer, --nBuffer)
                                {
                                pSpi->transfer(*pBuffer);
                                }
                this->endTransaction();
                }
        void transferOutOut(
                const uint8_t *pBuffer, size_t nBuffer,
                const uint8_t *pBuffer2, size_t nBuffer2
                )
                {
                SPIClass * const pSpi = this->m_pSpi;

                this->beginTransaction();
                        for (; nBuffer > 0; ++pBuffer, --nBuffer)
                                {
                                pSpi->transfer(*pBuffer);
                                }
                        for (; nBuffer2 > 0; ++pBuffer2, --nBuffer2)
                                {
                                pSpi->transfer(*pBuffer2);
                                }
                this->endTransaction();
                }

        bool writeEnable()
                {
                uint8_t status[2];

                this->transfer(this->CMD_WREN);
                status[0] = this->CMD_RDSR;
                this->transfer(status, sizeof(status));
                return ((status[1] & this->STS_WEL) != 0);
                }

        bool timeout_p(uint32_t tBegin, Timing timeout)
                {
                this->m_tEnd = micros();
                return ((int32_t)(this->m_tEnd - tBegin) >= static_cast<int32_t>(timeout));
                }

        bool writeWait(Timing timeout)
                {
                uint8_t status[2];
                this->m_tBegin = micros();

                do	{
                        status[0] = this->CMD_RDSR;
                        this->transfer(status, sizeof(status));
                        if (! (status[1] & this->STS_WIP))
                                return true;
                        delay(1);
                        } while (! timeout_p(this->m_tBegin, timeout));

                return false;
                }

        uint32_t get_tBegin() { return this->m_tBegin; }
        uint32_t get_tEnd() { return this->m_tEnd; }

private:
        SPISettings	m_Settings;
        bool		m_Initialized;
        bool		m_PowerDown;
        uint8_t		m_CS;
        SPIClass *	m_pSpi;
        uint32_t	m_nesting;
        uint32_t        m_tBegin;
        uint32_t        m_tEnd;

        bool erase(uint32_t Address, uint8_t Command, Timing Delay);
        };

} // namespace McciCatena

/**** end of Catena_Flash_at25sf081.h ****/
#endif /* _CATENA_FLASH_AT25SF081__H_ */
