/* CatenaBase_GetProvisioningStyle.cpp	Tue Dec 04 2018 16:00:03 chwon */

/*

Module:  CatenaBase_GetProvisioningStyle.cpp

Function:
	CatenaBase::GetProvisioningStyle().

Version:
	V0.12.0	Tue Dec 04 2018 16:00:03 chwon	Edit level 1

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
   0.12.0  Tue Dec 04 2018 16:00:03  chwon
	20782: Module created.

*/

#include <CatenaBase.h>

#include <Catena_Fram.h>
#include <Catena_Log.h>

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


Arduino_LoRaWAN::ProvisioningStyle
CatenaBase::GetProvisioningStyle(
	void
	)
	{
	auto const pFram = this->getFram();

	if (pFram == nullptr)
		{
		gLog.printf(
			gLog.kError,
			"?CatenaBase::GetAbpProvisioningInfo: no FRAM\n"
			);
		return Arduino_LoRaWAN::ProvisioningStyle::kNone;
		}

	cFram::Cursor framJoin(pFram);

	if (! framJoin.locate(cFramStorage::vItemDefs[cFramStorage::kJoin]))
		{
		gLog.printf(
			gLog.kError,
			"?CatenaBase::GetProvisioningStyle: failing\n"
			);
		return Arduino_LoRaWAN::ProvisioningStyle::kNone;
		}

	uint8_t uJoin;
	if (! framJoin.get(&uJoin, sizeof(uJoin)))
		{
		gLog.printf(
			gLog.kError,
			"?CatenaBase::GetProvisioningStyle: get() failed\n"
			);
		return Arduino_LoRaWAN::ProvisioningStyle::kNone;
		}

	switch (uJoin)
		{
	/*
	|| we use 0 as the "none" indicator, because that's the default
	|| value when writing out the key.
	*/
	case 0:
		return Arduino_LoRaWAN::ProvisioningStyle::kNone;

	case 1:
		return Arduino_LoRaWAN::ProvisioningStyle::kOTAA;

	case 2:
		return Arduino_LoRaWAN::ProvisioningStyle::kABP;

	default:
		gLog.printf(
			gLog.kError,
			"?CatenaBase::GetProvisioningStyle:"
			" bad Join value: %u\n",
			uJoin
			);
		return Arduino_LoRaWAN::ProvisioningStyle::kNone;
		}
	}

/**** end of CatenaBase_LoRaWAN_getprovisioningstyle.cpp ****/
