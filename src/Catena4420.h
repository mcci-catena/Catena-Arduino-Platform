/* Catena4420.h	Sun Dec  4 2016 23:45:17 tmm */

/*

Module:  Catena4420.h

Function:
	class Catena4420 (Aduino header file)

Version:
	V0.4.0	Sun Dec  4 2016 23:45:17 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2016 by

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
	enum DIGITAL_PINS
		{
		PIN_SX1276_NSS = 6,
		PIN_SX1276_NRESET = 5,
		PIN_SX1276_DIO0 = 12,    // pin assignment for DIO0 (aka IRQ)
		PIN_SX1276_DIO1 = 11,
		PIN_SX1276_DIO2 = 10,
		};
	
	/*
	|| Methods
	*/
	Catena4420();

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
	LoRaWAN();
        LoRaWAN(const lmic_pinmap &pinmap) : Super(pinmap) {};

	/*
	|| the begin function loads data from the local
	|| platform's stable storage and initializes
	|| the connection. 
	*/
	// bool begin(CatenaFeatherM0 *pCatena4420);

protected:
	/*
	|| we use the defaults
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
