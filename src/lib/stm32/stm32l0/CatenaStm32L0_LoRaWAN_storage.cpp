/* CatenaStm32L0_LoRaWAN_storage.cpp	Wed Dec 05 2018 14:31:03 chwon */

/*

Module:  CatenaStm32L0_LoRaWAN_storage.cpp

Function:
	Interface from LoRaWAN to FRAM.

Version:
	V0.12.0	Wed Dec 05 2018 14:31:03 chwon	Edit level 2

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

   0.12.0  Wed Dec 05 2018 14:31:03  chwon
	Use Catena NetSave methods.

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

        pCatena->NetSaveFCntUp(uFCntUp);
	}

void
CatenaStm32L0::LoRaWAN::NetSaveFCntDown(
	uint32_t uFCntDown
	)
	{
        CatenaStm32L0 * const pCatena = this->m_pCatena;

        pCatena->NetSaveFCntDown(uFCntDown);
        }

void
CatenaStm32L0::LoRaWAN::NetSaveSessionInfo(
	const SessionInfo &Info,
	const uint8_t *pExtraInfo,
	size_t nExtraInfo
	)
	{
        CatenaStm32L0 * const pCatena = this->m_pCatena;

	pCatena->NetSaveSessionInfo(Info, pExtraInfo, nExtraInfo);
	}

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32L0_LoRaWAN_storage.cpp ****/
