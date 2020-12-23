/* Catena461x.h	Fri Dec 28 2018 13:54:46 chwon */

/*

Module:  Catena461x.h

Function:
	class Catena461x: CatenaBase Platform to represent a Catena 461x
	(4610, 4611, 4612, etc.)

Version:
	V0.13.0	Fri Dec 28 2018 13:54:47 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	November 2018

Revision history:
   0.11.0  Tue Nov 06 2018 14:25:23 chwon
	Module created.

   0.12.0  Mon Nov 26 2018 15:50:21  chwon
	Change from CatenaStm32L0LoRa to CatenaStm32L0 class.

   0.13.0  Fri Dec 28 2018 13:54:47  chwon
	Add ANALOG_CHANNELS definition and move ReadVbat() and ReadVbus()
	override to Catena4610/11/12.

*/

#ifndef _CATENA461X_H_	/* prevent multiple includes */
#define _CATENA461X_H_

#pragma once

#ifndef _CATENASTM32L0_H_
# include "CatenaStm32L0.h"
#endif

namespace McciCatena {

class Catena461x : public CatenaStm32L0
	{
public:
        using Super = CatenaStm32L0;

        // no specific constructor.
        Catena461x() {};

	// uses default destructor

	// neither copyable nor movable
	Catena461x(const Catena461x&) = delete;
	Catena461x& operator=(const Catena461x&) = delete;
	Catena461x(const Catena461x&&) = delete;
	Catena461x& operator=(const Catena461x&&) = delete;

	// LoRaWAN binding
	class LoRaWAN /* forward */;

	// Sigfox binding
	class Sigfox /* forward */;

	enum ANALOG_PINS
		{
		APIN_VBAT_SENSE = A3,
		APIN_VBUS_SENSE = A4,
		};

	enum ANALOG_CHANNELS
		{
		ANALOG_CHANNEL_A0 = 0,
		ANALOG_CHANNEL_A1 = 5,
		ANALOG_CHANNEL_A2 = 4,
		ANALOG_CHANNEL_A3 = 3,
		ANALOG_CHANNEL_A4 = 2,
		ANALOG_CHANNEL_VBAT = ANALOG_CHANNEL_A3,
		ANALOG_CHANNEL_VBUS = ANALOG_CHANNEL_A4,
		ANALOG_CHANNEL_VREF = 17,
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

protected:

private:
	};

/*
|| The LoRaWAN class for the Catena 455x. Assumes The Things Network
*/
class Catena461x::LoRaWAN : public CatenaStm32L0::LoRaWAN
	{
public:
	using Super = CatenaStm32L0::LoRaWAN;

	/*
	|| the constructor. We don't do anything at this level, the
	|| Super constructor does most of the work.
	*/
	LoRaWAN() {};

	bool begin(Catena461x *pParent);

protected:

private:
	};

/*
|| The Sigfox class for the Catena 461x.
*/
class Catena461x::Sigfox : public CatenaStm32L0::Sigfox
	{
public:
	using Super = CatenaStm32L0::Sigfox;

	/*
	|| the constructor. We don't do anything at this level, the
	|| Super constructor does most of the work.
	*/
	Sigfox() {};

	bool begin(Catena461x *pParent);

protected:

private:
	};

} // namespace McciCatena

/**** end of Catena461x.h ****/
#endif /* _CATENA461X_H_ */
