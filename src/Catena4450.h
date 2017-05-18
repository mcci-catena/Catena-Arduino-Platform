/* Catena4450.h	Sun Mar 19 2017 15:50:15 tmm */

/*

Module:  Catena4450.h

Function:
	class Catena4450: CatenaBase Platform to represent a Catena 4450

Version:
	V0.5.0	Sun Mar 19 2017 15:50:15 tmm	Edit level 2

Copyright notice:
	This file copyright (C) 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	March 2017

Revision history:
   0.5.0  Sun Mar 12 2017 18:16:07  tmm
	Module created.

*/

#ifndef _CATENA4450_H_		/* prevent multiple includes */
#define _CATENA4450_H_

#pragma once

#ifndef _CATENAFEATHERM0LORA_H_
# include "CatenaFeatherM0LoRa.h"
#endif

#ifndef _CATENA_FRAM2K_H_
# include "Catena_Fram2k.h"
#endif

#include <Arduino_LoRaWAN_ttn.h>

class Catena4450 : public CatenaFeatherM0LoRa
	{
public:
        using Super = CatenaFeatherM0LoRa;

        // no specific constructor.
        Catena4450() {};

	// uses default destructor

	// neither copyable nor movable
	Catena4450(const Catena4450&) = delete;
	Catena4450& operator=(const Catena4450&) = delete;
	Catena4450(const Catena4450&&) = delete;
	Catena4450& operator=(const Catena4450&&) = delete;

	// LoRaWAN binding
	class LoRaWAN /* forward */;

	// methods
	virtual bool begin() override;
        virtual const EUI64_buffer_t *GetSysEUI(void) override;
	virtual const CATENA_PLATFORM *GetPlatformForID(
		const UniqueID_buffer_t *pIdBuffer,
		EUI64_buffer_t *pSysEUI,
		uint32_t *pOperatingFlags
		) override;

        McciCatena::cFram2k *getFram() { return &this->m_Fram; };

        bool getBootCount(uint32_t &bootCount)
                { 
                bootCount = this->m_BootCount;
                return true;
                };

protected:
        virtual void registerCommands(void);

private:
	// the FRAM instance
	McciCatena::cFram2k			m_Fram;
        uint32_t                                m_BootCount;

        // the known platforms
        static const CATENA_PLATFORM (* const vPlatforms[]);
        static const size_t nvPlatforms;

        // internal methods
        void savePlatform(
                const CATENA_PLATFORM &Platform,
                const EUI64_buffer_t *pSysEUI,
                const uint32_t *pOperatingFlags
                );
	};

/*
|| The LoRaWAN class for the Catena 4450. Assumes The Things Network
*/
class Catena4450::LoRaWAN : public CatenaFeatherM0LoRa::LoRaWAN
	{
public:
	using Super = CatenaFeatherM0LoRa::LoRaWAN;

	/*
	|| the constructor. We don't do anything at this level, the
	|| Super constructor does most of the owrk.
	*/
	LoRaWAN() {};
	LoRaWAN(const lmic_pinmap &pinmap) : Super(pinmap) {};

	bool begin(Catena4450 *pParent);
        Catena4450 *getCatena() const { return this->m_pCatena; };

protected:
        virtual ProvisioningStyle GetProvisioningStyle(void) override;
        virtual bool GetAbpProvisioningInfo(Arduino_LoRaWAN::AbpProvisioningInfo*) override;
        virtual bool GetOtaaProvisioningInfo(Arduino_LoRaWAN::OtaaProvisioningInfo*) override;
        virtual void NetSaveFCntUp(uint32_t uFCntUp) override;
        virtual void NetSaveFCntDown(uint32_t uFCntDown) override;
        virtual void NetSaveSessionInfo(const SessionInfo &Info, const uint8_t *pExtraInfo, size_t nExtraInfo) override;

private:
	Catena4450		*m_pCatena;

        // initialize the commands
        bool addCommands();
	};

/**** end of Catena4450.h ****/
#endif /* _CATENA4450_H_ */
