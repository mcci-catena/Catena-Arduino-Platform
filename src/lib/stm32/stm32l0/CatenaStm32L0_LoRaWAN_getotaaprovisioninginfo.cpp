/* CatenaStm32L0_LoRaWAN_getotaaprovisioninginfo.cpp	Fri Oct 13 2017 15:19:30 chwon */

/*

Module:  CatenaStm32L0_LoRaWAN_getotaaprovisioninginfo.cpp

Function:
	CatenaStm32L0::LoRaWAN::GetOtaaProvisioningInfo()

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

Name:	CatenaStm32L0::LoRaWAN::GetOtaaProvisioningInfo()

Function:
	Fetch OTAA provisioning info from FRAM (if availalbe)

Definition:
	public: virtual bool
		CatenaStm32L0::LoRaWAN::GetOtaaProvisioningInfo(
		        CatenaStm32L0::LoRaWAN::OtaaProvisioningInfo *pInfo
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
CatenaStm32L0::LoRaWAN::GetOtaaProvisioningInfo(
        CatenaStm32L0::LoRaWAN::OtaaProvisioningInfo *pInfo
        )
        {
        CatenaStm32L0 * const pCatena = this->m_pCatena;
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

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32L0_LoRaWAN_getotaaprovisioninginfo.cpp ****/
