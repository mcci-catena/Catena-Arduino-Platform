/* CatenaStm32L0.cpp	Wed Jan 03 2018 11:03:38 chwon */

/*

Module:  CatenaStm32L0.cpp

Function:
	CatenaStm32L0::ReadVbat()

Version:
	V0.7.0	Wed Jan 03 2018 11:03:39 chwon	Edit level 2

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

   0.7.0  Wed Jan 03 2018 11:03:39  chwon
	Remove multiply 2.

*/

#ifdef ARDUINO_ARCH_STM32

#include "CatenaStm32L0.h"

#include "Arduino.h"
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

float
CatenaStm32L0::ReadVbat(void) const
	{
	float rawVoltage = analogRead(CatenaStm32L0::APIN_VBAT_SENSE);
	return rawVoltage * 3.3 / 1024;
	}

float
CatenaStm32L0::ReadVbus(void) const
	{
	float rawVoltage = analogRead(CatenaStm32L0::APIN_VBUS_SENSE);
	return rawVoltage * 3.3 / 1024;
	}

#ifdef __cplusplus
extern "C" {
#endif

/*

Name:	yield

Function:
	Yield CPU

Definition:
	void yield(void);

Description:
	This function override default week yield() function in the cores/
	arduino/hooks.c file. It requests wait for interrupt.

Returns:
	No explicit result.

Notes:
	We need to keep this function in this file because linker should link
	this yield function in this file. So do not make this yield function
	in separate file.

*/

void yield(void)
	{
	/* Read PRIMASK register, check interrupt status before disable them */
	/* Returns 0 if they are enabled, or non-zero if disabled */
	if (__get_PRIMASK())
		{
#if 1
		if (SCB->ICSR & SCB_ICSR_PENDSTSET_Msk)
			{
			SCB->ICSR = SCB_ICSR_PENDSTCLR_Msk;
			HAL_IncTick();
			}
#else
		if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
			HAL_IncTick();
#endif
		}
	else
		{
		/* Request Wait For Interrupt */
		__WFI();
		}
	}

uint32_t HAL_GetTick(void)
	{
	extern __IO uint32_t uwTick;

	/* Read PRIMASK register, check interrupt status before disable them */
	/* Returns 0 if they are enabled, or non-zero if disabled */
	if (__get_PRIMASK())
		{
#if 1
		if (SCB->ICSR & SCB_ICSR_PENDSTSET_Msk)
			{
			SCB->ICSR = SCB_ICSR_PENDSTCLR_Msk;
			HAL_IncTick();
			}
#else
		if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
			HAL_IncTick();
#endif
		}

	return uwTick;
	}

#ifdef __cplusplus
}
#endif

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32L0.cpp ****/
