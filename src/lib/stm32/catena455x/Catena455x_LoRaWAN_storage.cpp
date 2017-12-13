/* Catena455x_LoRaWAN_storage.cpp	Fri Oct 13 2017 15:19:30 chwon */

/*

Module:  Catena455x_LoRaWAN_storage.cpp

Function:
	Interface from LoRaWAN to FRAM.

Version:
	V0.6.0	Fri Oct 13 2017 15:19:30 chwon	Edit level 1

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
   0.6.0  Fri Oct 13 2017 15:19:30  chwon
	Module created.

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena455x.h"

#include "Catena_Fram.h"
#include "Catena_Log.h"

using namespace McciCatena;

/*

Name:	Catena455x::LoRaWAN::GetAbpProvisioningInfo()

Function:
	Get the ABP info (which is also what's saved after an OTAA Join)

Definition:
	public: virtual bool
		Catena455x::LoRaWAN::GetAbpProvisioningInfo(
		        Catena455x::LoRaWAN::AbpProvisioningInfo *pInfo
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
Catena455x::LoRaWAN::GetAbpProvisioningInfo(
        Catena455x::LoRaWAN::AbpProvisioningInfo *pInfo
        )
        {
        Catena455x * const pCatena = this->m_pCatena;
	auto const pFram = pCatena->getFram();
	cFram::Cursor framNwkSKey(pFram),
		      framAppSKey(pFram),
		      framDevAddr(pFram),
		      framNetID(pFram),
		      framFCntUp(pFram),
		      framFCntDown(pFram);

	bool fResult;

	fResult = false;

	if (framNwkSKey.locate(cFramStorage::vItemDefs[cFramStorage::kNwkSKey]) &&
	    framAppSKey.locate(cFramStorage::vItemDefs[cFramStorage::kAppSKey]) &&
	    framDevAddr.locate(cFramStorage::vItemDefs[cFramStorage::kDevAddr]) &&
	    framNetID.locate(cFramStorage::vItemDefs[cFramStorage::kNetID]) &&
	    framFCntUp.locate(cFramStorage::vItemDefs[cFramStorage::kFCntUp]) &&
	    framFCntDown.locate(cFramStorage::vItemDefs[cFramStorage::kFCntDown]))
		fResult = true;

	if (! fResult)
		{
		gLog.printf(gLog.kError, "%s: failing\n", __FUNCTION__);

		if (pInfo != nullptr)
			memset(pInfo, 0, sizeof(pInfo));

		return false;
		}

	framNwkSKey.get(pInfo->NwkSKey, sizeof(pInfo->NwkSKey));
	framAppSKey.get(pInfo->AppSKey, sizeof(pInfo->AppSKey));
	framDevAddr.getuint32(pInfo->DevAddr);
	framNetID.getuint32(pInfo->NetID);
	framFCntUp.getuint32(pInfo->FCntUp);
	framFCntDown.getuint32(pInfo->FCntDown);

	return true;
	}

/*

Name:	Catena455x::LoRaWAN::GetOtaaProvisioningInfo()

Function:
	Fetch OTAA provisioning info from FRAM (if availalbe)

Definition:
	public: virtual bool
		Catena455x::LoRaWAN::GetOtaaProvisioningInfo(
		        Catena455x::LoRaWAN::OtaaProvisioningInfo *pInfo
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
Catena455x::LoRaWAN::GetOtaaProvisioningInfo(
        Catena455x::LoRaWAN::OtaaProvisioningInfo *pInfo
        )
        {
        Catena455x * const pCatena = this->m_pCatena;
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
		gLog.printf(gLog.kError, "%s: failing\n", __FUNCTION__);

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

Catena455x::LoRaWAN::ProvisioningStyle
Catena455x::LoRaWAN::GetProvisioningStyle(
	void
	)
	{
        Catena455x * const pCatena = this->m_pCatena;
        cFram::Cursor framJoin(pCatena->getFram());

        if (! framJoin.locate(cFramStorage::vItemDefs[cFramStorage::kJoin]))
                {
        	gLog.printf(gLog.kError, "%s: failing\n", __FUNCTION__);

        	return ProvisioningStyle::kNone;
                }

        uint8_t uJoin;
        if (! framJoin.get(&uJoin, sizeof(uJoin)))
                {
                gLog.printf(gLog.kError, "%s: get() failed\n", __FUNCTION__);
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
                gLog.printf(gLog.kError, "%s: bad Join value: %u\n", __FUNCTION__, uJoin);
                return ProvisioningStyle::kNone;
                }
	}

void
Catena455x::LoRaWAN::NetSaveFCntUp(
	uint32_t uFCntUp
	)
	{
        Catena455x * const pCatena = this->m_pCatena;
        auto const pFram = pCatena->getFram();

        pFram->saveField(cFramStorage::kFCntUp, uFCntUp);
	}

void
Catena455x::LoRaWAN::NetSaveFCntDown(
	uint32_t uFCntDown
	)
	{
        Catena455x * const pCatena = this->m_pCatena;
        auto const pFram = pCatena->getFram();

        pFram->saveField(cFramStorage::kFCntDown, uFCntDown);
        }

void
Catena455x::LoRaWAN::NetSaveSessionInfo(
	const SessionInfo &Info,
	const uint8_t *pExtraInfo,
	size_t nExtraInfo
	)
	{
        Catena455x * const pCatena = this->m_pCatena;
	auto const pFram = pCatena->getFram();

        pFram->saveField(cFramStorage::kNetID,   Info.V1.NetID);
        pFram->saveField(cFramStorage::kDevAddr, Info.V1.DevAddr);
        pFram->saveField(cFramStorage::kNwkSKey, Info.V1.NwkSKey);
        pFram->saveField(cFramStorage::kAppSKey, Info.V1.AppSKey);
        pFram->saveField(cFramStorage::kFCntUp,  Info.V1.FCntUp);
        pFram->saveField(cFramStorage::kFCntDown, Info.V1.FCntDown);

        gLog.printf(
                gLog.kAlways,
                "NwkID:   %08x   "
                "DevAddr: %08x\n",
                Info.V1.NetID,
                Info.V1.DevAddr
                );
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Catena455x_LoRaWAN_storage.cpp ****/
