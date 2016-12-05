/* catena_led.h	Sun Dec  4 2016 23:32:54 tmm */

/*

Module:  catena_led.h

Function:
	Simple LED library.

Version:
	V0.4.0	Sun Dec  4 2016 23:32:54 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2016 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	December 2016

Revision history:
   0.4.0  Sun Dec  4 2016 23:32:54  tmm
	Module created.

*/

#ifndef _CATENA_LED_H_		/* prevent multiple includes */
#define _CATENA_LED_H_

#include <type_traits>

// each bit is 128 ms
enum class LedPattern:uint64_t
        {
        Off = 0,
        On = 1,

        OneEigth = 0b100000001,
        OneSixteenth = 0b10000000000000001,
        FastFlash = 0b1010101,
        TwoShort = 0b10000000000000001001,
        FiftyFiftySlow = 0b100000000000000001111111111111111,

        Joining = TwoShort,
        Measuring = FastFlash,
        Sending = FiftyFiftySlow,
        WarmingUp = OneEigth,
        Settling = OneSixteenth,
        };

class StatusLed
        {
private:
        typedef std::underlying_type<LedPattern>::type LedPatternInt;

public:
        StatusLed(uint8_t uPin) : m_Pin(uPin) {};
        void begin(void)
                {
                pinMode(m_Pin, OUTPUT);
                digitalWrite(m_Pin, LOW);
                m_StartTime = millis();
                m_Pattern = LedPattern::Off;
                }
        
        void loop(void)
                {
                uint32_t delta;
                uint32_t now;

                now = millis();
                delta = now ^ m_StartTime;
                if (delta & (1 << 7))
                        {
                        // the 2^7 bit changes every 2^6 ms.
                        m_StartTime = now;

                        this->update();
                        }
                }

        void update(void)
                {
                if (m_Pattern == LedPattern::Off)
                        {
                        digitalWrite(m_Pin, LOW);
                        return;
                        }

                if (m_Current <= 1)
                        m_Current = static_cast<LedPatternInt>(m_Pattern);

                digitalWrite(m_Pin, m_Current & 1);
                m_Current >>= 1;
                }

        LedPattern Set(LedPattern newPattern)
                {
                LedPattern const oldPattern = m_Pattern;

                m_Pattern = newPattern;
                m_Current = 0;
                digitalWrite(m_Pin, LOW);
                return oldPattern;
                }

private:
        uint8_t         m_Pin;
        LedPattern      m_Pattern;
        LedPatternInt   m_Current;
        uint32_t        m_StartTime;
        };


/**** end of catena_led.h ****/
#endif /* _CATENA_LED_H_ */
