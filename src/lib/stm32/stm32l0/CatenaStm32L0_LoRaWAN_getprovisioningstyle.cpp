/* CatenaStm32L0_LoRaWAN_getprovisioningstyle.cpp	Wed Dec 05 2018 14:36:35 chwon */

/*

Module:  CatenaStm32L0_LoRaWAN_getprovisioningstyle.cpp

Function:
	CatenaStm32L0::LoRaWAN::GetProvisioningStyle().

Version:
	V0.12.0	Wed Dec 05 2018 14:36:35 chwon	Edit level 2

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

   0.12.0  Wed Dec 05 2018 14:36:35  chwon
	Use Catena provisioning method.

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


CatenaStm32L0::LoRaWAN::ProvisioningStyle
CatenaStm32L0::LoRaWAN::GetProvisioningStyle(
	void
	)
	{
        CatenaStm32L0 * const pCatena = this->m_pCatena;

	return pCatena->GetProvisioningStyle();
	}

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32L0_LoRaWAN_getprovisioningstyle.cpp ****/
