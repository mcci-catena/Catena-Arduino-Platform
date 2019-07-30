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

static uint32_t MeasureMillisPerRtcSecond(void);


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
	This function calibrates the system clock.

	On platforms using this code, the clock is running
	either from the MSI or the HSI clock. These clocks
	are based on an internal resonator, and so they're
	not very accurate.  Over temperature and Vdd
	voltage, they can drift +/- 10%.  This affects
	SYSTICK, and therefore affects all the computed
	timing in the system. So we have to calibrate
	somehow.

	There are two ways to calibrate: either trim the
	oscillator to match a known reference, or measure
	the actual frequency of the system oscilator and
	adjust values read from SYSTICK (and the SYSTICK.LOAD
	register) accordingly.  We chose to adjust the trim
	of the oscillator.

	Calibration values are clock-speed specific. If MSI
	clock is being used, then they range from 0 to 255;
	if HSI is being used, tehn they range from 0 to 31.
	The HSI clock cannot be as finely adjusted.

	The datasheet [section 7.2.15] mentions that you can
	use TIM21, but there's no sample code. We're need to
	do better than we've been doing, so the first version
	of this function runs for several seconds and compares
	millis (driven by system clock) to the output of the
	RTC (driven by LSE).

	In general, this routine works by stepping the clock
	rate up or down until the sign of the error changes.
	Then we use the calibration value that got us closest.

	We recommend that you call this periodically, but
	choose a time when you don't expect to do anything
	for a while, becuase this routine can run for quite
	a while if the calibration is badly off.

Returns:
	New calibration value

Notes:
	This can be quite time consuming in the current
	implementation. But we don't use TIM21, which might
	be considered an advantage, leaving it free for
	other purposes.

*/

uint32_t CatenaStm32L0::CalibrateSystemClock(void)
	{
	uint32_t Calib;
	uint32_t CalibNew;
	uint32_t CalibLow;
	uint32_t CalibHigh;
	uint32_t mSecond;
	uint32_t mSecondNew;
	uint32_t mSecondLow;
	uint32_t mSecondHigh;
	bool fHaveSeenLow;
	bool fHaveSeenHigh;
	const bool fCalibrateMSI = GetSystemClockRate() < 16000000;

	if (fCalibrateMSI)
		{
		Calib = (RCC->ICSCR & RCC_ICSCR_MSITRIM) >> 24;
		}
	else
		{
		Calib = (RCC->ICSCR & RCC_ICSCR_HSITRIM) >> 8;
		}
	gLog.printf(
		gLog.kTrace,
		"+CatenaStm32L0::CalibrateSystemClock: %cSICalib=%u\n",
		fCalibrateMSI ? 'M' : 'H',
		Calib
		);

	/* preapre to loop, setting suitable defaults */
	CalibNew = Calib;
	CalibLow = 0;
	CalibHigh = 0;
	mSecondLow = 0;
	mSecondHigh = 2000;
	fHaveSeenLow = fHaveSeenHigh = false;

	/* loop until we have a new value */
	do	{
		/* meassure the # of millis per RTC second */
		mSecond = MeasureMillisPerRtcSecond();

		/* invariant: */
		if (Calib == CalibNew)
			mSecondNew = mSecond;
		gLog.printf(
			gLog.kTrace,
			" Calib=%u %u msec\n",
			Calib,
			mSecond
			);

		/* if mSecond is low, this meaans we must increase the system clock */
		if (mSecond <= 1000)
			{
			/*
			|| the following condition establishes that we're
			|| below the target frequency, but closer than we've been
			|| before (mSecondLow is the previous "low" limit). If
			|| so, we reduce the limit, and capture the "low" calibration
			|| value.
			*/
			if (mSecond > mSecondLow)
				{
				mSecondLow = mSecond;
				CalibLow = Calib; /* save previous calibration value */
				fHaveSeenLow = true;
				}

			/*
			|| if we are low, and we have never exceeded the high limit,
			|| we can  increase the clock.
			*/
			if (! fHaveSeenHigh)
				{
				if (fCalibrateMSI)
					{
					if (Calib < 0xFF)
						{
						++Calib;
						__HAL_RCC_MSI_CALIBRATIONVALUE_ADJUST(Calib);
						}
					else
						break;
					}
				else
					{
					if (Calib < 0x1F)
						{
						++Calib;
						__HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(Calib);
						}
					else
						{
						break;
						}
					}

				/* let the clock settle */
				delay(500);
				}
			}

		/* if mSecond is high, we must reduce the system clock */
		else
			{
			/*
			|| the following condition establishes that we're
			|| above the target frequency, but closer than we've been
			|| before (mSecondHigh is the previous "high" limit). If
			|| so, we reduce the limit, and capture the calibration
			|| value.
			*/
			if (mSecond < mSecondHigh)
				{
				mSecondHigh = mSecond;
				CalibHigh = Calib;
				fHaveSeenHigh = true;
				}

			/*
			|| if we are above the target frequency, and we have
			|| never raised the frequence, we can lower the
			|| frequency
			*/
			if (! fHaveSeenLow)
				{
				if (Calib == 0)
					break;

				--Calib;
				if (fCalibrateMSI)
					{
					__HAL_RCC_MSI_CALIBRATIONVALUE_ADJUST(Calib);
					}
				else
					{
					__HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(Calib);
					}
				delay(500);
				}
			}
		} while ((Calib != CalibNew) &&
				(! fHaveSeenLow || !fHaveSeenHigh));

	//
	// We are going to take higher calibration value first and
	// it allows us not to call LMIC_setClockError().
	//
	if (fHaveSeenHigh)
		{
		mSecondNew = mSecondHigh;
		CalibNew = CalibHigh;
		}
	else if (fHaveSeenLow)
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
		gLog.kTrace,
		"-CatenaStm32L0::CalibrateSystemClock: %cSICalib=%u %u msec\n",
		fCalibrateMSI ? 'M' : 'H',
		CalibNew,
		mSecondNew
		);
	return CalibNew;
	}

static
uint32_t
MeasureMillisPerRtcSecond(
	void
	)
	{
	uint32_t second;
	uint32_t now;
	uint32_t start;
	uint32_t end;

	/* get the starting time */
	second = RTC->TR & (RTC_TR_ST | RTC_TR_SU);

	/* wait for a new second to start, and capture millis() in start */
	do	{
		now = RTC->TR & (RTC_TR_ST | RTC_TR_SU);
		start = millis();
		} while (second == now);

	/* update our second of interest */
	second = now;

	/* no point in watching the register until we get close */
	delay(500);

	/* wait for the next second to start, and capture millis() */
	do	{
		now = RTC->TR & (RTC_TR_ST | RTC_TR_SU);
		end = millis();
		} while (second == now);

	/* return the delta */
	return end - start;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32L0_CalibrateSystemClock.cpp ****/
