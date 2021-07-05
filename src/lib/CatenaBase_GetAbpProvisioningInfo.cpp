/* CatenaBase_GetAbpProvisioningInfo.cpp	Mon Dec 03 2018 14:41:22 chwon */

/*

Module:  CatenaBase_GetAbpProvisioningInfo.cpp

Function:
	CatenaBase::GetAbpProvisioningInfo()

Version:
	V0.12.0	Mon Dec 03 2018 14:41:22 chwon	Edit level 1

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
   0.12.0  Mon Dec 03 2018 14:41:22  chwon
	Module created.

*/

#include <CatenaBase.h>

#include <Catena_Fram.h>
#include <Catena_Log.h>

using namespace McciCatena;

/****************************************************************************\
|
|		Manifest constants & typedefs.
|
|	This is strictly for private types and constants which will not
|	be exported.
|
\****************************************************************************/


/****************************************************************************\
|
|	Read-only data.
|
|	If program is to be ROM-able, these must all be tagged read-only
|	using the ROM storage class; they may be global.
|
\****************************************************************************/


/****************************************************************************\
|
|	VARIABLES:
|
|	If program is to be ROM-able, these must be initialized
|	using the BSS keyword.  (This allows for compilers that require
|	every variable to have an initializer.)  Note that only those
|	variables owned by this module should be declared here, using the BSS
|	keyword; this allows for linkers that dislike multiple declarations
|	of objects.
|
\****************************************************************************/


/*

Name:	CatenaBase::GetAbpProvisioningInfo()

Function:
	Get the ABP info (which is also what's saved after an OTAA Join)

Definition:
	public: virtual bool
		CatenaBase::GetAbpProvisioningInfo(
			Arduino_LoRaWAN::AbpProvisioningInfo *pInfo
			) override;

Description:
	This routine fills in an ABP info table with saved FRAM data:

		NwkSkey (the network session key)
		AppSkey (the app session key)
		DevAddr (the assigned device address)
		NwId	(the assigned network ID)
		FCntUp	(the uplink frame count)
		FCntDown (the downlink frame count)

	(When provisioning a device for ABP, you'll want to reset the frame
	counts, as these are maintained on uplink/downlink)

Returns:
	true if the data was filled in, false if not.

*/

bool
CatenaBase::GetAbpProvisioningInfo(
	Arduino_LoRaWAN::AbpProvisioningInfo *pInfo
	)
	{
	auto const pFram = this->getFram();
	Arduino_LoRaWAN::SessionState State;

	if (pFram == nullptr)
		{
		gLog.printf(
			gLog.kError,
			"?CatenaBase::GetAbpProvisioningInfo: no FRAM\n"
			);

		if (pInfo != nullptr)
			memset(pInfo, 0, sizeof(pInfo));

		return false;
		}

	cFram::Cursor framNwkSKey(pFram),
		      framAppSKey(pFram),
		      framDevAddr(pFram),
		      framNetID(pFram),
		      framSessionState(pFram);

	bool fResult;

	fResult = false;

	if (framNwkSKey.locate(cFramStorage::vItemDefs[cFramStorage::kNwkSKey]) &&
	    framAppSKey.locate(cFramStorage::vItemDefs[cFramStorage::kAppSKey]) &&
	    framDevAddr.locate(cFramStorage::vItemDefs[cFramStorage::kDevAddr]) &&
	    framNetID.locate(cFramStorage::vItemDefs[cFramStorage::kNetID]) &&
	    framSessionState.locate(cFramStorage::vItemDefs[cFramStorage::kLmicSessionState]) &&
	    framSessionState.get((uint8_t *)&State, sizeof(State)) &&
	    State.isValid())
		{
		fResult = true;
		}

	if (! fResult)
		{
		gLog.printf(
			gLog.kError,
			"?CatenaBase::GetAbpProvisioningInfo: failing\n"
			);

		if (pInfo != nullptr)
			memset(pInfo, 0, sizeof(pInfo));

		return false;
		}

	framNwkSKey.get(pInfo->NwkSKey, sizeof(pInfo->NwkSKey));
	framAppSKey.get(pInfo->AppSKey, sizeof(pInfo->AppSKey));
	framDevAddr.getuint32(pInfo->DevAddr);
	framNetID.getuint32(pInfo->NetID);
	pInfo->FCntDown = State.V1.FCntDown;
	pInfo->FCntUp = State.V1.FCntUp;

	return true;
	}

/**** end of CatenaBase_GetAbpProvisioningInfo.cpp ****/
