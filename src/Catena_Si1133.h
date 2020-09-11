/* Catena_Si1133.h	Fri Oct 13 2017 15:19:30 chwon */

/*

Module:  Catena_Si1133.h

Function:
	Class Catena_Si1133 (RTC wrapper for Catena)

Version:
	V0.6.0	Fri Oct 13 2017 15:19:30 chwon	Edit level 1

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

*/

#ifndef _CATENA_SI1133_H_		/* prevent multiple includes */
#define _CATENA_SI1133_H_

#include <Arduino.h>
#include <Wire.h>
#include <cstdint>

/* TODO: change these to enums and constepxrs in the McciCatena namespace */

#define	CATENA_SI1133_ADDRESS		(0x55u)

/* Si1133 Registers */
#define	SI1133_REG_PART_ID		0x00u
#define	SI1133_REG_HW_ID		0x01u
#define	SI1133_REG_REV_ID		0x02u
#define	SI1133_REG_INFO0		0x03u
#define	SI1133_REG_INFO1		0x04u
#define	SI1133_REG_HOSTIN3		0x07u
#define	SI1133_REG_HOSTIN2		0x08u
#define	SI1133_REG_HOSTIN1		0x09u
#define	SI1133_REG_HOSTIN0		0x0Au
#define	SI1133_REG_COMMAND		0x0Bu
#define	SI1133_REG_IRQ_ENABLE		0x0Fu
#define	SI1133_REG_RESPONSE1		0x10u
#define	SI1133_REG_RESPONSE0		0x11u
#define	SI1133_REG_IRQ_STATUS		0x12u
#define	SI1133_REG_HOSTOUT0		0x13u
#define	SI1133_REG_HOSTOUT1		0x14u
#define	SI1133_REG_HOSTOUT2		0x15u
#define	SI1133_REG_HOSTOUT3		0x16u
#define	SI1133_REG_HOSTOUT4		0x17u
#define	SI1133_REG_HOSTOUT5		0x18u
#define	SI1133_REG_HOSTOUT6		0x19u
#define	SI1133_REG_HOSTOUT7		0x1Au
#define	SI1133_REG_HOSTOUT8		0x1Bu
#define	SI1133_REG_HOSTOUT9		0x1Cu
#define	SI1133_REG_HOSTOUT10		0x1Du
#define	SI1133_REG_HOSTOUT11		0x1Eu
#define	SI1133_REG_HOSTOUT12		0x1Fu
#define	SI1133_REG_HOSTOUT13		0x20u
#define	SI1133_REG_HOSTOUT14		0x21u
#define	SI1133_REG_HOSTOUT15		0x22u
#define	SI1133_REG_HOSTOUT16		0x23u
#define	SI1133_REG_HOSTOUT17		0x24u
#define	SI1133_REG_HOSTOUT18		0x25u
#define	SI1133_REG_HOSTOUT19		0x26u
#define	SI1133_REG_HOSTOUT20		0x27u
#define	SI1133_REG_HOSTOUT21		0x28u
#define	SI1133_REG_HOSTOUT22		0x29u
#define	SI1133_REG_HOSTOUT23		0x2Au
#define	SI1133_REG_HOSTOUT24		0x2Bu
#define	SI1133_REG_HOSTOUT25		0x2Cu

