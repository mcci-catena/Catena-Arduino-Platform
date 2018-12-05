/* CatenaBase_NetSave.cpp	Mon Dec 03 2018 13:21:28 chwon */

/*

Module:  CatenaBase_NetSave.cpp

Function:
	Interface from LoRaWAN to FRAM.

Version:
	V0.12.0	Mon Dec 03 2018 13:21:28 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	December 2018

Revision history:
   0.12.0  Mon Dec 03 2018 13:21:28  chwon
	Module created.

*/

#include <CatenaBase.h>

#include <Catena_Fram.h>
#include <Catena_Log.h>

using namespace McciCatena;

void
CatenaBase::NetSaveFCntUp(
	uint32_t uFCntUp
	)
	{
	auto const pFram = this->getFram();

	if (pFram != nullptr)
		pFram->saveField(cFramStorage::kFCntUp, uFCntUp);
	}

void
CatenaBase::NetSaveFCntDown(
	uint32_t uFCntDown
	)
	{
	auto const pFram = this->getFram();

	if (pFram != nullptr)
		pFram->saveField(cFramStorage::kFCntDown, uFCntDown);
	}

void
CatenaBase::NetSaveSessionInfo(
	const Arduino_LoRaWAN::SessionInfo &Info,
	const uint8_t *pExtraInfo,
	size_t nExtraInfo
	)
	{
	auto const pFram = this->getFram();

	if (pFram != nullptr)
		{
		pFram->saveField(cFramStorage::kNetID,   Info.V1.NetID);
		pFram->saveField(cFramStorage::kDevAddr, Info.V1.DevAddr);
		pFram->saveField(cFramStorage::kNwkSKey, Info.V1.NwkSKey);
		pFram->saveField(cFramStorage::kAppSKey, Info.V1.AppSKey);
		pFram->saveField(cFramStorage::kFCntUp,  Info.V1.FCntUp);
		pFram->saveField(cFramStorage::kFCntDown, Info.V1.FCntDown);
		}

	gLog.printf(
		gLog.kAlways,
		"NwkID:   %08x   "
		"DevAddr: %08x\n",
		Info.V1.NetID,
		Info.V1.DevAddr
		);
	}

/**** end of CatenaBase_NetSave.cpp ****/
