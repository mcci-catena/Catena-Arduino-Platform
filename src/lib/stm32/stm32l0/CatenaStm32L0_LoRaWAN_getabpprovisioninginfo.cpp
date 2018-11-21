/* CatenaStm32L0_LoRaWAN_getabpprovisioninginfo.cpp	Fri Oct 13 2017 15:19:30 chwon */

/*

Module:  CatenaStm32L0_LoRaWAN_getabpprovisioninginfo.cpp

Function:
	CatenaStm32L0::LoRaWAN::GetAbpProvisioningInfo()

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

#include <CatenaStm32L0.h>

#include <Catena_Fram.h>
#include <Catena_Log.h>

using namespace McciCatena;

/****************************************************************************\
|
|		Manifest constants & typedefs.
|
\****************************************************************************/


/****************************************************************************\
|
|	Read-only data.
|
\****************************************************************************/


/****************************************************************************\
|
|	VARIABLES:
|
\****************************************************************************/


/*

Name:	CatenaStm32L0::LoRaWAN::GetAbpProvisioningInfo()

Function:
	Get the ABP info (which is also what's saved after an OTAA Join)

Definition:
	public: virtual bool
		CatenaStm32L0::LoRaWAN::GetAbpProvisioningInfo(
		        CatenaStm32L0::LoRaWAN::AbpProvisioningInfo *pInfo
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
CatenaStm32L0::LoRaWAN::GetAbpProvisioningInfo(
        CatenaStm32L0::LoRaWAN::AbpProvisioningInfo *pInfo
        )
        {
        CatenaStm32L0 * const pCatena = this->m_pCatena;
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

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32L0_LoRaWAN_getabpprovisioninginfo.cpp ****/
