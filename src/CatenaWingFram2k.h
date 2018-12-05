/* CatenaWingFram2k.h	Wed Dec 05 2018 14:15:16 chwon */

/*

Module:  CatenaWingFram2k.h

Function:
	class CatenaWingFram2k: CatenaBase Platform to represent a Catena
        Feather Wing with 2k FRAM and the DIO1>D6 loopback.

Version:
	V0.12.0	Wed Dec 05 2018 14:15:17 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2017-2018 by

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

   0.12.0  Wed Dec 05 2018 14:15:17  chwon
	Use CatenaSamd21 super class instead of CatenaFeatherM0LoRa class.
	Remove GetSysEUI() and GetPlatformForID() methods override.
	Add ReadVbat() and add getFRAM() method override.

*/

#ifndef _CATENAWINGFRAM2K_H_		/* prevent multiple includes */
#define _CATENAWINGFRAM2K_H_

#pragma once

#ifndef _CATENASAMD21_H_
# include "CatenaSamd21.h"
#endif

#ifndef _CATENA_FRAM2K_H_
# include "Catena_Fram2k.h"
#endif

#include <Arduino_LoRaWAN_ttn.h>

namespace McciCatena {

class CatenaWingFram2k : public CatenaSamd21
	{
public:
        using Super = CatenaSamd21;

        // no specific constructor.
        CatenaWingFram2k() {};

	// uses default destructor

	// neither copyable nor movable
	CatenaWingFram2k(const CatenaWingFram2k&) = delete;
	CatenaWingFram2k& operator=(const CatenaWingFram2k&) = delete;
	CatenaWingFram2k(const CatenaWingFram2k&&) = delete;
	CatenaWingFram2k& operator=(const CatenaWingFram2k&&) = delete;

	// LoRaWAN binding
	class LoRaWAN /* forward */;

	// all FeatherM0s put vbat on A7
	enum ANALOG_PINS
		{
		APIN_VBAT_SENSE = A7,
		};

	enum DIGITAL_PINS
		{
		PIN_STATUS_LED = 13,
		};

	// methods
	virtual bool begin() override;

        virtual McciCatena::cFram *getFram(void) override
        	{
        	return &this->m_Fram;
        	};

        bool getBootCount(uint32_t &bootCount)
                { 
                bootCount = this->m_BootCount;
                return true;
                };

	// read the current battery voltage, in engineering units
	float ReadVbat(void) const;

protected:
        virtual void registerCommands(void);

private:
	// the FRAM instance
	McciCatena::cFram2k			m_Fram;
        uint32_t                                m_BootCount;
	};

/*
|| The LoRaWAN class for the Catena Wings. Needed because we have a
|| specific storage implementation and pin table.
*/
class CatenaWingFram2k::LoRaWAN : public Arduino_LoRaWAN_ttn,
                                  public McciCatena::cPollableObject
	{
public:
	using Super = Arduino_LoRaWAN_ttn;

	/*
	|| the constructor. We don't do anything at this level, the
	|| Super constructor does most of the owrk.
	*/
	LoRaWAN() {};

	/* start operations: this starts up the LMIC */
	bool begin(CatenaWingFram2k *pParent);

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

private:
	CatenaWingFram2k	*m_pCatena;
	};

} /* namespace McciCatena */

/**** end of CatenaWingFram2k.h ****/
#endif /* _CATENAWINGFRAM2K_H_ */
