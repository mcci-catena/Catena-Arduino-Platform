/* CatenaStm32L0_ReadAnalog.cpp	Wed Mar 27 2019 11:06:04 chwon */

/*

Module:  CatenaStm32L0_ReadAnalog.cpp

Function:
	CatenaStm32L0::ReadAnalog()

Version:
	V0.14.0	Wed Mar 27 2019 11:06:04 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2018-2019 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	December 2018

Revision history:
   0.13.0  Tue Dec 18 2018 15:41:52  chwon
	Module created.

   0.14.0  Mon Feb 18 2019 09:56:32  chwon
	Turn on and off HSI clock if system clock use MSI clock.

   0.14.0  Wed Mar 27 2019 11:06:04  chwon
	Use HAL_RCC_GetHCLKFreq() instead of CATENA_CFG_SYSCLK.

*/

#ifdef ARDUINO_ARCH_STM32

#include "CatenaStm32L0.h"
#include "Catena_Log.h"

#include <Arduino.h>

using namespace McciCatena;

/****************************************************************************\
|
|	Manifest constants & typedefs.
|
|	This is strictly for private types and constants which will not
|	be exported.
|
\****************************************************************************/

constexpr unsigned long ADC_TIMEOUT = 10;
constexpr int STM32L0_ADC_CHANNEL_VREFINT = 17;
constexpr int STM32L0_ADC_CHANNEL_TEMPSENSOR = 18;

static bool AdcCalibrate(void);
static bool AdcDisable(void);
static bool AdcEnable(void);
static bool AdcGetValue(uint32_t *value);
static void AdcStart(void);


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

// because this is defined inside the McciCatena namespace in the header file
// we need to explicitly add the namespace tag here.
bool McciCatena::CatenaStm32L0_ReadAnalog(
	uint32_t Channel,
	uint32_t ReadCount,
	uint32_t Multiplier,
	uint32_t *pValue
	)
	{
	uint32_t	vRef;
	uint32_t	vChannel;
	bool		fStatusOk;
	constexpr uint32_t AdcClockModeAsync = 0;
	constexpr uint32_t AdcClockModePclk2 = ADC_CFGR2_CKMODE_0;
	/* the clock we'll use: */
	constexpr uint32_t AdcClockMode = AdcClockModeAsync;


	if (pValue == NULL)
		return false;

	*pValue = 0;
	if (Channel > STM32L0_ADC_CHANNEL_VREFINT)
		return false;

	/* make sure that the RCC is generating the low-frequency clock */
	__HAL_RCC_ADC1_CLK_ENABLE();

	if (HAL_RCC_GetHCLKFreq() < 16000000)
		{
		/* Set the Low Frequency Mode */
		/* ADC->CCR = ADC_CCR_LFMEN; -- it's not working with MSI clock */

		/* ADC requires HSI clock, so enable it now */
		LL_RCC_HSI_Enable();
		while (LL_RCC_HSI_IsReady() != 1U)
			{
			/* Wait for HSI ready */
			}
		}

	fStatusOk = true;

	if (fStatusOk)
		{
		*pValue = 2;

		// make sure the clock is configured
		// ADC1->CFGR2 = (ADC1->CFGR2 & ~ADC_CFGR2_CKMODE) | AdcClockMode;

		fStatusOk = AdcCalibrate();
		}

	// calibration turns off the ADC. Turn it back on.
	if (fStatusOk)
		{
		*pValue = 3;

		// Set the ADC clock source
		ADC1->CFGR2 = (ADC1->CFGR2 & ~ADC_CFGR2_CKMODE) | AdcClockMode;
		ADC1->CR = ADC_CR_ADVREGEN;
		fStatusOk = AdcEnable();
		}

	if (fStatusOk)
		{
		*pValue = 4;
		ADC1->CFGR1 = ADC_CFGR1_WAIT | ADC_CFGR1_SCANDIR;
		ADC1->CHSELR = (1u << STM32L0_ADC_CHANNEL_VREFINT) |
			       (1u << Channel);
		ADC1->SMPR = ADC_SMPR_SMPR;
		ADC->CCR = ADC_CCR_VREFEN;

		/* start ADC, before each reading */
		AdcStart();

		fStatusOk = AdcGetValue(&vRef);
		if (fStatusOk)
			{
			*pValue = 5;
			if (Channel != STM32L0_ADC_CHANNEL_VREFINT)
				{
				fStatusOk = AdcGetValue(&vChannel);
				}
			else
				{
				vChannel = vRef;
				}
			}

		if (fStatusOk)
			{
			*pValue = 6;

			ADC1->CHSELR = (1u << Channel);
			for (auto i = 1; i < ReadCount; ++i)
				{
				++*pValue;

				AdcStart();
				fStatusOk = AdcGetValue(&vChannel);
				if (! fStatusOk)
					break;
				}
			}
		}

	AdcDisable();
	ADC1->CR &= ~ADC_CR_ADVREGEN;
	ADC->CCR &= ~ADC_CCR_VREFEN;

	if (HAL_RCC_GetHCLKFreq() < 16000000)
		{
		LL_RCC_HSI_Disable();
		}

	__HAL_RCC_ADC1_FORCE_RESET();
	__HAL_RCC_ADC1_RELEASE_RESET();
	__HAL_RCC_ADC1_CLK_DISABLE();

	if (fStatusOk)
		{
		uint16_t *	pVrefintCal;
		uint32_t	vRefInt_cal;
		uint32_t	vResult;

		pVrefintCal = (uint16_t *) (0x1FF80078);
		// add a factor of 4 to vRefInt_cal, we'll take it out below
		vRefInt_cal = (*pVrefintCal * 3000 + 1) / (4096 / 4);

		vResult = vChannel * Multiplier;
	
		vResult = vResult * vRefInt_cal / vRef;
		// remove the factor of 4, and round
		*pValue = (vResult + 2) >> 2;

//		gLog.printf(
//			gLog.kError,
//			"vRef=%u vChannel=%u *pVrefintCal=%u\t",
//			vRef, vChannel, *pVrefintCal
//			);
		}

	return fStatusOk;
	}

