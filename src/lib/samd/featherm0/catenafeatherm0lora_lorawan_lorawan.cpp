/* catenafeatherm0lora_lorawan_lorawan.cpp	Sun Mar 12 2017 18:50:23 tmm */

/*

Module:  catenafeatherm0lora_lorawan_lorawan.cpp

Function:
	CatenaFeatherM0LoRa::LoRaWAN::LoRaWAN()

Version:
	V0.5.0	Sun Mar 12 2017 18:50:23 tmm	Edit level 2

Copyright notice:
	This file copyright (C) 2016-2017 by

		MCCI Corporation
		3520 Krums Corners Road
                Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	October 2016

Revision history:
   0.1.0  Tue Oct 25 2016 03:53:26  tmm
	Module created.

*/

#ifdef ARDUINO_ARCH_SAMD

#include <CatenaFeatherM0LoRa.h>

#include <Arduino_LoRaWAN_lmic.h>
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

// assumes external jumpers [feather_lora_jumper]

static const Arduino_LoRaWAN::lmic_pinmap lmic_pins_featherm0lora = 
{
    .nss = CatenaFeatherM0LoRa::PIN_SX1276_NSS,      // chip select is D8
    .rxtx = Arduino_LoRaWAN::lmic_pinmap::LMIC_UNUSED_PIN,
    .rst = CatenaFeatherM0LoRa::PIN_SX1276_NRESET,   // NRESET is D4

    .dio = {CatenaFeatherM0LoRa::PIN_SX1276_DIO0,    // DIO0 (IRQ) is D3
            CatenaFeatherM0LoRa::PIN_SX1276_DIO1,    // DIO1 is D6
            CatenaFeatherM0LoRa::PIN_SX1276_DIO2,    // DIO2 is not used
            },
};



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

/* the constructor */
CatenaFeatherM0LoRa::LoRaWAN::LoRaWAN() : 
		CatenaFeatherM0LoRa::LoRaWAN::Super(lmic_pins_featherm0lora)
        {
        /* nothing needs to be done... but this pulls in the pin table */
        }

#endif // ARDUINO_ARCH_SAMD
