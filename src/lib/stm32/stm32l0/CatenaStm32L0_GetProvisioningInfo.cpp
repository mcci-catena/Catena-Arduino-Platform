/* CatenaStm32L0_GetProvisioningInfo.cpp	Wed Dec 13 2017 17:07:07 tmm */

/*

Module:  CatenaStm32L0_GetProvisioningInfo.cpp

Function:
	CatenaStm32L0::GetProvisioningInfo()

Version:
	V0.7.0	Wed Dec 13 2017 17:07:07 tmm	Edit level 2

Copyright notice:
	This file copyright (C) 2016 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	November 2016

Revision history:
   0.3.0  Tue Nov  1 2016 11:58:03  tmm
	Module created.
	
   0.7.0  Wed Dec 13 2017 17:07:07   tmm
	Refactoring.

*/

#ifdef ARDUINO_ARCH_STM32

#include "CatenaStm32L0.h"

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

const Arduino_LoRaWAN::ProvisioningInfo *
CatenaStm32L0::GetProvisioningInfo(
        void
        )
        {
        const Arduino_LoRaWAN::ProvisioningTable * const pTable = this->GetLoRaWANkeys();

        const EUI64_buffer_t * const pSysEUI = this->GetSysEUI();

        if (pSysEUI == NULL)
                return NULL;

        unsigned i;
        const Arduino_LoRaWAN::ProvisioningInfo *pInfo;

        for (i = pTable->nInfo, pInfo = pTable->pInfo; i > 0; ++pInfo, --i)
                {
                /* 
                || even for ABP, we use the DevEUI to match up -- but it's 
                || reversed. All Catenas have DevEUIs, so this is really not
                || a sacrifice.
                */
                const uint8_t *p;
                const uint8_t *q;
                unsigned j;

                for (j = sizeof(*pSysEUI), 
                       p = &pSysEUI->b[0], q = &pInfo->OtaaInfo.DevEUI[j - 1]; 
                     j > 0; 
                     --j, ++p, --q
                     )
                        {
                        if (*p != *q)
                                break;
                        }

                if (j == 0)
                        /* this one matched */
                        return pInfo;
                }

        /* nothing matched */
        return NULL;
        }

#endif // ARDUINO_ARCH_STM32
