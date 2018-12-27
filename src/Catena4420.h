/* Catena4420.h	Wed Dec 05 2018 14:20:58 chwon */

/*

Module:  Catena4420.h

Function:
	class Catena4420 (Aduino header file)

Version:
	V0.12.0	Wed Dec 05 2018 14:20:58 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2016, 2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	December 2016

Revision history:
   0.4.0  Sun Dec  4 2016 23:45:17  tmm
	Module created.

   0.11.0  Wed Nov 21 2018 13:41:46  chwon
	Add CatenaName() method.

   0.12.0  Wed Dec 05 2018 14:20:58  chwon
	Add getCpuIdPlatformTable() method override.

*/

#ifndef _CATENA4420_H_		/* prevent multiple includes */
#define _CATENA4420_H_

#ifndef _CATENAFEATHERM0_H_
# include "CatenaFeatherM0.h"
#endif

#include <Arduino_LoRaWAN_ttn.h>

namespace McciCatena {

class Catena4420 : public CatenaFeatherM0
	{
public:
	/*
	|| Methods
	*/
	Catena4420();

	/*
	|| LoRaWAN binding
	*/
	class LoRaWAN /* forward */;

	virtual const char *CatenaName() const override { return "Catena 4420"; };

protected:
	using Super = CatenaFeatherM0;
        virtual const Arduino_LoRaWAN::ProvisioningTable *
        	GetLoRaWANkeys(void) const override;

	virtual void getCpuIdPlatformTable(
		const CPUID_PLATFORM_MAP * &vCpuIdToPlatform,
		size_t &nvCpuIdToPlatform
		) override;

private:
        static const Arduino_LoRaWAN::ProvisioningTable gk_LoRaWAN_Keys
                /* __attribute__((__weak__)) */;

	static const CPUID_PLATFORM_MAP vCpuIdToPlatform[];
	static const size_t nvCpuIdToPlatform;
	};

/*
|| The LoRaWAN class for the Catena4420. For now, we assume The Things
|| Network.
*/
class Catena4420::LoRaWAN : public CatenaFeatherM0::LoRaWAN
	{
public:
        using Super = CatenaFeatherM0::LoRaWAN;

	/*
	|| the constructor.
	*/
	LoRaWAN() {};

	/*
	|| the begin function loads data from the local
	|| platform's stable storage and initializes
	|| the connection. 
	*/
	// bool begin(CatenaFeatherM0 *pCatena4420);

protected:
	/*
	|| we use the CatenaFeatherM0 defaults
	*/
	// ProvisioningStyle GetProvisioningStyle(void);

	// bool GetAbpProvisioningInfo(
	//		AbpProvisioningInfo *pProvisioningInfo
	//		);

	// bool GetOtaaProvisioningInfo(
	//		OtaaProvisioningInfo *pProvisioningInfo
	//		);

private:
	};

} /* namespace McciCatena */


/**** end of Catena4420.h ****/
#endif /* _CATENA4420_H_ */