static bool AdcDisable(void)
	{
	uint32_t uTime;

	if (ADC1->CR & ADC_CR_ADSTART)
		ADC1->CR |= ADC_CR_ADSTP;

	uTime = millis();
	while (ADC1->CR & ADC_CR_ADSTP)
		{
		if ((millis() - uTime) > ADC_TIMEOUT)
			{
//			gLog.printf(
//				gLog.kError,
//				"?AdcDisable:"
//				" CR=%x\n",
//				ADC1->CR
//				);
			return false;
			}
		}

	ADC1->CR |= ADC_CR_ADDIS;
	uTime = millis();
	while (ADC1->CR & ADC_CR_ADEN)
		{
		if ((millis() - uTime) > ADC_TIMEOUT)
			{
//			gLog.printf(
//				gLog.kError,
//				"?AdcDisable:"
//				" CR=%x\n",
//				ADC1->CR
//				);
			return false;
			}
		}
	return true;
	}

static bool AdcCalibrate(void)
	{
	uint32_t uTime;

	if (ADC1->CR & ADC_CR_ADEN)
		{
		ADC1->CR &= ~ADC_CR_ADEN;

		// if (! AdcDisable())
		// 	return false;
		}

	uTime = millis();

	/* turn on the cal bit */
	ADC1->ISR = ADC_ISR_EOCAL;
	ADC1->CR |= ADC_CR_ADCAL;

	while (! (ADC1->ISR & ADC_ISR_EOCAL))
		{
		if ((millis() - uTime) > ADC_TIMEOUT)
			{
//			gLog.printf(
//				gLog.kError,
//				"?AdcCalibrate:"
//				" CCR=%x CR=%x ISR=%x\n",
//				ADC->CCR,
//				ADC1->CR,
//				ADC1->ISR
//				);
			return false;
			}
		}

	// uint32_t calData = ADC1->DR;

	/* turn off eocal */
	ADC1->ISR = ADC_ISR_EOCAL;
	return true;
	}

