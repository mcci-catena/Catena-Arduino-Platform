/* Catena_Spi.h	Mon Nov 06 2017 16:55:55 chwon */

/*

Module:  Catena_Spi.h

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

#ifndef _CATENA_SPI_H_	/* prevent multiple includes */
#define _CATENA_SPI_H_

#pragma once

#include <Arduino.h>
#include <SPI.h>

namespace McciCatena {

extern SPIClass Catena_SPI;

} // namespace McciCatena

/**** end of Catena_Spi.h ****/
#endif /* _CATENA_SPI_H_ */
