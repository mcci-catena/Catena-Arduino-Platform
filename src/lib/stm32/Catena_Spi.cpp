/* Catena_Spi.cpp	Mon Nov 06 2017 16:55:55 chwon */

/*

Module:  Catena_Spi.cpp

Function:
	Definition of Catena SPI

Version:
	V0.6.0	Mon Nov 06 2017 16:55:55 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	November 2017

Revision history:
   0.6.0  Mon Nov 06 2017 16:55:55 chwon
	Module created.

*/


#ifdef ARDUINO_ARCH_STM32

#include "Catena_Spi.h"
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


SPIClass Catena_SPI(FLASH_MOSI, FLASH_MISO, FLASH_SCK);

#endif // ARDUINO_ARCH_STM32

/**** end of Catena_Spi.cpp ****/
