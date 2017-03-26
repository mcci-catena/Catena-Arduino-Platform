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

/*

Name:	Catena4450::LoRaWAN::GetAbpProvisioningInfo()

Function:
	Get the ABP info (which is also what's saved after an OTAA Join)

Definition:
	public: virtual bool
		Catena4450::LoRaWAN::GetAbpProvisioningInfo(
		        Catena4450::LoRaWAN::AbpProvisioningInfo *pInfo
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
Catena4450::LoRaWAN::GetAbpProvisioningInfo(
        Catena4450::LoRaWAN::AbpProvisioningInfo *pInfo
        )
        {
        Catena4450 * const pCatena = this->m_pCatena;
	auto const pFram = pCatena->getFram();
	cFram::Cursor framNwkSKey(pFram),
		      framAppSKey(pFram),
		      framDevAddr(pFram),
		      framNwkID(pFram),
		      framFCntUp(pFram),
		      framFCntDown(pFram);
		      
	bool fResult;

	fResult = false;

	if (framNwkSKey.locate(cFramStorage::vItemDefs[cFramStorage::kNwkSKey]) &&
	    framAppSKey.locate(cFramStorage::vItemDefs[cFramStorage::kAppSKey]) &&
	    framDevAddr.locate(cFramStorage::vItemDefs[cFramStorage::kDevAddr]) &&
	    framNwkID.locate(cFramStorage::vItemDefs[cFramStorage::kNwkID]) &&
	    framFCntUp.locate(cFramStorage::vItemDefs[cFramStorage::kFCntUp]) &&
	    framFCntDown.locate(cFramStorage::vItemDefs[cFramStorage::kFCntDown]))
		fResult = true;

	if (! fResult)
		{
		Log.printf(Log.kError, "%s: failing\n", __FUNCTION__);

		if (pInfo != nullptr)
			memset(pInfo, 0, sizeof(pInfo));

		return false;
		}

	framNwkSKey.get(pInfo->NwkSKey, sizeof(pInfo->NwkSKey));
	framAppSKey.get(pInfo->AppSKey, sizeof(pInfo->AppSKey));
	framDevAddr.getuint32(pInfo->DevAddr);
	framNwkID.getuint32(pInfo->NwkID);
	framFCntUp.getuint32(pInfo->FCntUp);
	framFCntDown.getuint32(pInfo->FCntDown);

	return true;
	}

/*

Name:	Catena4450::LoRaWAN::GetOtaaProvisioningInfo()

Function:
	Fetch OTAA provisioning info from FRAM (if availalbe)

Definition:
	public: virtual bool
		Catena4450::LoRaWAN::GetOtaaProvisioningInfo(
		        Catena4450::LoRaWAN::OtaaProvisioningInfo *pInfo
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
	/* 
	|| we use 0 as the "none" indicator, because that's the default
	|| value when writing out the key.
	*/
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
        auto const pFram = pCatena->getFram();
        cFram::Cursor cursor(pFram, cFramStorage::kFCntUp);

        if (! cursor.create())
                {
        	Log.printf(Log.kError, "%s: can't save FCntUp: %u\n", __FUNCTION__, uFCntUp);
                }

        cursor.putuint32(uFCntUp);
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