static bool AdcEnable(void)
	{
	uint32_t uTime;

	if (ADC1->CR & ADC_CR_ADEN)
		return true;

	/* clear the done bit */
	ADC1->ISR = ADC_ISR_ADRDY;
	ADC1->CR |= ADC_CR_ADEN;

	if (ADC1->CFGR1 & ADC_CFGR1_AUTOFF)
		return true;

	uTime = millis();
	while (!(ADC1->ISR & ADC_ISR_ADRDY))
		{
		if ((millis() - uTime) > ADC_TIMEOUT)
			{
//			gLog.printf(
//				gLog.kError,
//				"?AdcEnable:"
//				" CR=%x ISR=%x\n",
//				ADC1->CR,
//				ADC1->ISR
//				);
			return false;
			}
		}
	return true;
	}

static bool AdcGetValue(uint32_t *value)
	{
	uint32_t rAdcIsr;
	uint32_t uTime;

	uTime = millis();
	while (! ((rAdcIsr = ADC1->ISR) & (ADC_ISR_EOC | ADC_ISR_EOSEQ)))
		{
		if ((millis() - uTime) > ADC_TIMEOUT)
			{
			*value = 0x0FFFu;
//			gLog.printf(
//				gLog.kError,
//				"?AdcGetValue:"
//				" ISR=%x\n",
//				rAdcIsr
//				);
			return false;
			}
		}

	/* rarely, EOSEQ gets set early... so wait if needed */
	if (! (rAdcIsr & ADC_ISR_EOC))
		{
		for (unsigned i = 0; i < 256u; ++i)
			{
			if ((rAdcIsr = ADC1->ISR) & ADC_ISR_EOC)
				break;
			}
		}

	if (rAdcIsr & ADC_ISR_EOC)
		{
		*value = ADC1->DR;

		/* clear the EOC flag post ADC data read to start new read.
		In wait mode, clearing EOC flag helps to match speed of ADC to
		speed of system clock */
		ADC1->ISR &= ~ADC_ISR_EOC;
		return true;
		}

//	gLog.printf(
//		gLog.kError,
//		"?AdcGetValue:"
//		" ISR=%x\n",
//		rAdcIsr
//		);
	// no more data in this sequence
	*value = 0x0FFFu;
	return false;
	}

static void AdcStart(void)
	{
	ADC1->ISR = (ADC_ISR_EOC | ADC_ISR_EOSEQ);
	ADC1->CR |= ADC_CR_ADSTART;
	}

/*

Name:	CatenaStm32L0::ReadAnalog

Function:
	Read analog value for given channel

Definition:
	uint32_t CatenaStm32L0::ReadAnalog(
		uint32_t Channel,
		uint32_t ReadCount,
		uint32_t Multiplier
		);

Description:
	This function reads the analog value for the given channel.

Returns:
	Analog value

*/

uint32_t CatenaStm32L0::ReadAnalog(
	uint32_t Channel,
	uint32_t ReadCount,
	uint32_t Multiplier
	) const
	{
	uint32_t	vResult;
	bool		fStatusOk;

	fStatusOk = CatenaStm32L0_ReadAnalog(
			Channel,
			ReadCount,
			Multiplier,
			&vResult
			);
	if (! fStatusOk)
		{
		gLog.printf(
			gLog.kError,
			"?CatenaStm32L0::ReadAnalog(%u):"
			" CatenaStm32L0_ReadAnalog() failed (%u)\n",
			Channel,
			vResult
			);
		vResult = 0x0FFFu;
		}

	return vResult;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32L0_ReadAnalog.cpp ****/
