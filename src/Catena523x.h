/*

Module:  Catena523x.h

Function:
        class Catena523x: CatenaBase Platform to represent a Catena 523x
        (such as the 5230).

Copyright notice:
        See accompanying LICENSE file.

Author:
        Murali, MCCI Corporation	Sep 2025

*/

#ifndef _CATENA523X_H_	/* prevent multiple includes */
#define _CATENA523X_H_

#pragma once

#ifndef _CATENASTM32L0_H_
# include "CatenaStm32L0.h"
#endif

#ifndef _MCCI_CATENA_NPM1300_H_
# include <MCCI_Catena_nPM1300.h>
#endif

namespace McciCatena {

class Catena523x : public CatenaStm32L0
	{
public:
        using Super = CatenaStm32L0;

        // constructor is defined out-of-line, in Catena523x_begin.cpp,
        // where it binds m_pNpm1300 to this board's single PMIC
        // instance.
        Catena523x();

	// uses default destructor

	// neither copyable nor movable
	Catena523x(const Catena523x&) = delete;
	Catena523x& operator=(const Catena523x&) = delete;
	Catena523x(const Catena523x&&) = delete;
	Catena523x& operator=(const Catena523x&&) = delete;

	// LoRaWAN binding
	class LoRaWAN /* forward */;

	enum ANALOG_CHANNELS
		{
		ANALOG_CHANNEL_A0 = 4,
		ANALOG_CHANNEL_A1 = 3,
		ANALOG_CHANNEL_A2 = 2,
		ANALOG_CHANNEL_A3 = 0,
		ANALOG_CHANNEL_A4 = 1,
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

	// get the PMIC driver. Unlike the watchdog timer (which is part of
	// the STM32 itself, and present on every board), the nPM1300 is a
	// part on the 5230 board specifically: it's a static object
	// reached only through the Catena, not a library-wide free global.
	McciCatenaNpm1300::cNPM1300 &getPmic(void) const
		{
		return *this->m_pNpm1300;
		}

	// true if the PMIC driver successfully started. See the comment on
	// begin() in Catena523x_begin.cpp for why a failed PMIC fails the
	// whole begin() sequence, rather than leaving this as the only way
	// to tell.
	bool isPmicRunning(void) const
		{
		return this->m_pNpm1300->isRunning();
		}

protected:
	// a pointer, not a value member: ReadVbat()/ReadVbus() are const
	// overrides (per the CatenaBase contract followed by every board
	// class), and this lets them reach the (non-const) PMIC driver
	// through a const Catena523x without needing a mutable member --
	// the same pattern used for the TwoWire* members in this library's
	// other I2C drivers (for example Catena_Mb85rc64ta's m_pWire).
	McciCatenaNpm1300::cNPM1300 * const m_pNpm1300;

private:
	};

/*
|| The LoRaWAN class for the Catena 523x. Assumes The Things Network
*/
class Catena523x::LoRaWAN : public CatenaStm32L0::LoRaWAN
	{
public:
	using Super = CatenaStm32L0::LoRaWAN;

	/*
	|| the constructor. We don't do anything at this level, the
	|| Super constructor does most of the work.
	*/
	LoRaWAN() {};

	bool begin(Catena523x *pParent);

protected:

private:
	};

} // namespace McciCatena

/**** end of Catena523x.h ****/
#endif /* _CATENA523X_H_ */
