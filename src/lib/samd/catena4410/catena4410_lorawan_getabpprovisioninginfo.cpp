/* catena4410_lorawan_getabpprovisioninginfo.cpp	Mon Oct 31 2016 17:59:43 tmm */

/*

Module:  catena4410_lorawan_getabpprovisioninginfo.cpp

Function:
	Catena4410::LoRaWAN::GetAbpProvisioningInfo()

Version:
	V0.3.0	Mon Oct 31 2016 17:59:43 tmm	Edit level 1

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

#include <Catena4410.h>

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
Catena4410::LoRaWAN::GetAbpProvisioningInfo(
        Catena4410::LoRaWAN::AbpProvisioningInfo *pInfo
        )
        {
        Catena4410 * const pCatena = this->m_pCatena4410;
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
