/* Catena455x.h	Mon Nov 26 2018 15:49:24 chwon */

/*

Module:  Catena455x.h

Function:
	class Catena455x: CatenaBase Platform to represent a Catena 455x
	(4550, 4551, etc.)

Version:
	V0.12.0	Mon Nov 26 2018 15:49:24 chwon	Edit level 2

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

   0.12.0  Mon Nov 26 2018 15:49:24  chwon
	Change from CatenaStm32L0LoRa to CatenaStm32L0 class.

*/

#ifndef _CATENA455X_H_		/* prevent multiple includes */
#define _CATENA455X_H_

#pragma once

#ifndef _CATENASTM32L0_H_
# include "CatenaStm32L0.h"
#endif

namespace McciCatena {

class Catena455x : public CatenaStm32L0
	{
public:
        using Super = CatenaStm32L0;

        // no specific constructor.
        Catena455x() {};

	// uses default destructor

	// neither copyable nor movable
	Catena455x(const Catena455x&) = delete;
	Catena455x& operator=(const Catena455x&) = delete;
	Catena455x(const Catena455x&&) = delete;
	Catena455x& operator=(const Catena455x&&) = delete;

	// LoRaWAN binding
	class LoRaWAN /* forward */;

	// Sigfox binding
	class Sigfox /* forward */;

	enum ANALOG_PINS
		{
		APIN_VBAT_SENSE = A3,
		APIN_VBUS_SENSE = A4,
		};

	enum DIGITAL_PINS
		{
		PIN_STATUS_LED = D13,
		PIN_SPI2_FLASH_SS = D19,
		PIN_SPI2_MOSI = D23,
		PIN_SPI2_MISO = D22,
		PIN_SPI2_SCK = D24,
		};

	// methods
	virtual bool begin() override;
	virtual float ReadVbat(void) const override;
	virtual float ReadVbus(void) const override;

protected:

private:
	};

/*
|| The LoRaWAN class for the Catena 455x. Assumes The Things Network
*/
class Catena455x::LoRaWAN : public CatenaStm32L0::LoRaWAN
	{
public:
	using Super = CatenaStm32L0::LoRaWAN;

	/*
	|| the constructor. We don't do anything at this level, the
	|| Super constructor does most of the work.
	*/
	LoRaWAN() {};

	bool begin(Catena455x *pParent);

protected:

private:
	};

/*
|| The Sigfox class for the Catena 455x.
*/
class Catena455x::Sigfox : public CatenaStm32L0::Sigfox
	{
public:
	using Super = CatenaStm32L0::Sigfox;

	/*
	|| the constructor. We don't do anything at this level, the
	|| Super constructor does most of the work.
	*/
	Sigfox() {};

	bool begin(Catena455x *pParent);

protected:

private:
	};

} // namespace McciCatena

/**** end of Catena455x.h ****/
#endif /* _CATENA455X_H_ */
