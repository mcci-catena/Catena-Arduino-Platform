/* CatenaStm32L0_LoRaWAN_getotaaprovisioninginfo.cpp	Wed Dec 05 2018 14:36:22 chwon */

/*

Module:  CatenaStm32L0_LoRaWAN_getotaaprovisioninginfo.cpp

Function:
	CatenaStm32L0::LoRaWAN::GetOtaaProvisioningInfo()

Version:
	V0.12.0	Wed Dec 05 2018 14:36:22 chwon	Edit level 2

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

   0.12.0  Wed Dec 05 2018 14:36:22  chwon
	Use Catena provisioning method.

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

	return pCatena->GetOtaaProvisioningInfo(pInfo);
	}

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32L0_LoRaWAN_getotaaprovisioninginfo.cpp ****/
