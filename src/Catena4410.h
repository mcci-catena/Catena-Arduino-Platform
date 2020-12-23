/* Catena4410.h	Wed Dec 05 2018 14:18:53 chwon */

/*

Module:  Catena4410.h

Function:
	Arduino library header for Catena 4410

Version:
	V0.12.0	Wed Dec 05 2018 14:18:53 chwon	Edit level 4

Copyright notice:
	This file copyright (C) 2016, 2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	October 2016

Revision history:
   0.1.0  Sat Oct 15 2016 22:12:56  tmm
	Module created.

   0.4.0  Sun Dec  4 2016 20:01:55  tmm
	Refactor based on CatenaFeatherM0.

   0.11.0  Wed Nov 21 2018 13:40:53  chwon
	Add CatenaName() method.

   0.12.0  Wed Dec 05 2018 14:18:53  chwon
	Use CatenaFeatherM0 super class instead of CatenaFeatherM0LoRa class.
	Add getCpuIdPlatformTable() method override.

*/

#ifndef _CATENA4410_H_		/* prevent multiple includes */
#define _CATENA4410_H_

#ifndef _CATENAFEATHERM0_H_
# include "CatenaFeatherM0.h"
#endif

#ifndef _CATENAFEATHERM0LORA_H_
# include "CatenaFeatherM0.h"
#endif

namespace McciCatena {

class Catena4410 : public CatenaFeatherM0
	{
public:
	enum DIGITAL_PINS
		{
		PIN_ONE_WIRE = 0,
		PIN_SHT10_CLK = 11,
		PIN_SHT10_DATA = 10
		};
	
	/*
	|| Methods
	*/
	Catena4410();

	/*
	|| LoRaWAN binding
	*/
	class LoRaWAN /* forward */;

	// Sigfox binding
	class Sigfox /* forward */;

	virtual const char *CatenaName() const override { return "Catena 4410"; };

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
|| The LoRaWAN class for the Catena4410. For now, we assume The Things
|| Network.
*/
class Catena4410::LoRaWAN : public CatenaFeatherM0::LoRaWAN
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
	// bool begin(Catena4410 *pCatena4410);

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

/**** end of Catena4410.h ****/
#endif /* _CATENA4410_H_ */
