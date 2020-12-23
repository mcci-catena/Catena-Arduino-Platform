/*

Module:  CatenaBase_GetSigfoxConfiguringInfo.cpp

Function:
	CatenaBase::GetSigfoxConfiguringInfo()

Copyright notice:
	See accompanying LICENSE file.

Author:
	Dhinesh Kumar Pitchai, MCCI Corporation	December 2020

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

Name:	CatenaBase::GetSigfoxConfiguringInfo()

Function:
	Fetch OTAA provisioning info from FRAM (if availalbe)

Definition:
	public: virtual bool
		CatenaBase::GetSigfoxConfiguringInfo(
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
CatenaBase::GetSigfoxConfiguringInfo(
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

	cFram::Cursor framDevID(pFram),
		      framPAC(pFram),
		      framKey(pFram),
		      framRegion(pFram),
		      framEncryption(pFram);
	bool fResult;

	fResult = false;

	if (framDevID.locate(cFramStorage::vItemDefs[cFramStorage::kDevID]) &&
	    framPAC.locate(cFramStorage::vItemDefs[cFramStorage::kPAC]) &&
	    framKey.locate(cFramStorage::vItemDefs[cFramStorage::kKey]) &&
	    framRegion.locate(cFramStorage::vItemDefs[cFramStorage::kRegion]) &&
	    framEncryption.locate(cFramStorage::vItemDefs[cFramStorage::kEncryption]))
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
	framDevID.get(pInfo->AppKey, sizeof(pInfo->AppKey));
	framPAC.get(pInfo->DevEUI, sizeof(pInfo->DevEUI));
	framKey.get(pInfo->AppEUI, sizeof(pInfo->AppEUI));
	framRegion.get(pInfo->DevEUI, sizeof(pInfo->DevEUI));
	framEncryption.get(pInfo->AppEUI, sizeof(pInfo->AppEUI));

	return true;
	}

/**** end of CatenaBase_GetOtaaProvisioningInfo.cpp ****/
