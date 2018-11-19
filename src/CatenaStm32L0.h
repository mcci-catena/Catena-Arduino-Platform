/* CatenaStm32L0.h	Mon Nov 19 2018 12:11:24 chwon */

/*

Module:  CatenaStm32L0.h

Function:
	Class CatenaStm32L0

Version:
	V0.11.0	Mon Nov 19 2018 12:11:24 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2017-2018 by

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

   0.11.0  Mon Nov 19 2018 12:11:25  chwon
	Add Sleep() method override and add m_Rtc in the private context.

*/

#ifndef _CATENASTM32L0_H_		/* prevent multiple includes */
#define _CATENASTM32L0_H_

#pragma once

#ifndef _CATENASTM32_H_
# include "CatenaStm32.h"
#endif

#ifndef _CATENA_FRAM8K_H_
# include "Catena_Fram8k.h"
#endif

#include <Arduino_LoRaWAN_ttn.h>

namespace McciCatena {

class CatenaStm32L0 : public CatenaStm32
	{
public:
        using Super = CatenaStm32;

        CatenaStm32L0() {};
	virtual ~CatenaStm32L0() {};

	// forward reference
	class LoRaWAN;

	// all Stm32L0s put vbat on A7
	enum ANALOG_PINS
		{
		APIN_VBAT_SENSE = A3,
		APIN_VBUS_SENSE = A4,
		};

	enum DIGITAL_PINS
		{
		PIN_STATUS_LED = D13,
		PIN_SPI2_FLASH_SS = D19,
		PIN_SPI2_MOSI = D23,
		PIN_SPI2_MISO = D22,
		PIN_SPI2_SCK = D24,
		};

        // start the Stm32L0 level
	virtual bool begin(void);
	virtual const EUI64_buffer_t *GetSysEUI(void) override;
	virtual const CATENA_PLATFORM *GetPlatformForID(
					const UniqueID_buffer_t *pIdBuffer,
					EUI64_buffer_t *pSysEUI,
					uint32_t *pOperatingFlags
					) override;

	McciCatena::cFram8k *getFram() { return &this->m_Fram; };

	bool getBootCount(uint32_t &bootCount)
		{
		bootCount = this->m_BootCount;
		return true;
		};

	// read the current battery voltage, in engineering units
	float ReadVbat(void) const;
	float ReadVbus(void) const;

protected:
	// methods
	virtual void registerCommands(void);

        // instance data
        const CATENA_PLATFORM *m_pPlatform;

private:
	// the FRAM instance
	McciCatena::cFram8k		m_Fram;
        uint32_t			m_BootCount;

        // the known platforms
        static const CATENA_PLATFORM(* const vPlatforms[]);
        static const size_t nvPlatforms;

        // internal methods
        void savePlatform(
                const CATENA_PLATFORM &Platform,
                const EUI64_buffer_t *pSysEUI,
                const uint32_t *pOperatingFlags
                );
	};

class CatenaStm32L0::LoRaWAN : public Arduino_LoRaWAN_ttn,
                                 public McciCatena::cPollableObject
	{
public:
        using Super = Arduino_LoRaWAN_ttn;

	/*
	|| the constructor.
	*/
	LoRaWAN();
        LoRaWAN(const lmic_pinmap &pinmap) : Super(pinmap) {};

	/*
	|| the begin function loads data from the local
	|| platform's stable storage and initializes
	|| the connection.
	*/
	virtual bool begin(CatenaStm32L0 *pCatena);
	CatenaStm32L0 *getCatena() const { return this->m_pCatena; };

        virtual void poll() { this->Super::loop(); };

protected:
	/*
	|| we have to provide these for the lower level
	*/
        virtual ProvisioningStyle GetProvisioningStyle(void) override;
        virtual bool GetAbpProvisioningInfo(
        		Arduino_LoRaWAN::AbpProvisioningInfo *
        		) override;
        virtual bool GetOtaaProvisioningInfo(
        		Arduino_LoRaWAN::OtaaProvisioningInfo *
        		) override;
        virtual void NetSaveFCntUp(uint32_t uFCntUp) override;
        virtual void NetSaveFCntDown(uint32_t uFCntDown) override;
        virtual void NetSaveSessionInfo(
        		const SessionInfo &Info,
        		const uint8_t *pExtraInfo,
        		size_t nExtraInfo
        		) override;

	//
	// TODO(tmm@mcci.com) -- the following are not used but are always
	// hanging around even when we have better ways to do things.
	//
private:
	CatenaStm32L0		*m_pCatena;
	const CATENA_PLATFORM	*m_pPlatform;

	// initialize the commands
	bool addCommands();
	};

} // namespace McciCatena

/**** end of CatenaStm32L0.h ****/
#endif /* _CATENASTM32L0_H_ */
