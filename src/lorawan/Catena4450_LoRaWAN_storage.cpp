/* Catena4450_LoRaWAN_storage.cpp	Fri Mar 17 2017 22:49:16 tmm */

/*

Module:  Catena4450_LoRaWAN_storage.cpp

Function:
	Interface from LoRaWAN to FRAM.

Version:
	V0.5.0	Fri Mar 17 2017 22:49:16 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	March 2017

Revision history:
   0.5.0  Fri Mar 17 2017 22:49:16  tmm
	Module created.

*/

#include <Catena4450.h>

bool
Catena4450::LoRaWAN::GetAbpProvisioningInfo(
        Catena4450::LoRaWAN::AbpProvisioningInfo *pInfo
        )
        {
        Catena4450 * const pCatena = this->m_pCatena;

	pCatena->SafePrintf("%s: failing\n", __FUNCTION__);

	if (pInfo != nullptr)
		memset(pInfo, 0, sizeof(pInfo));

	return false;
	}

bool
Catena4450::LoRaWAN::GetOtaaProvisioningInfo(
        Catena4450::LoRaWAN::OtaaProvisioningInfo *pInfo
        )
        {
        Catena4450 * const pCatena = this->m_pCatena;

	pCatena->SafePrintf("%s: failing\n", __FUNCTION__);

	if (pInfo != nullptr)
		memset(pInfo, 0, sizeof(pInfo));

	return false;
	}

Catena4450::LoRaWAN::ProvisioningStyle
Catena4450::LoRaWAN::GetProvisioningStyle(
	void
	)
	{
        Catena4450 * const pCatena = this->m_pCatena;
	
	pCatena->SafePrintf("%s: failing\n", __FUNCTION__);

	return ProvisioningStyle::kNone;
	}

void
Catena4450::LoRaWAN::NetSaveFCntUp(
	uint32_t uFCntUp
	)
	{
        Catena4450 * const pCatena = this->m_pCatena;
	
	pCatena->SafePrintf("%s: FCntUp: %u\n", __FUNCTION__, uFCntUp);
	}

void
Catena4450::LoRaWAN::NetSaveFCntDown(
	uint32_t uFCntDown
	)
	{
        Catena4450 * const pCatena = this->m_pCatena;
	
	pCatena->SafePrintf("%s: FCntDown: %u\n", __FUNCTION__, uFCntDown);
	}

void 
Catena4450::LoRaWAN::NetSaveSessionInfo(
	const SessionInfo &Info, 
	const uint8_t *pExtraInfo, 
	size_t nExtraInfo
	)
	{
        Catena4450 * const pCatena = this->m_pCatena;
	
	pCatena->SafePrintf("%s: ignoring\n", __FUNCTION__);
	}
