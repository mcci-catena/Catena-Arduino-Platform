/* Catena4450.h	Sun Mar 12 2017 18:16:07 tmm */

/*

Module:  Catena4450.h

Function:
	class Catena4450: CatenaBase Platform to represent a Catena 4450

Version:
	V0.5.0	Sun Mar 12 2017 18:16:07 tmm	Edit level 1

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
# include "Catena_Fram2K.h"
#endif

#include <Arduino_LoRaWAN_ttn.h>

class Catena4450 : public CatenaFeatherM0LoRa
	{
public:
	Catena4450() {};
	// default destructor

	// neither copyable nor movable
	Catena4450(const Catena4450&) = delete;
	Catena4450& operator=(const Catena4450&) = delete;
	Catena4450(const Catena4450&&) = delete;
	Catena4450& operator=(const Catena4450&&) = delete;

	// LoRaWAN binding
	class LoRaWAN /* forward */;

	// methods
	virtual bool begin();

protected:
	using Super = CatenaFeatherM0LoRa;

        class cSerialReady : public McciCatena::cStreamLineCollector::cStreamReady
	        {
        public:
	        virtual bool isReady() const
		        {
		        return (!! ::Serial);
		        }
	        };

private:
	// the FRAM instance
	McciCatena::Fram2k			m_Fram;

        // the callback object to use for commands (since we're on USB)
        cSerialReady                            m_SerialReady;

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

protected:
        virtual ProvisioningStyle GetProvisioningStyle(void);
        virtual bool GetAbpProvisioningInfo(Arduino_LoRaWAN::AbpProvisioningInfo*);
        virtual bool GetOtaaProvisioningInfo(Arduino_LoRaWAN::OtaaProvisioningInfo*);
        virtual void NetSaveFCntUp(uint32_t uFCntUp);
        virtual void NetSaveFCntDown(uint32_t uFCntDown);
        virtual void NetSaveSessionInfo(const SessionInfo &Info, const uint8_t *pExtraInfo, size_t nExtraInfo);

private:
	Catena4450		*m_pCatena;

	};

/**** end of Catena4450.h ****/
#endif /* _CATENA4450_H_ */
