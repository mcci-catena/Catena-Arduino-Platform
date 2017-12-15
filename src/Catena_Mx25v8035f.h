/* Catena_Mx25v8035f.h	Mon Oct 30 2017 14:52:40 chwon */

/*

Module:  Catena_Mx25v8035f.h

Function:
	class Catena_Mx25v8035f

Version:
	V0.6.0	Mon Oct 30 2017 14:52:40 chwon	Edit level 1

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

*/

#ifndef _CATENA_MX25V8035F_H_		/* prevent multiple includes */
#define _CATENA_MX25V8035F_H_

#pragma once

#include <Arduino.h>
#include <SPI.h>

/* TODO: change these to enums and constepxrs in the McciCatena namespace */

/* MX25V8035F commands */
#define	MX25V8035F_CMD_READ		0x03u
#define	MX25V8035F_CMD_FAST_READ	0x0Bu
#define	MX25V8035F_CMD_PP		0x02u	/* Page Program */
#define	MX25V8035F_CMD_SE		0x20u	/* Sector Erase */
#define	MX25V8035F_CMD_BE_32K		0x52u	/* Block Erase 32KB */
#define	MX25V8035F_CMD_BE		0xD8u	/* Block Erase 64KB */
#define	MX25V8035F_CMD_CE		0x60u	/* Chip Erase */
#define	MX25V8035F_CMD_RDSFDP		0x5Au	/* Read SFDP */
#define	MX25V8035F_CMD_WREN		0x06u	/* Write Enable */
#define	MX25V8035F_CMD_WRDI		0x04u	/* Write Disable */
#define	MX25V8035F_CMD_RDSR		0x05u	/* Read Status Register */
#define	MX25V8035F_CMD_RDCR		0x15u	/* Read Configuration Register */
#define	MX25V8035F_CMD_WRSR		0x01u	/* Write Status Register */
#define	MX25V8035F_CMD_SUSPEND		0x75u	/* PGM/ERS Suspend */
#define	MX25V8035F_CMD_RESUME		0x7Au	/* PGM/ERS Resume */
#define	MX25V8035F_CMD_DP		0xB9u	/* Deep Power Down */
#define	MX25V8035F_CMD_SBL		0xC0u	/* Set Burst Length */
#define	MX25V8035F_CMD_RDID		0x9Fu	/* Read Identification */
#define	MX25V8035F_CMD_RES		0xABu	/* Read Electronic ID */
#define	MX25V8035F_CMD_REMS		0x90u	/* Read Electronic Manu & Dev ID */
#define	MX25V8035F_CMD_ENSO		0xB1u	/* Enter Secured OTP */
#define	MX25V8035F_CMD_EXSO		0xC1u	/* Exit Secured OTP */
#define	MX25V8035F_CMD_RDSCUR		0x2Bu	/* Read Security Register */
#define	MX25V8035F_CMD_WRSCUR		0x2Fu	/* Write Security Register */
#define	MX25V8035F_CMD_NOP		0x00u	/* No operation */
#define	MX25V8035F_CMD_RSTEN		0x66u	/* Reset Enable */
#define	MX25V8035F_CMD_RST		0x99u	/* Reset Memory */
#define	MX25V8035F_CMD_RRE		0xFFu	/* Release Read Enhanced */

/* MX25V8035F status bits */
#define	MX25V8035F_STS_WIP		(1u << 0) /* Write In Progress */
#define	MX25V8035F_STS_WEL		(1u << 1) /* Write Enable Latch */
#define	MX25V8035F_STS_BP0		(1u << 2) /* Level of protected block */
#define	MX25V8035F_STS_BP1		(1u << 3) /* Level of protected block */
#define	MX25V8035F_STS_BP2		(1u << 4) /* Level of protected block */
#define	MX25V8035F_STS_BP3		(1u << 5) /* Level of protected block */
#define	MX25V8035F_STS_BP_MASK		(15u << 2)
#define	MX25V8035F_STS_QE		(1u << 6) /* Quad Enable */
#define	MX25V8035F_STS_SRWD		(1u << 7) /* Status Regster Write Protect */

/* MX25V8035F configuration bits */
#define	MX25V8035F_CFG_TB		(1u << 3) /* 0=Top, 1=Bottom area protect */

/* MX25V8035F Security bits */
#define	MX25V8035F_SCUR_OTP		(1u << 0) /* 1=Factory lock (2nd 4Kb) */
#define	MX25V8035F_SCUR_LDSO		(1u << 1) /* 1=Lock-Down (1st 4Kb) */
#define	MX25V8035F_SCUR_PSB		(1u << 2) /* Program Suspend */
#define	MX25V8035F_SCUR_ESB		(1u << 3) /* Erase Suspend */
#define	MX25V8035F_SCUR_P_FAIL		(1u << 5) /* Program Failed */
#define	MX25V8035F_SCUR_E_FAIL		(1u << 6) /* Erase Failed */