/* Si1133 Parameters */
#define	SI1133_PARAM_I2C_ADDR		0x00u
#define	SI1133_PARAM_CHAN_LIST		0x01u
#define	SI1133_PARAM_ADCCONFIG0		0x02u
#define	SI1133_PARAM_ADCSENS0		0x03u
#define	SI1133_PARAM_ADCPOST0		0x04u
#define	SI1133_PARAM_MEASCONFIG0	0x05u
#define	SI1133_PARAM_ADCCONFIG1		0x06u
#define	SI1133_PARAM_ADCSENS1		0x07u
#define	SI1133_PARAM_ADCPOST1		0x08u
#define	SI1133_PARAM_MEASCONFIG1	0x09u
#define	SI1133_PARAM_ADCCONFIG2		0x0Au
#define	SI1133_PARAM_ADCSENS2		0x0Bu
#define	SI1133_PARAM_ADCPOST2		0x0Cu
#define	SI1133_PARAM_MEASCONFIG2	0x0Du
#define	SI1133_PARAM_ADCCONFIG3		0x0Eu
#define	SI1133_PARAM_ADCSENS3		0x0Fu
#define	SI1133_PARAM_ADCPOST3		0x10u
#define	SI1133_PARAM_MEASCONFIG3	0x11u
#define	SI1133_PARAM_ADCCONFIG4		0x12u
#define	SI1133_PARAM_ADCSENS4		0x13u
#define	SI1133_PARAM_ADCPOST4		0x14u
#define	SI1133_PARAM_MEASCONFIG4	0x15u
#define	SI1133_PARAM_ADCCONFIG5		0x16u
#define	SI1133_PARAM_ADCSENS5		0x17u
#define	SI1133_PARAM_ADCPOST5		0x18u
#define	SI1133_PARAM_MEASCONFIG5	0x19u
#define	SI1133_PARAM_MEASRATE_H		0x1Au
#define	SI1133_PARAM_MEASRATE_L		0x1Bu
#define	SI1133_PARAM_MEASCOUNT0		0x1Cu
#define	SI1133_PARAM_MEASCOUNT1		0x1Du
#define	SI1133_PARAM_MEASCOUNT2		0x1Eu
#define	SI1133_PARAM_THRESHOLD0_H	0x25u
#define	SI1133_PARAM_THRESHOLD0_L	0x26u
#define	SI1133_PARAM_THRESHOLD1_H	0x27u
#define	SI1133_PARAM_THRESHOLD1_L	0x28u
#define	SI1133_PARAM_THRESHOLD2_H	0x29u
#define	SI1133_PARAM_THRESHOLD2_L	0x2Au
#define	SI1133_PARAM_BURST		0x2Bu

#define	SI1133_NUM_CHANNEL		6

/* Si1133 Command Register Values */
#define	SI1133_CMD_RESET_CMD_CTR	0x00u
#define	SI1133_CMD_RESET_SW		0x01u
#define	SI1133_CMD_FORCE		0x11u
#define	SI1133_CMD_PAUSE		0x12u
#define	SI1133_CMD_START		0x13u
#define	SI1133_CMD_PARAM_QUERY		0x40u
#define	SI1133_CMD_PARAM_SET		0x80u

/* Si1133 Response0 Register Values */
#define	SI1133_RSP_CMD_CTR_MASK		0x0Fu
#define	SI1133_RSP_CMD_ERR		(1u << 4)
#define	SI1133_RSP_CMD_SLEEP		(1u << 5)
#define	SI1133_RSP_CMD_SUSPEND		(1u << 6)
#define	SI1133_RSP_CMD_RUNNING		(1u << 7)

/* configure uMode parameter values */
#define	CATENA_SI1133_MODE_NotUsed	0
#define	CATENA_SI1133_MODE_SmallIR	1
#define	CATENA_SI1133_MODE_MediumIR	2
#define	CATENA_SI1133_MODE_LargeIR	3
#define	CATENA_SI1133_MODE_White	4
#define	CATENA_SI1133_MODE_LargeWhite	5
#define	CATENA_SI1133_MODE_UV		6
#define	CATENA_SI1133_MODE_UVDeep	7

#ifndef	CATENA_SI1133_MEASUREMENT_RATE	/* PARAM */
# define CATENA_SI1133_MEASUREMENT_RATE	125	/* 125 * 800us = 100ms */
#endif

#ifndef	CATENA_SI1133_MEASUREMENT_COUNT	/* PARAM */
# define CATENA_SI1133_MEASUREMENT_COUNT 5	/* 5 * 100ms = 500ms */
#endif

namespace McciCatena {

class Catena_Si1133
	{
private:
	static constexpr uint32_t kPollDelayMs = 5;
	static constexpr uint32_t kStartDelayMs = 200;

	static constexpr uint32_t adcconfig_adcmux 		= UINT32_C(0x1F) << 0;
	static constexpr uint32_t adcconfig_decim_rate 		= UINT32_C(3) << 5;

	static constexpr uint32_t adcsense_hw_gain 		= UINT32_C(0xF) << (0 + 8);
	static constexpr uint32_t adcsense_sw_gain 		= UINT32_C(0x7) << (4 + 8);
	static constexpr uint32_t adcsense_hsig 		= UINT32_C(1) << (7 + 8);

