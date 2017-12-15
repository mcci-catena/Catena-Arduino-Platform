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

#include <Arduino_LoRaWAN_lmic.h>
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


bool
CatenaStm32L0::LoRaWAN::GetAbpProvisioningInfo(
        CatenaStm32L0::LoRaWAN::AbpProvisioningInfo *pInfo
        )
        {
        CatenaStm32L0 * const pCatena = this->m_pCatena;
        const ProvisioningInfo * const pInstance = pCatena->GetProvisioningInfo();

        if (! pInstance)
                ARDUINO_LORAWAN_PRINTF(
                        LogVerbose,
                        "%s: no provisioning info\n",
                        __func__
                        );

        if (! pInstance ||
            pInstance->Style != ProvisioningStyle::kABP)
                {
                if (pInfo)
                        {
                        // ensure consistent behavior
                        memset(pInfo, 0, sizeof(*pInfo));
                        }

                return false;
                }

        // got instance data
        if (pInfo)
                {
                *pInfo = pInstance->AbpInfo;
                }

        return true;
        }

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32L0_LoRaWAN_getabpprovisioninginfo.cpp ****/