#define	MX25V8035F_MANUFACTURER_ID	0xC2u
#define	MX25V8035F_DEVICE_ID		0x2314u

#define	MX25V8035F_PAGE_SIZE		(256)
#define	MX25V8035F_SECTOR_SIZE		(4 * 1024)
#define	MX25V8035F_PAGE_PER_SECTOR	(MX25V8035F_SECTOR_SIZE / MX25V8035F_PAGE_SIZE)
#define	MX25V8035F_BLOCK32_SIZE		(32 * 1024)
#define	MX25V8035F_SECTOR_PER_BLOCK32	(MX25V8035F_BLOCK32_SIZE / MX25V8035F_SECTOR_SIZE)
#define	MX25V8035F_PAGE_PER_BLOCK32	(MX25V8035F_BLOCK32_SIZE / MX25V8035F_PAGE_SIZE)
#define	MX25V8035F_BLOCK64_SIZE		(64 * 1024)
#define	MX25V8035F_SECTOR_PER_BLOCK64	(MX25V8035F_BLOCK64_SIZE / MX25V8035F_SECTOR_SIZE)
#define	MX25V8035F_PAGE_PER_BLOCK64	(MX25V8035F_BLOCK64_SIZE / MX25V8035F_PAGE_SIZE)

#define	MX25V8035F_MAX_SCLK		104000000u /* 104MHz */
#define	CATENA_MX25V8035F_SCLK		80000000u  /* 80MHz */

/* protection level values */
#define	CATENA_MX25V8035F_PL_NONE		0x00
#define	CATENA_MX25V8035F_PL_BLOCK_15		0x01	/* 1 block */
#define	CATENA_MX25V8035F_PL_BLOCK_14_15	0x02	/* 2 blocks */
#define	CATENA_MX25V8035F_PL_BLOCK_12_15	0x03	/* 4 blocks */
#define	CATENA_MX25V8035F_PL_BLOCK_8_15		0x04	/* 8 blocks */
#define	CATENA_MX25V8035F_PL_BLOCK_0		0x81	/* 1 blocks */
#define	CATENA_MX25V8035F_PL_BLOCK_0_1		0x82	/* 2 blocks */
#define	CATENA_MX25V8035F_PL_BLOCK_0_3		0x83	/* 4 blocks */
#define	CATENA_MX25V8035F_PL_BLOCK_0_7		0x84	/* 8 blocks */
#define	CATENA_MX25V8035F_PL_BLOCK_ALL		0x85	/* 16 blocks */

#define	CATENA_MX25V8035F_PAGE_BASE(a)		((a) & ~MX25V8035F_PAGE_SIZE)
#define	CATENA_MX25V8035F_SECTOR_BASE(a)	((a) & ~MX25V8035F_SECTOR_SIZE)
#define	CATENA_MX25V8035F_BLOCK32_BASE(a)	((a) & ~MX25V8035F_BLOCK32_SIZE)
#define	CATENA_MX25V8035F_BLOCK64_BASE(a)	((a) & ~MX25V8035F_BLOCK64_SIZE)

namespace McciCatena {

class Catena_Mx25v8035f
	{
public:
	Catena_Mx25v8035f(void);

	// set up and probe device
	boolean begin(SPIClass *pSpi, uint8_t ChipSelectPin = D19);
	void end(void);

	// reset chip
	void reset(void);

	// read id
	void readId(uint8_t *pManufacturerId, uint16_t *pDeviceId);

	// chip earse
	void eraseChip(void);

	// earse sector, block32 and block64
	void eraseSector(uint32_t SectorAddress);
	void eraseBlock32(uint32_t Block32Address);
	void eraseBlock64(uint32_t Block64Address);

	// set protection -- CATENA_MX25V8035F_PL_xxx
	void setProtection(uint8_t protectionLevel);

	// read a buffer
	void read(uint32_t Address, uint8_t *pBuffer, size_t nBuffer);

	// program a buffer
	void program(uint32_t Address, uint8_t *pBuffer, size_t nBuffer);
	size_t programPage(uint32_t Address, uint8_t *pBuffer, size_t nBuffer);

	// power management
	void powerDown(void);
	void powerUp(void);

private:
	boolean		m_Initialized;
	boolean		m_PowerDown;
	uint8_t		m_CS;
	SPIClass *	m_pSpi;

	void erase(uint32_t Address, uint8_t Command, uint32_t Delay);
	};

} // namespace McciCatena

/**** end of Catena_Mx25v8035f.h ****/
#endif /* _CATENA_MX25V8035F_H_ */
