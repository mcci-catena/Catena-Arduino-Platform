/* catenafeatherm0_lorawan_getotaaprovisioninginfo.cpp	Mon Dec  5 2016 02:09:31 tmm */

/*

Module:  catenafeatherm0_lorawan_getotaaprovisioninginfo.cpp

Function:
	CatenaFeatherM0::LoRaWAN::GetOtaaProvisioningInfo()

Version:
	V0.4.0	Mon Dec  5 2016 02:09:31 tmm	Edit level 2

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
   0.3.0  Mon Oct 31 2016 18:04:04  tmm
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
CatenaFeatherM0::LoRaWAN::GetOtaaProvisioningInfo(
        CatenaFeatherM0::LoRaWAN::OtaaProvisioningInfo *pInfo
        )
        {
        CatenaFeatherM0 * const pCatena = this->m_pCatena;
        const ProvisioningInfo * const pInstance = pCatena->GetProvisioningInfo();

        if (! pInstance || 
            pInstance->Style != ProvisioningStyle::kOTAA)
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
                *pInfo = pInstance->OtaaInfo;
                }

        return true;
        }

#endif // ARDUINO_ARCH_SAMD
