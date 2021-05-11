/*

Module:  CatenaStm32L0_Sigfox_getsigfoxconfiguringinfo.cpp

Function:
	CatenaStm32L0::Sigfox::GetSigfoxConfiguringInfo()

Copyright notice:
        See accompanying LICENSE file.

Author:
	Dhinesh Kumar Pitchai, MCCI Corporation	December 2020

*/

#ifdef ARDUINO_ARCH_STM32

#include <CatenaStm32L0.h>

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


/*

Name:	CatenaStm32L0::Sigfox::GetSigfoxConfiguringInfo()

Function:
	Fetch Sigfox provisioning info from FRAM (if availalbe)

Definition:
	public: virtual bool
		CatenaStm32L0::Sigfox::GetSigfoxConfiguringInfo(
		        CatenaStm32L0::Sigfox::OtaaProvisioningInfo *pInfo
		        ) override;

Description:
	This routine fetches the OTAA provisioning info from FRAM if
	available, formatting it into *pInfo.  For this to work, FRAM
	must be initialized and the Key, PAC and DevID must be
	available.

	If pInfo is nullptr, the routine simply checks whether the info
	is availalbe.

Returns:
	This routine returns true if and only if the provisioning info
	is available.

*/

bool
CatenaStm32L0::Sigfox::GetSigfoxConfiguringInfo(
        CatenaStm32L0::Sigfox::SigfoxConfiguringInfo *pInfo
        )
        {
        CatenaStm32L0 * const pCatena = this->m_pCatena;

	return pCatena->GetSigfoxConfiguringInfo(pInfo);
	}

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32L0_Sigfox_getsigfoxconfiguringinfo.cpp ****/
