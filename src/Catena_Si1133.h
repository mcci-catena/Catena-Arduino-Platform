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
public:
        Catena_Si1133(void);

        boolean begin(uint8_t DeviceAddress = CATENA_SI1133_ADDRESS,
		      TwoWire *pWire = &Wire);

	boolean configure(
			uint8_t uChannel,
			uint8_t uMode,
			uint8_t uMeasurementCount = 0	/* use default */
			);

	boolean start(boolean fOneTime = false);
	boolean stop(void);

	uint16_t readChannelData(uint8_t uChannel = 0);
	void readMultiChannelData(uint16_t *pChannelData, uint32_t nChannel);

	uint8_t readReg(uint8_t uReg);
	uint8_t readParam(uint8_t uParam);

	void writeReg(uint8_t uReg, uint8_t uData);
	void writeParam(uint8_t uReg, uint8_t uData);

private:
	uint8_t m_DeviceAddress;
	uint8_t	m_ChannelEnabled;
	uint8_t	m_ChannelDataReg[SI1133_NUM_CHANNEL];
	boolean	m_Initialized;
	boolean	m_fOneTime;
	TwoWire *m_pWire;
	uint8_t	getResponse(void);
	uint8_t	waitResponse(uint8_t uResponse);
	};

} // namespace McciCatena

/**** end of Catena_Si1133.h ****/
#endif /* _CATENA_SI1133_H_ */
