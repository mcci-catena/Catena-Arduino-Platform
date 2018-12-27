/* CatenaBase_GetOtaaProvisioningInfo.cpp	Mon Dec 03 2018 14:27:58 chwon */

/*

Module:  CatenaBase_GetOtaaProvisioningInfo.cpp

Function:
	CatenaBase::GetOtaaProvisioningInfo()

Version:
	V0.12.0	Mon Dec 03 2018 14:27:58 chwon	Edit level 1

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
   0.12.0  Mon Dec 03 2018 14:27:58  chwon
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

Name:	CatenaBase::GetOtaaProvisioningInfo()

Function:
	Fetch OTAA provisioning info from FRAM (if availalbe)

Definition:
	public: virtual bool
		CatenaBase::GetOtaaProvisioningInfo(
			Arduino_LoRaWAN::OtaaProvisioningInfo *pInfo
			) override;

Description:
	This routine fetches the OTAA provisioning info from FRAM if
	available, formatting it into *pInfo.  For this to work, FRAM
	must be initialized and the AppKey, AppEUI and DevEUI must be
	available.

	If pInfo is nullptr, the routine simply checks whether the info
	is availalbe.

Returns:
	This routine returns true if and only if the provisioning info
	is available.

*/

bool
CatenaBase::GetOtaaProvisioningInfo(
	Arduino_LoRaWAN::OtaaProvisioningInfo *pInfo
	)
	{
	auto const pFram = this->getFram();

	if (pFram == nullptr)
		{
		gLog.printf(
			gLog.kError,
			"?CatenaBase::GetOtaaProvisioningInfo: no FRAM\n"
			);

		if (pInfo != nullptr)
			memset(pInfo, 0, sizeof(pInfo));

		return false;
		}

	cFram::Cursor framAppEUI(pFram),
		      framDevEUI(pFram),
		      framAppKey(pFram);
	bool fResult;

	fResult = false;

	if (framAppEUI.locate(cFramStorage::vItemDefs[cFramStorage::kAppEUI]) &&
	    framDevEUI.locate(cFramStorage::vItemDefs[cFramStorage::kDevEUI]) &&
	    framAppKey.locate(cFramStorage::vItemDefs[cFramStorage::kAppKey]))
		fResult = true;

	if (! fResult)
		{
		gLog.printf(
			gLog.kError,
			"?CatenaBase::GetOtaaProvisioningInfo: failing\n"
			);

		if (pInfo != nullptr)
			memset(pInfo, 0, sizeof(pInfo));

		return false;
		}

	if (pInfo == nullptr)
		return true;

	/* copy the data */
	framAppKey.get(pInfo->AppKey, sizeof(pInfo->AppKey));
	framDevEUI.get(pInfo->DevEUI, sizeof(pInfo->DevEUI));
	framAppEUI.get(pInfo->AppEUI, sizeof(pInfo->AppEUI));

	return true;
	}

/**** end of CatenaBase_GetOtaaProvisioningInfo.cpp ****/
