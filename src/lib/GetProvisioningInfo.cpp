/* GetProvisioningInfo.cpp	Tue Nov  1 2016 11:58:03 tmm */

/*

Module:  GetProvisioningInfo.cpp

Function:
	Catena4410::GetProvisioningInfo()

Version:
	V0.3.0	Tue Nov  1 2016 11:58:03 tmm	Edit level 1

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

*/

#include <Catena4410.h>

/****************************************************************************\
|
|		Manifest constants & typedefs.
|
\****************************************************************************/

#if 0
extern const Arduino_LoRaWAN::ProvisioningTable gk_LoRaWAN_Keys
        /* __attribute__((__weak__)) */;
#endif

static const Arduino_LoRaWAN::ProvisioningInfo
skProvisioningInfo[] =
    {
        {  // 00-02-CC-01-00-00-00-06
        Style:  Arduino_LoRaWAN::ProvisioningStyle::kOTAA,
        AbpInfo: {},
        OtaaInfo:
                {
                AppKey:  {0x29, 0xA6, 0xE3, 0xEF, 0x78, 0x4E, 0x46, 0x0D, 0xF8, 0xC8, 0xD5, 0xFD, 0x59, 0x41, 0x17, 0xF9},
                DevEUI: { 0x06, 0x00, 0x00, 0x00, 0x01, 0xCC, 0x02, 0x00 },
                AppEUI: { 0x52, 0x08, 0x00, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 },
                },
        },

        {  // 00-02-CC-01-00-00-00-0A
        Style:  Arduino_LoRaWAN::ProvisioningStyle::kOTAA,
	AbpInfo: {},
        OtaaInfo:
                {
                AppKey:  {0x29, 0xA6, 0xE3, 0xEF, 0x78, 0x4E, 0x46, 0x0D, 0xF8, 0xC8, 0xD5, 0xFD, 0x59, 0x41, 0x17, 0xF9},
                DevEUI: { 0x0A, 0x00, 0x00, 0x00, 0x01, 0xCC, 0x02, 0x00 },
                AppEUI: { 0x52, 0x08, 0x00, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 },
                },
        },
    };

const Arduino_LoRaWAN::ProvisioningTable
gk_LoRaWAN_Keys =
	{
	pInfo: skProvisioningInfo,
	nInfo: MCCIADK_LENOF(skProvisioningInfo)
	};

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
Catena4410::GetProvisioningInfo(
        void
        )
        {
        const Arduino_LoRaWAN::ProvisioningTable * const pTable =
                &gk_LoRaWAN_Keys;

        if (! pTable)
                return NULL;

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
