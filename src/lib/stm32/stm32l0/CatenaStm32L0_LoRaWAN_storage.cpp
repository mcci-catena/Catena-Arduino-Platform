/* CatenaStm32L0_LoRaWAN_storage.cpp	Fri Oct 13 2017 15:19:30 chwon */

/*

Module:  CatenaStm32L0_LoRaWAN_storage.cpp

Function:
	Interface from LoRaWAN to FRAM.

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

void
CatenaStm32L0::LoRaWAN::NetSaveFCntUp(
	uint32_t uFCntUp
	)
	{
        CatenaStm32L0 * const pCatena = this->m_pCatena;
        auto const pFram = pCatena->getFram();

        pFram->saveField(cFramStorage::kFCntUp, uFCntUp);
	}

void
CatenaStm32L0::LoRaWAN::NetSaveFCntDown(
	uint32_t uFCntDown
	)
	{
        CatenaStm32L0 * const pCatena = this->m_pCatena;
        auto const pFram = pCatena->getFram();

        pFram->saveField(cFramStorage::kFCntDown, uFCntDown);
        }

void
CatenaStm32L0::LoRaWAN::NetSaveSessionInfo(
	const SessionInfo &Info,
	const uint8_t *pExtraInfo,
	size_t nExtraInfo
	)
	{
        CatenaStm32L0 * const pCatena = this->m_pCatena;
	auto const pFram = pCatena->getFram();

        pFram->saveField(cFramStorage::kNetID,   Info.V1.NetID);
        pFram->saveField(cFramStorage::kDevAddr, Info.V1.DevAddr);
        pFram->saveField(cFramStorage::kNwkSKey, Info.V1.NwkSKey);
        pFram->saveField(cFramStorage::kAppSKey, Info.V1.AppSKey);
        pFram->saveField(cFramStorage::kFCntUp,  Info.V1.FCntUp);
        pFram->saveField(cFramStorage::kFCntDown, Info.V1.FCntDown);

        gLog.printf(
                gLog.kAlways,
                "NwkID:   %08x   "
                "DevAddr: %08x\n",
                Info.V1.NetID,
                Info.V1.DevAddr
                );
	}

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32L0_LoRaWAN_storage.cpp ****/