	static constexpr uint32_t adcpost_thresh_en 		= UINT32_C(0x3) << (0 + 16);
	static constexpr uint32_t adcpost_postshift 		= UINT32_C(0x7) << (3 + 16);
	static constexpr uint32_t adcpost_24bit 		= UINT32_C(1) << (6 + 16);

	static constexpr uint32_t measconfig_counter_index 	= UINT32_C(3) << (6 + 24);

public:
	enum class InputLed_t : uint8_t
		{
		SmallIR		= 0b00000,
		MediumIR 	= 0b00001,
		LargeIR		= 0b00010,
		White 		= 0b01011,
		LargeWhite 	= 0b01101,
		UV		= 0b11000,
		UVdeep		= 0b11001,
		Disabled	= 0b11111,
		};
	static constexpr bool isValid(InputLed_t v)
		{
		return (v <= InputLed_t::Disabled) &&
		       ((UINT32_C(1) << uint8_t(v)) & 
		       		((1u << uint8_t(InputLed_t::SmallIR)) |
				 (1u << uint8_t(InputLed_t::MediumIR)) |
				 (1u << uint8_t(InputLed_t::LargeIR)) |
				 (1u << uint8_t(InputLed_t::White)) |
				 (1u << uint8_t(InputLed_t::LargeWhite)) |
				 (1u << uint8_t(InputLed_t::UV)) |
				 (1u << uint8_t(InputLed_t::UVdeep)) |
				 (1u << uint8_t(InputLed_t::Disabled))));
		}
	
public:
	Catena_Si1133(void);

	// neither copyable nor movable
	Catena_Si1133(const Catena_Si1133&) = delete;
	Catena_Si1133& operator=(const Catena_Si1133&) = delete;
	Catena_Si1133(const Catena_Si1133&&) = delete;
	Catena_Si1133& operator=(const Catena_Si1133&&) = delete;

	static constexpr uint32_t clocksToMicroseconds(uint32_t nClocks) {
		return ((nClocks * 2) / 21 + 1) / 2;
		}
	static constexpr uint32_t microsecondsToClocks(uint32_t us) {
		return us * 21;
		}

	//! \brief Configuration object for SI1133 setup
	class ChannelConfiguration_t
		{
	private:
		uint32_t	m_value;

		template <typename T>
		static constexpr T fieldlsb(T fmask) { return ((fmask) & (~(fmask) + T(1))); }
		template <typename T>
		static constexpr T fieldvalue(T fmask, T val) { return (fieldlsb(fmask) * val) & fmask; }
		template <typename T>
		static constexpr T fieldget(T fmask, T val) { return (val & fmask) / fieldlsb(fmask); }
		template <typename T, typename TV>
		static constexpr T fieldset(T fmask, T val, TV fv) { return (val & ~fmask) | (fv * fieldlsb(fmask)); }

		template <typename T>
		ChannelConfiguration_t& setValue(uint32_t fmask, T value) {
			this->m_value = fieldset(fmask, this->m_value, value);
			return *this;
		}

	public:
		uint32_t getValue() const { return this->m_value; };

		ChannelConfiguration_t(uint32_t v = 0)
			: m_value(v)
			{}

		ChannelConfiguration_t& setDecimationRateCode(uint8_t rate) {
			return this->setValue(adcconfig_decim_rate, rate);
			}
		uint8_t getDecimationRateCode() const { return fieldget(adcconfig_decim_rate, this->m_value); }
		static constexpr uint32_t decimationRateCodeToClocks(uint8_t ratecode) {
			return ((ratecode & 3) == 0) ? 1024 :
			       ((ratecode & 3) == 1) ? 2048 :
			       ((ratecode & 3) == 2) ? 4096 :
			       				512;
			}
		ChannelConfiguration_t& setAdcMux(InputLed_t channel) {
			return this->setValue(adcconfig_adcmux, uint8_t(channel));
			}
		InputLed_t getAdcMux() { return InputLed_t(fieldget(adcconfig_adcmux, this->m_value)); }

		ChannelConfiguration_t& setSwGainCode(uint8_t gc) {
			return this->setValue(adcsense_sw_gain, gc);
			}
		uint8_t getSwGainCode() { return fieldget(adcsense_sw_gain, this->m_value); }			

