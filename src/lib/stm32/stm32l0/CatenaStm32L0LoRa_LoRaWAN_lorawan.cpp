/* CatenaStm32L0LoRa_LoRaWAN_lorawan.cpp	Fri Dec 01 2017 13:57:56 chwon */

/*

Module:  CatenaStm32L0LoRa_LoRaWAN_lorawan.cpp

Function:
	CatenaStm32L0LoRa::LoRaWAN::LoRaWAN()

Version:
	V0.6.0	Fri Dec 01 2017 13:57:56 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2017 by

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

   0.6.0  Fri Dec 01 2017 13:57:56  chwon
	Add rxtx_rx_active and spi_freq in the lmic_pinmap structure.

*/

#ifdef ARDUINO_ARCH_STM32

#include <CatenaStm32L0LoRa.h>

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

static const Arduino_LoRaWAN::lmic_pinmap lmic_pins_stm32l0lora =
	{
	.nss = CatenaStm32L0LoRa::PIN_SX1276_NSS,      // chip select is D7
	.rxtx = CatenaStm32L0LoRa::PIN_SX1276_ANT_SWITCH_RX, // RXTX is D29
	.rst = CatenaStm32L0LoRa::PIN_SX1276_NRESET,   // NRESET is D8
	
	.dio = {CatenaStm32L0LoRa::PIN_SX1276_DIO0,    // DIO0 (IRQ) is D25
		CatenaStm32L0LoRa::PIN_SX1276_DIO1,    // DIO1 is D26
		CatenaStm32L0LoRa::PIN_SX1276_DIO2,    // DIO2 is D27
	       },
	.rxtx_rx_active = 1,
        .rssi_cal = 10,
	.spi_freq = 8000000	/* 8MHz */
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
CatenaStm32L0LoRa::LoRaWAN::LoRaWAN() :
		CatenaStm32L0LoRa::LoRaWAN::Super(lmic_pins_stm32l0lora)
	{
	/* nothing needs to be done... but this pulls in the pin table */
	}

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32L0LoRa_LoRaWAN_lorawan.cpp ****/
