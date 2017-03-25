/* Catena4450_LoRaWAN_storage.cpp	Fri Mar 17 2017 22:49:16 tmm */

/*

Module:  Catena4450_LoRaWAN_storage.cpp

Function:
	Interface from LoRaWAN to FRAM.

Version:
	V0.5.0	Fri Mar 17 2017 22:49:16 tmm	Edit level 1

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
   0.5.0  Fri Mar 17 2017 22:49:16  tmm
	Module created.

*/

#include "Catena4450.h"

#include "Catena_Fram.h"
#include "Catena_Log.h"

using namespace McciCatena;

bool
Catena4450::LoRaWAN::GetAbpProvisioningInfo(
        Catena4450::LoRaWAN::AbpProvisioningInfo *pInfo
        )
        {
        Catena4450 * const pCatena = this->m_pCatena;

	Log.printf(Log.kError, "%s: failing\n", __FUNCTION__);

	if (pInfo != nullptr)
		memset(pInfo, 0, sizeof(pInfo));

	return false;
	}

/*

Name:	Catena4450::LoRaWAN::GetOtaaProvisioningInfo()

Function:
	Fetch OTAA provisioning info from FRAM (if availalbe)

Definition:
	public: virtual bool
		Catena4450::LoRaWAN::GetOtaaProvisioningInfo(
		        Catena4450::LoRaWAN::OtaaProvisioningInfo *pInfo
		        );

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
Catena4450::LoRaWAN::GetOtaaProvisioningInfo(
        Catena4450::LoRaWAN::OtaaProvisioningInfo *pInfo
        )
        {
        Catena4450 * const pCatena = this->m_pCatena;
	cFram::Cursor framAppEUI(pCatena->getFram()), 
		      framDevEUI(pCatena->getFram()), 
		      framAppKey(pCatena->getFram());
	bool fResult;

	fResult = false;

	if (framAppEUI.locate(cFramStorage::vItemDefs[cFramStorage::kAppEUI]) &&
	    framDevEUI.locate(cFramStorage::vItemDefs[cFramStorage::kDevEUI]) &&
	    framAppKey.locate(cFramStorage::vItemDefs[cFramStorage::kAppKey]))
		fResult = true;

	if (! fResult)
		{
		Log.printf(Log.kError, "%s: failing\n", __FUNCTION__);

		if (pInfo != nullptr)
			memset(pInfo, 0, sizeof(pInfo));

		return false;
		}

	if (pInfo == nullptr)
		return true;

	/* copy the data */
	framAppKey.get(pInfo->AppKey, sizeof(pInfo->AppKey));
	framDevEUI.get(pInfo->DevEUI, sizeof(pInfo->DevEUI));
	framAppEUI.get(pInfo->AppEUI, sizeof(pInfo->AppEUI));

	return true;
	}

Catena4450::LoRaWAN::ProvisioningStyle
Catena4450::LoRaWAN::GetProvisioningStyle(
	void
	)
	{
        Catena4450 * const pCatena = this->m_pCatena;
        cFram::Cursor framJoin(pCatena->getFram());

        if (! framJoin.locate(cFramStorage::vItemDefs[cFramStorage::kJoin]))
                {
        	Log.printf(Log.kError, "%s: failing\n", __FUNCTION__);

        	return ProvisioningStyle::kNone;
                }

        uint8_t uJoin;
        if (! framJoin.get(&uJoin, sizeof(uJoin)))
                {
                Log.printf(Log.kError, "%s: get() failed\n", __FUNCTION__);
                return ProvisioningStyle::kNone;
                }

        switch (uJoin)
                {
        case 0:
                return ProvisioningStyle::kNone;

        case 1:
                return ProvisioningStyle::kOTAA;

        case 2:
                return ProvisioningStyle::kABP;

        default:
                Log.printf(Log.kError, "%s: bad Join value: %u\n", __FUNCTION__, uJoin);
                return ProvisioningStyle::kNone;
                }
	}

void
Catena4450::LoRaWAN::NetSaveFCntUp(
	uint32_t uFCntUp
	)
	{
        Catena4450 * const pCatena = this->m_pCatena;
	
	pCatena->SafePrintf("%s: FCntUp: %u\n", __FUNCTION__, uFCntUp);
	}

void
Catena4450::LoRaWAN::NetSaveFCntDown(
	uint32_t uFCntDown
	)
	{
        Catena4450 * const pCatena = this->m_pCatena;
	
	pCatena->SafePrintf("%s: FCntDown: %u\n", __FUNCTION__, uFCntDown);
	}

void 
Catena4450::LoRaWAN::NetSaveSessionInfo(
	const SessionInfo &Info, 
	const uint8_t *pExtraInfo, 
	size_t nExtraInfo
	)
	{
        Catena4450 * const pCatena = this->m_pCatena;
	
	pCatena->SafePrintf("%s: ignoring\n", __FUNCTION__);
	}