		ChannelConfiguration_t& setHwGainCode(uint8_t gc) {
			return this->setValue(adcsense_hw_gain, gc);
			}
		uint8_t getHwGainCode() { return fieldget(adcsense_hw_gain, this->m_value); }			

		ChannelConfiguration_t& setHsig(bool fHighRange) {
			return this->setValue(adcsense_hsig, fHighRange);
			}
		bool getHsig() { return fieldget(adcsense_hsig, this->m_value); }			

		enum class Threshold_t : uint8_t {
			None = 0,
			Threshold0,
			Threshold1, 
			Threshold2
			};
		
		ChannelConfiguration_t& setInterruptThreshold(Threshold_t nBits) {
			return this->setValue(adcpost_thresh_en, uint8_t(nBits));
			}
		Threshold_t getInterruptThreshold() const { return Threshold_t(fieldget(adcpost_postshift, this->m_value)); }

		ChannelConfiguration_t& setPostShift(uint8_t nBits) {
			return this->setValue(adcpost_postshift, nBits);
			}
		uint8_t getPostShift() const { return fieldget(adcpost_postshift, this->m_value); }

		ChannelConfiguration_t& set24bit(bool f24bit = true) {
			return this->setValue(adcpost_24bit, f24bit);
			}
		bool get24bit() const { return fieldget(adcpost_24bit, this->m_value); }

		enum class CounterSelect_t : uint8_t {
			None = 0,
			MeasCount1,
			MeasCount2,
			MeasCount3
			};

		ChannelConfiguration_t& setCounter(CounterSelect_t c) {
			return this->setValue(measconfig_counter_index, uint8_t(c));
			}
		CounterSelect_t getCounter() const { return CounterSelect_t(fieldget(measconfig_counter_index, this->m_value)); }
		};


        boolean begin(uint8_t DeviceAddress = CATENA_SI1133_ADDRESS,
		      TwoWire *pWire = &Wire);

	bool configure(
		uint8_t uChannel,			//! channel in 0..5
		ChannelConfiguration_t cc,		//! configuration value
		uint8_t uMeasurementCount		//! 0: forced mode, otherwise count of measurements
		);

	/// \brief fully configure the measurement for a given
	///	channel. Must be called in ascending order of channel
	///	to simplify measurement.
	///
	/// \note This is a legacy API; we recommend using the `ChannelConfiguration_t`
	///	variant for new code.
	///
	/// \param uChannel is the sequence of the measrurement
	///	in the Si1133 output buffer.
	///
	/// \param uMode is an index into 
	boolean configure(
		uint8_t uChannel,
		uint8_t uMode,
		uint8_t uMeasurementCount = 0	/* use forced mode */
		);

	/// \brief start a measurement.
	/// \param fOneTime controls whether this is a one-time (if `true`)
	///	or continuous (if `false` or omitted) measurement.
	boolean start(boolean fOneTime = false);

	/// \brief stop an ongoing measurement.
	boolean stop(void);

	/// \brief read a single channel o
	uint32_t readChannelData(uint8_t uChannel = 0);
	bool readMultiChannelData(uint16_t *pChannelData, uint32_t nChannel) const;
	bool readMultiChannelData(uint32_t *pChannelData, uint32_t nChannel) const;

	uint8_t readReg(uint8_t uReg);
	uint8_t readParam(uint8_t uParam);

	void writeReg(uint8_t uReg, uint8_t uData);
	void writeParam(uint8_t uReg, uint8_t uData);

	bool isOneTimeReady(void);

private:
	uint8_t m_DeviceAddress;
	/// bitmap of enabled channels.
	uint8_t	m_ChannelEnabled;
	uint8_t m_ChannelCompleted;
	uint8_t	m_ChannelDataReg[SI1133_NUM_CHANNEL];
	/// array of channel configuration info.
	ChannelConfiguration_t m_config[SI1133_NUM_CHANNEL];
	bool	m_Initialized : 1;
	bool	m_fOneTime : 1;
	bool	m_fDataRegValid : 1;
	std::uint32_t m_StartTime;
	std::uint32_t m_LastPollTime;
	TwoWire *m_pWire;
	uint8_t	getResponse(void);
	uint8_t	waitResponse(uint8_t uResponse);
	};

} // namespace McciCatena

/**** end of Catena_Si1133.h ****/
#endif /* _CATENA_SI1133_H_ */
