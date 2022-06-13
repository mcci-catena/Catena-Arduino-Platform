/*

Module:  Catena4610.h

Function:
	class Catena4610: CatenaBase Platform to represent a Catena 4610

Copyright notice:
	See accompanying license file.

Author:
	ChaeHee Won, MCCI Corporation	November 2018

Revision history:
   0.11.0  Thu Nov 15 2018 15:07:23 chwon
	Module created.

   0.13.0  Fri Dec 28 2018 13:58:16  chwon
	Add ReadVbat() and ReadVbus() override.

   0.23.0-pre1  Sun Jun 12 2022 07:20:39  tmm
	Refactor to add common properties.

*/

#ifndef _Catena4610_H_	/* prevent multiple includes */
#define _Catena4610_H_

#pragma once

#ifndef _CATENA461X_H_
# include "Catena461x.h"
#endif

namespace McciCatena {

class Catena4610 : public Catena461x
	{
public:
        using Super = Catena461x;

        // no specific constructor.
        Catena4610() {};

	// uses default destructor

	// neither copyable nor movable
	Catena4610(const Catena4610&) = delete;
	Catena4610& operator=(const Catena4610&) = delete;
	Catena4610(const Catena4610&&) = delete;
	Catena4610& operator=(const Catena4610&&) = delete;

	virtual const char *CatenaName() const override { return "Catena 4610"; };
	virtual float ReadVbat(void) const override;
	virtual float ReadVbus(void) const override;

	/// return name of screw terminal. On the 4610, these are JP4 and JP5.
        virtual const char *get_screwTerminalLabel(uint8_t iTerminal) const override
		{
		static const char jp4[] = "JP4";
		static const char jp5[] = "JP5";
		return iTerminal == 0 ? jp4 : iTerminal == 1 ? jp5 : nullptr;
		}

	/// return Arduino pin ID of pin 2 on indicated screw terminal
        virtual uint8_t get_screwTerminalPin2(uint8_t iTerminal) const override
		{
		return iTerminal == 0 ? D5 : iTerminal == 1 ? A1 : 0;
		}
	/// return Arduino pin ID of pin 3 on indicated screw terminal
        virtual uint8_t get_screwTerminalPin3(uint8_t iTerminal) const override
		{
		return iTerminal == 0 ? D12 : iTerminal == 1 ? A2 : 0;
		}

	/// return true if board really has a BME280 by design
	// TODO(tmm@mcci.com): this changes on rev D hardware.
        virtual bool has_BME280() const override
		{ return true; }
        // virtual bool has_BME680() const override;
        // virtual bool has_HS3001() const override;
        // virtual bool has_SHT3x() const override;
        // virtual uint32_t enable_SHT3x(bool fStatus) override;
        // virtual bool get_SHT3xRequest() const override;
        // virtual bool has_SHT3xPowerControl() const override;
        // virtual bool has_SGPC3() const override;
        // virtual uint32_t enable_SGPC3(bool) override;
        // virtual bool get_SGPC3Request() const override;
        // virtual bool has_SGPC3PowerControl() const override;
        // virtual bool has_PMS7003() const override;
        // virtual uint32_t enable_PMS7003(bool fStatus) override;
        // virtual bool get_PMS7003Request() const override;
        // virtual bool has_PMS7003Control() const override;
	/// return true if board really has a Si1133 by design
	// TODO(tmm@mcci.com): this changes on rev D hardware.
        virtual bool has_Si1133() const override
		{ return true; }
        // virtual bool has_BH1750() const override;
        // virtual bool get_PMS7003Request() const override;
        virtual uint32_t enable_tcxo(bool fStatus) override
		{ return this->m_tcxo.enable(fStatus); }
        virtual bool get_tcxoRequest() const override
		{ return this->m_tcxo.getRequest(); }
        virtual bool has_tcxoControl() const override
		{ return this->m_tcxo.hasControl(); }
        virtual bool has_usbPort() const override
		{
		return true;
		}
        virtual bool get_consoleIsUsb() const override
		{
#ifdef USBCON
		return true;
#else
		return false;
#endif
		}
        virtual bool has_usbVbusMeasurementStandard() const override
		{ return true; }
        virtual uint8_t get_usbVbusMeasurementPin() const override
		{ return APIN_VBUS_SENSE; }

protected:
	/// on the Catena 4610, VOUT1 is controlled by D10
	cPowerControlGPIO  m_Vout1 = cPowerControlGPIO{ D10, 10 };
	/// on the Catena 4610, VOUT2 is controlled by D11
	cPowerControlGPIO  m_Vout2 = cPowerControlGPIO{ D11, 10 };
	/// on the Catena 4610, each screw terminal has its own VOUT
	cPowerControlNested m_screwTerminal[2] = { cPowerControlNested{this->m_Vout1}, cPowerControlNested{this->m_Vout2} };
	/// on the Catena 4610, there is no 3.3v regulator
	cPowerControlDummy m_3v3;
	/// on the Catena 4610, there's a TCXO controlled by D33
	cPowerControlGPIO m_tcxo = cPowerControlGPIO{ D33, 2 };
	/// on the Catena 4610, flash is always powered
	cPowerControlDummy m_flashVdd;
	/// on the Catena 4610, i2c is always powered
	cPowerControlDummy m_i2cVdd;
	/// on the Catena 4610, FRAM is always powered
	cPowerControlDummy m_framVdd;
	/// the Catena 4610 doesn't have an I2C bridge
	cPowerControlDummy m_externalI2cBridgeVdd;

protected:
	// we are required to provide a table of platforms
	virtual void getPlatformTable(
		const CATENA_PLATFORM * const * &vPlatforms,
		size_t &nvPlatforms
		) override;

private:
	// the known platforms
	static const CATENA_PLATFORM(* const vPlatforms[]);
	static const size_t nvPlatforms;
	};

} // namespace McciCatena

/**** end of Catena4610.h ****/
#endif /* _Catena4610_H_ */
