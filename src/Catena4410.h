/* Catena4410.h	Sun Dec  4 2016 20:01:55 tmm */

/*

Module:  Catena4410.h

Function:
	Arduino library header for Catena 4410

Version:
	V0.4.0	Sun Dec  4 2016 20:01:55 tmm	Edit level 3

Copyright notice:
	This file copyright (C) 2016 by

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

*/

#ifndef _CATENA4410_H_		/* prevent multiple includes */
#define _CATENA4410_H_

#ifndef _CATENAFEATHERM0_H_
# include "CatenaFeatherM0.h"
#endif

#ifndef _CATENAFEATHERM0LORA_H_
# include "CatenaFeatherM0LoRa.h"
#endif

namespace McciCatena {

class Catena4410 : public CatenaFeatherM0
	{
public:
	enum DIGITAL_PINS
		{
		PIN_ONE_WIRE = 0,
		PIN_SHT10_CLK = 11,
		PIN_SHT10_DATA = 10,
		PIN_SX1276_NSS = 8,
		PIN_SX1276_NRESET = 4,
		PIN_SX1276_DIO0 = 3,
		PIN_SX1276_DIO1 = 6,
		};
	
	/*
	|| Methods
	*/
	Catena4410();

	/*
	|| LoRaWAN binding
	*/
	class LoRaWAN /* forward */;

protected:
	using Super = CatenaFeatherM0;
        virtual const Arduino_LoRaWAN::ProvisioningTable *GetLoRaWANkeys(void) const;

private:
        static const Arduino_LoRaWAN::ProvisioningTable gk_LoRaWAN_Keys
                /* __attribute__((__weak__)) */;
	};

/*
|| The LoRaWAN class for the Catena4410. For now, we assume The Things
|| Network.
*/
class Catena4410::LoRaWAN : public CatenaFeatherM0LoRa::LoRaWAN
	{
public:
        using Super = CatenaFeatherM0LoRa::LoRaWAN;

	/*
	|| the constructor.
	*/
	LoRaWAN();
        LoRaWAN(const lmic_pinmap &pinmap) : Super(pinmap) {};
		
	/*
	|| the begin function loads data from the local
	|| platform's stable storage and initializes
	|| the connection. 
	*/
	bool begin(Catena4410 *pCatena4410);

protected:
	/*
	|| we have to provide these for the lower level
	*/
	ProvisioningStyle GetProvisioningStyle(void);

	bool GetAbpProvisioningInfo(
			AbpProvisioningInfo *pProvisioningInfo
			);

	bool GetOtaaProvisioningInfo(
			OtaaProvisioningInfo *pProvisioningInfo
			);

private:
	Catena4410		*m_pCatena4410;
	const CATENA_PLATFORM	*m_pPlatform;
	};

} /* namespace McciCatena */

/**** end of Catena4410.h ****/
#endif /* _CATENA4410_H_ */
