/* catenafeatherm0_lorawan_getabpprovisioninginfo.cpp	Mon Dec  5 2016 02:08:45 tmm */

/*

Module:  catenafeatherm0_lorawan_getabpprovisioninginfo.cpp

Function:
	CatenaFeatherM0::LoRaWAN::GetAbpProvisioningInfo()

Version:
	V0.4.0	Mon Dec  5 2016 02:08:45 tmm	Edit level 2

Copyright notice:
	This file copyright (C) 2016 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	October 2016

Revision history:
   0.3.0  Mon Oct 31 2016 17:59:43  tmm
	Module created.

*/

#ifdef ARDUINO_ARCH_SAMD

#include <CatenaFeatherM0.h>

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
CatenaFeatherM0::LoRaWAN::GetAbpProvisioningInfo(
        CatenaFeatherM0::LoRaWAN::AbpProvisioningInfo *pInfo
        )
        {
        CatenaFeatherM0 * const pCatena = this->m_pCatena;
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

#endif // ARDUINO_ARCH_SAMD
