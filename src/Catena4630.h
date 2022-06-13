/*

Module:  Catena4630.h

Function:
        class Catena4630: CatenaBase Platform to represent a Catena 4630.

Copyright notice:
        See accompanying LICENSE file.

Author:
        Dhinesh Kumar Pitchai, MCCI Corporation	April 2019

*/

#ifndef _Catena4630_H_	/* prevent multiple includes */
#define _Catena4630_H_

#pragma once

#ifndef _CATENA463X_H_
# include "Catena463x.h"
#endif

namespace McciCatena {

class Catena4630 : public Catena463x
	{
public:
        using Super = Catena463x;

        // no specific constructor.
        Catena4630() {};

	// uses default destructor

	// neither copyable nor movable
	Catena4630(const Catena4630&) = delete;
	Catena4630& operator=(const Catena4630&) = delete;
	Catena4630(const Catena4630&&) = delete;
	Catena4630& operator=(const Catena4630&&) = delete;

	virtual const char *CatenaName() const override { return "Catena 4630"; };
	virtual float ReadVbat(void) const override;
	virtual float ReadVbus(void) const override;

	///
	/// \brief return name of screw terminals on Catena 4630
	///
	/// \param [in] iTerminal zero-origin index of screw terminal.
	///
	/// \return There are no screw terminals, so the result is nullptr.
	///
        virtual const char *get_screwTerminalLabel(uint8_t iTerminal) const override
		{
		MCCIADK_API_PARAMETER(iTerminal);

		return nullptr;
		}

	///
	/// \brief return pin2 digital pin ID for Catena 4630
	///
	/// \param [in] iTerminal zero-origin index of screw terminal in question.
	///
	/// \return There are no screw terminals, so the result is zero.
	///
        virtual uint8_t get_screwTerminalPin2(uint8_t iTerminal) const override
		{
		MCCIADK_API_PARAMETER(iTerminal);

		return 0;
		}
	///
	/// \brief return pin3 digital pin ID for Catena 4630
	///
	/// \param [in] iTerminal zero-origin index of screw terminal in question.
	///
	/// \return There are no screw terminals, so the result is zero.
	///
        virtual uint8_t get_screwTerminalPin3(uint8_t iTerminal) const override
		{
		MCCIADK_API_PARAMETER(iTerminal);

		return 0;
		}

        // virtual bool has_BME280() const override
        // virtual bool has_BME680() const override;
        // virtual bool has_HS3001() const override;
        virtual bool has_SHT3x() const override
		{ return true; }
        virtual uint32_t enable_SHT3x(bool fStatus) override
		{ return this->m_sht3x_vdd.enable(fStatus); }
        // virtual bool get_SHT3xRequest() const override;
        // virtual bool has_SHT3xPowerControl() const override;
        virtual bool has_SGPC3() const override
		{ return true; }
        virtual uint32_t enable_SGPC3(bool fStatus) override
		{ return this->m_sgpc3.enable(fStatus); }
        virtual bool get_SGPC3Request() const override
		{ return this->m_sgpc3.getRequest(); }
        virtual bool has_SGPC3PowerControl() const override
		{ return this->m_sgpc3.hasControl(); }
        virtual bool has_PMS7003() const override
		{ return true; }
        virtual uint32_t enable_PMS7003(bool fStatus) override
		{ return this->m_pms7003.enable(fStatus); }
        // virtual bool get_PMS7003Request() const override;
        // virtual bool has_PMS7003Control() const override;
        // virtual bool has_Si1133() const override;
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
	/// on the Catena 4630, there's no Vout1
	cPowerControlDummy	m_Vout1;
	/// on the Catena 4630, there's no Vout1
	cPowerControlDummy 	m_Vout2;
	/// on the Catena 4630, there are no screw terminals
	cPowerControlNested	m_screwTerminal[2] = { cPowerControlNested{this->m_Vout1}, cPowerControlNested{this->m_Vout2} };
	/// on the Catena 4630, there's no control for the 3.3V regulator
	cPowerControlDummy	m_3v3;
	/// on the Catena 4630, there's a TCXO controlled by D33
	cPowerControlGPIO 	m_tcxo = cPowerControlGPIO{ D33, 2 };
	/// on the Catena 4630, flash is always powered
	cPowerControlDummy 	m_flashVdd;
	/// on the Catena 4630, i2c is always powered
	cPowerControlDummy 	m_i2cVdd;
	/// on the Catena 4630, FRAM is always powered
	cPowerControlDummy 	m_framVdd;
	/// on the Catena 4630, there's no external I2C bridge
	cPowerControlDummy 	m_externalI2cBridgeVdd;
	/// on the Catena 4630, the SHT31 has a power switch. Wait 5ms after power on.
	cPowerControlDummy	m_sht3x_vdd;
	/// On the Catena 4630, the SGPC3 must be enabled before you talk to it.
	/// Wait 5ms after power on.
	cPowerControlGPIO	m_sgpc3 = cPowerControlGPIO{ D5, 5 };
	/// the Catena 4630 has a 5V supply
	cPowerControlGPIO	m_5v = cPowerControlGPIO{ D12, 10 };	// wait 10ms
	/// the PMS7003 uses the 5V supply
	cPowerControlNested	m_pms7003 = cPowerControlNested{this->m_5v};

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

/**** end of Catena4630.h ****/
#endif /* _Catena4630_H_ */
