/*

Module:  CatenaWingFram2k_Sigfox_storage.cpp

Function:
	Interface from Sigfox to FRAM.

Copyright notice:
    See accompanying LICENSE file.
 
Author:
	Dhinesh Kumar Pitchai, MCCI Corporation	December 2020

*/

#ifdef ARDUINO_ARCH_SAMD

#include "CatenaWingFram2k.h"

#include "Catena_Fram.h"
#include "Catena_Log.h"

using namespace McciCatena;


/*

Name:	CatenaWingFram2k::Sigfox::GetSigfoxConfiguringInfo()

Function:
	Fetch Sigfox provisioning info from FRAM (if availalbe)

Definition:
	public: virtual bool
		CatenaWingFram2k::Sigfox::GetSigfoxConfiguringInfo(
		        CatenaWingFram2k::Sigfox::SigfoxConfiguringInfo *pInfo
		        ) override;

Description:
	This routine fetches the Sigfox provisioning info from FRAM if
	available, formatting it into *pInfo.  For this to work, FRAM
	must be initialized and the DevID, PAC, Key and Region must be
	available.

	If pInfo is nullptr, the routine simply checks whether the info
	is availalbe.

Returns:
	This routine returns true if and only if the provisioning info
	is available.

*/

bool
CatenaWingFram2k::Sigfox::GetSigfoxConfiguringInfo(
        CatenaWingFram2k::Sigfox::SigfoxConfiguringInfo *pInfo
        )
        {
        CatenaWingFram2k * const pCatena = this->m_pCatena;

	return pCatena->GetSigfoxConfiguringInfo(pInfo);
	}

#endif // ARDUINO_ARCH_SAMD
