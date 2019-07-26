/*

Module:  CatenaStm32L0_CalibrateSystemClock.cpp

Function:
	Home for CatenaStm32L0::CalibrateSystemClock()

Copyright notice:
	See accompanying LICENSE file.

Author:
	ChaeHee Won, MCCI Corporation	October 2018

*/

#ifdef ARDUINO_ARCH_STM32

#include "CatenaStm32L0.h"
#include "Catena_Log.h"

using namespace McciCatena;

/****************************************************************************\
|
|	Manifest constants & typedefs.
|
\****************************************************************************/

static uint32_t GetMillisPerSecond(void);


/****************************************************************************\
|
|	Read-only data.
|
\****************************************************************************/



/****************************************************************************\
|
|	Variables.
|
\****************************************************************************/


/*

Name:	CatenaStm32L0::CalibrateSystemClock()

Function:
	Calibrate system clock

Definition:
	uint32_t CatenaStm32L0::CalibrateSystemClock(
		bool fSetCalibVal,
		uint32_t CalibVal
		);

Description:
	This function calibrates system clock.

Returns:
	New calibration value

*/

uint32_t CatenaStm32L0::CalibrateSystemClock(
	bool fSetCalibVal,
	uint32_t CalibVal
	)
	{
	uint32_t Calib;
	uint32_t CalibNew;
	uint32_t CalibLow;
	uint32_t CalibHigh;
	uint32_t mSecond;
	uint32_t mSecondNew;
	uint32_t mSecondLow;
	uint32_t mSecondHigh;
	bool fCalibrateMSI;

	fCalibrateMSI = GetSystemClockRate() < 16000000;
	if (fCalibrateMSI)
		{
		Calib = (RCC->ICSCR & RCC_ICSCR_MSITRIM) >> 24;
		}
	else
		{
		Calib = (RCC->ICSCR & RCC_ICSCR_HSITRIM) >> 8;
		}
	gLog.printf(
		gLog.kAlways,
		"+CatenaStm32L0::CalibrateSystemClock: %cSICalib=%u\n",
		fCalibrateMSI ? 'M' : 'H',
		Calib
		);

	if (fSetCalibVal)
		{
		if (fCalibrateMSI)
			{
			__HAL_RCC_MSI_CALIBRATIONVALUE_ADJUST(CalibVal);
			}
		else
			{
			__HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(CalibVal);
			}
		delay(500);
		CalibNew = CalibVal;
		mSecondNew = GetMillisPerSecond();
		}
	else
		{
		CalibNew = Calib;
		CalibLow = 0;
		CalibHigh = 0;
		mSecondLow = 0;
		mSecondHigh = 2000;

		do	{
			mSecond = GetMillisPerSecond();
			if (Calib == CalibNew)
				mSecondNew = mSecond;
			gLog.printf(
				gLog.kAlways,
				" Calib=%u %u msec\n",
				Calib,
				mSecond
				);
			if (mSecond <= 1000)
				{
				if (mSecond > mSecondLow)
					{
					mSecondLow = mSecond;
					CalibLow = Calib;
					}
				if (CalibHigh == 0)
					{
					if (fCalibrateMSI)
					  {
					  Calib = (Calib + 1) & 0xFFu;
					  __HAL_RCC_MSI_CALIBRATIONVALUE_ADJUST(Calib);
					  }
					else
					  {
					  Calib = (Calib + 1) & 0x1Fu;
					  __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(Calib);
					  }
					delay(500);
					}
				}
			else
				{
				if (mSecond < mSecondHigh)
					{
					mSecondHigh = mSecond;
					CalibHigh = Calib;
					}
				if (CalibLow == 0)
					{
					if (fCalibrateMSI)
					  {
					  Calib = (Calib - 1) & 0xFFu;
					  __HAL_RCC_MSI_CALIBRATIONVALUE_ADJUST(Calib);
					  }
					else
					  {
					  Calib = (Calib - 1) & 0x1Fu;
					  __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(Calib);
					  }
					delay(500);
					}
				}
			} while ((Calib != CalibNew) &&
				 (CalibLow == 0 || CalibHigh == 0));

		//
		// We are going to take higher calibration value first and
		// it allows us not to call LMIC_setClockError().
		// 
		if (CalibHigh != 0)
			{
			mSecondNew = mSecondHigh;
			CalibNew = CalibHigh;
			}
		else if (CalibLow != 0)
			{
			mSecondNew = mSecondLow;
			CalibNew = CalibLow;
			}
		else
			{
			// Use original value
			gLog.printf(
				gLog.kError,
				"?CatenaStm32L0::CalibrateSystemClock: can't calibrate\n"
				);
			}
		}

	if (CalibNew != Calib)
		{
		if (fCalibrateMSI)
			{
			__HAL_RCC_MSI_CALIBRATIONVALUE_ADJUST(CalibNew);
			}
		else
			{
			__HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(CalibNew);
			}
		delay(500);
		}

	gLog.printf(
		gLog.kAlways,
		"-CatenaStm32L0::CalibrateSystemClock: %cSICalib=%u %u msec\n",
		fCalibrateMSI ? 'M' : 'H',
		CalibNew,
		mSecondNew
		);
	return CalibNew;
	}

static
uint32_t
GetMillisPerSecond(
	void
	)
	{
	uint32_t second;
	uint32_t now;
	uint32_t start;
	uint32_t end;

	second = RTC->TR & (RTC_TR_ST | RTC_TR_SU);

	do	{
		now = RTC->TR & (RTC_TR_ST | RTC_TR_SU);
		start = millis();
		} while (second == now);

	second = now;
	delay(500);

	do	{
		now = RTC->TR & (RTC_TR_ST | RTC_TR_SU);
		end = millis();
		} while (second == now);

	return end - start;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32L0_CalibrateSystemClock.cpp ****/
