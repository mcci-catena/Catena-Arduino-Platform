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
			MCCI_Catena_Sigfox::SigfoxConfiguringInfo *pInfo
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
	MCCI_Catena_Sigfox::SigfoxConfiguringInfo *pInfo
	)
	{
	auto const pFram = this->getFram();

	if (pFram == nullptr)
		{
		gLog.printf(
			gLog.kError,
			"?CatenaBase::GetSigfoxConfiguringInfo: no FRAM\n"
			);

		if (pInfo != nullptr)
			memset(pInfo, 0, sizeof(pInfo));

		return false;
		}

	/* cFram::Cursor framSigfoxCredentials(pFram); */

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

	/* if (framSigfoxCredentials.locate(cFramStorage::vItemDefs[cFramStorage::kSigfoxCredentials]))
		fResult = true; */

	if (! fResult)
		{
		gLog.printf(
			gLog.kError,
			"?CatenaBase::GetSigfoxConfiguringInfo: failing\n"
			);

		if (pInfo != nullptr)
			memset(pInfo, 0, sizeof(pInfo));

		return false;
		}

	if (pInfo == nullptr)
		return true;

	/* copy the data */
	framDevID.get(pInfo->DevID, sizeof(pInfo->DevID));
	framPAC.get(pInfo->PAC, sizeof(pInfo->PAC));
	framKey.get(pInfo->Key, sizeof(pInfo->Key));
	framRegion.get(pInfo->Region, sizeof(pInfo->Region));
	framEncryption.get(pInfo->Encryption, sizeof(pInfo->Encryption));

	/* copy the data */
	/* framSigfoxCredentials.get(pInfo->AppKey, sizeof(pInfo->AppKey));
	framSigfoxCredentials.get(pInfo->DevEUI, sizeof(pInfo->DevEUI));
	framSigfoxCredentials.get(pInfo->AppEUI, sizeof(pInfo->AppEUI));
	framSigfoxCredentials.get(pInfo->DevEUI, sizeof(pInfo->DevEUI));
	framSigfoxCredentials.get(pInfo->AppEUI, sizeof(pInfo->AppEUI)); */

	return true;
	}

/**** end of CatenaBase_GetSigfoxConfiguringInfo.cpp ****/
