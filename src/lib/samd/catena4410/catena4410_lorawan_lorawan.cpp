/* catena4410_lorawan_lorawan.cpp	Tue Oct 25 2016 03:53:26 tmm */

/*

Module:  catena4410_lorawan_lorawan.cpp

Function:
	Catena4410::LoRaWAN::LoRaWAN()

Version:
	V0.1.0	Tue Oct 25 2016 03:53:26 tmm	Edit level 1

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
   0.1.0  Tue Oct 25 2016 03:53:26  tmm
	Module created.

*/

#ifdef ARDUINO_ARCH_SAMD

#include <Catena4410.h>

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

static const Arduino_LoRaWAN::lmic_pinmap sk_lmic_pins = 
{
    .nss = Catena4410::PIN_SX1276_NSS,      // chip select
    .rxtx = Arduino_LoRaWAN::lmic_pinmap::LMIC_UNUSED_PIN,
    .rst = Catena4410::PIN_SX1276_NRESET,   // reset pin

    .dio = {Catena4410::PIN_SX1276_DIO0,    // DIO0 is hardwired to GPIO3
            Catena4410::PIN_SX1276_DIO1,    // DIO1 is jumpered to GPIO6
            Arduino_LoRaWAN::lmic_pinmap::LMIC_UNUSED_PIN}, 
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
Catena4410::LoRaWAN::LoRaWAN() : Catena4410::LoRaWAN::Super(sk_lmic_pins)
        {
        /* nothing needs to be done... but this pulls in the pin table */
        }

#endif // ARDUINO_ARCH_SAMD
