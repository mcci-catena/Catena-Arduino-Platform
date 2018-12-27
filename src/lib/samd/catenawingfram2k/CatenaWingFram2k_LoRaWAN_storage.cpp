/* CatenaWingFram2k_LoRaWAN_storage.cpp	Wed Dec 05 2018 14:31:54 chwon */

/*

Module:  CatenaWingFram2k_LoRaWAN_storage.cpp

Function:
	Interface from LoRaWAN to FRAM.

Version:
	V0.12.0	Wed Dec 05 2018 14:31:54 chwon	Edit level 2

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
   0.5.0  Fri Mar 17 2017 22:49:16  tmm
	Module created.

   0.12.0  Wed Dec 05 2018 14:31:54  chwon
	Use Catena provisioning and NetSave methods.

*/

#ifdef ARDUINO_ARCH_SAMD

#include "CatenaWingFram2k.h"

#include "Catena_Fram.h"
#include "Catena_Log.h"

using namespace McciCatena;

/*

Name:	CatenaWingFram2k::LoRaWAN::GetAbpProvisioningInfo()

Function:
	Get the ABP info (which is also what's saved after an OTAA Join)

Definition:
	public: virtual bool
		CatenaWingFram2k::LoRaWAN::GetAbpProvisioningInfo(
		        CatenaWingFram2k::LoRaWAN::AbpProvisioningInfo *pInfo
		        ) override;

Description:
	This routine fills in an ABP info table with saved FRAM data:

		NwkSkey (the network session key)
		AppSkey (the app session key)
		DevAddr (the assigned device address)
		NwId	(the assigned network ID)
		FCntUp	(the uplink frame count)
		FCntDown (the downlink frame count)

	(When provisioning a device for ABP, you'll want to reset the frame
	counts, as these are maintained on uplink/downlink)

Returns:
	true if the data was filled in, false if not.

*/

bool
CatenaWingFram2k::LoRaWAN::GetAbpProvisioningInfo(
        CatenaWingFram2k::LoRaWAN::AbpProvisioningInfo *pInfo
        )
        {
        CatenaWingFram2k * const pCatena = this->m_pCatena;

	return pCatena->GetAbpProvisioningInfo(pInfo);
	}

/*

Name:	CatenaWingFram2k::LoRaWAN::GetOtaaProvisioningInfo()

Function:
	Fetch OTAA provisioning info from FRAM (if availalbe)

Definition:
	public: virtual bool
		CatenaWingFram2k::LoRaWAN::GetOtaaProvisioningInfo(
		        CatenaWingFram2k::LoRaWAN::OtaaProvisioningInfo *pInfo
		        ) override;

Description:
	This routine fetches the OTAA provisioning info from FRAM if
	available, formatting it into *pInfo.  For this to work, FRAM
	must be initialized and the AppKey, AppEUI and DevEUI must be
	available.

	If pInfo is nullptr, the routine simply checks whether the info
	is availalbe.

Returns:
	This routine returns true if and only if the provisioning info
	is available.

*/

bool
CatenaWingFram2k::LoRaWAN::GetOtaaProvisioningInfo(
        CatenaWingFram2k::LoRaWAN::OtaaProvisioningInfo *pInfo
        )
        {
        CatenaWingFram2k * const pCatena = this->m_pCatena;

	return pCatena->GetOtaaProvisioningInfo(pInfo);
	}

CatenaWingFram2k::LoRaWAN::ProvisioningStyle
CatenaWingFram2k::LoRaWAN::GetProvisioningStyle(
	void
	)
	{
        CatenaWingFram2k * const pCatena = this->m_pCatena;

	return pCatena->GetProvisioningStyle();
	}

void
CatenaWingFram2k::LoRaWAN::NetSaveFCntUp(
	uint32_t uFCntUp
	)
	{
        CatenaWingFram2k * const pCatena = this->m_pCatena;

        pCatena->NetSaveFCntUp(uFCntUp);
	}

void
CatenaWingFram2k::LoRaWAN::NetSaveFCntDown(
	uint32_t uFCntDown
	)
	{
        CatenaWingFram2k * const pCatena = this->m_pCatena;

        pCatena->NetSaveFCntDown(uFCntDown);
        }

void 
CatenaWingFram2k::LoRaWAN::NetSaveSessionInfo(
	const SessionInfo &Info, 
	const uint8_t *pExtraInfo, 
	size_t nExtraInfo
	)
	{
        CatenaWingFram2k * const pCatena = this->m_pCatena;

        pCatena->NetSaveSessionInfo(Info, pExtraInfo, nExtraInfo);
	}

#endif // ARDUINO_ARCH_SAMD
