/* Catena_TxBuffer.h	Sat Feb 03 2018 16:48:02 vel */

/*

Module:  Catena_TxBuffer.h

Function:
	namespace McciCatena, class TxBuffer_t;

Version:
	V0.8.0	Sat Feb 03 2018 16:48:02 vel	Edit level 4

Copyright notice:
	This file copyright (C) 2016-2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	December 2016

Revision history:
   0.4.0  Mon Dec  5 2016 00:02:11  tmm
	Module created.

   0.6.1  Wed Nov 08 2017 22:35:02  tmm
        #7: Fix error in handling of negative args to 
        TxBuffer_t::put2(int32_t) and TxBuffer_t::put2(int32_t).

   0.8.0 Sat Feb 03 2018 16:48:02 vel
    	Added 0x16 Format.

*/

#ifndef _CATENA_TXBUFFER_H_		/* prevent multiple includes */
#define _CATENA_TXBUFFER_H_

#include <cstdint>

namespace McciCatena {

// build a transmit buffer
class TxBuffer_t
        {
private:
        uint8_t buf[32];   // this sets the largest buffer size
        uint8_t *p;
public:
        TxBuffer_t() : p(buf) {};
        void begin()
                {
                p = buf;
                }
        void put(uint8_t c)
                {
                if (p < buf + sizeof(buf))
                        *p++ = c;
                }
        void put1u(int32_t v)
                {
                if (v > 0xFF)
                        v = 0xFF;
                else if (v < 0)
                        v = 0;
                put((uint8_t) v);
                }
        void put2(uint32_t v)
                {
                if (v > 0xFFFF)
                        v = 0xFFFF;

                put((uint8_t) (v >> 8));
                put((uint8_t) v);
                }
        void put2(int32_t v)
                {
                if (v < -0x8000)
                        v = -0x8000;
                else if (v > 0x7FFF)
                        v = 0x7FFF;

                put((uint8_t)(v >> 8));
                put((uint8_t)v);
                }
        void put3(uint32_t v)
                {
                if (v > 0xFFFFFF)
                        v = 0xFFFFFF;

                put((uint8_t) (v >> 16));
                put((uint8_t) (v >> 8));
                put((uint8_t) v);
                }
        void put2u(int32_t v)
                {
                if (v < 0)
                        v = 0;
                else if (v > 0xFFFF)
                        v = 0xFFFF;
                put2((uint32_t) v);
                }
        void put3(int32_t v)
                {
                if (v < -0x800000)
                        v = -0x800000;
                else if (v > 0x7FFFFF)
                        v = 0x7FFFFF;

                put((uint8_t)(v >> 16));
                put((uint8_t)(v >> 8));
                put((uint8_t)v);
        }
        uint8_t *getp(void)
                {
                return p;
                }
        size_t getn(void)
                {
                return p - buf;
                }
        uint8_t *getbase(void)
                {
                return buf;
                }
        void put2sf(float v)
                {
                int32_t iv;

                if (v > 32766.5f)
                        iv = 0x7fff;
                else if (v < -32767.5f)
                        iv = -0x8000;
                else
                        iv = (int32_t)(v + 0.5f);

                put2(iv);
                }
        void put2uf(float v)
                {
                uint32_t iv;

                if (v > 65535.5f)
                        iv = 0xffff;
                else if (v < 0.5f)
                        iv = 0;
                else
                        iv = (uint32_t)(v + 0.5f);

                put2(iv);
                }
        void put1uf(float v)
                {
                uint8_t c;

                if (v > 254.5)
                        c = 0xFF;
                else if (v < 0.5)
                        c = 0;
                else
                        c = (uint8_t) v;

                put(c);
                }
        void putT(float T)
                {
                put2sf(T * 256.0f + 0.5f);                
                }
        void putRH(float RH)
                {
                put1uf((RH / 0.390625f) + 0.5f);
                }
        void putV(float V)
                {
                put2sf(V * 4096.0f + 0.5f);
                }
        void putP(float P)
                {
                put2uf(P / 4.0f + 0.5f);
                }
        void putLux(float Lux)
                {
                put2uf(Lux);
                }
        void putLux(uint16_t Lux)
                {
                put2(uint32_t(Lux));
                }
        void putBootCountLsb(uint32_t bootCount)
                {
                put(uint8_t(bootCount & 0xFF));
                }
        void putWH(uint32_t wattHour)
                {
                put2(uint32_t(wattHour & 0xFFFF));
                }
        void putPulseFraction(uint16_t fracAsFloat12)
                {
                put2u(fracAsFloat12);
                }
        };

/* the magic byte at the front of the buffer */
enum : uint8_t   {
        FormatSensor1 = 0x11,
        FormatSensor2 = 0x14,
        FormatSensor3 = 0x15,
        FormatSensor4 = 0x16,
        FormatSensor5 = 0x17,
        };

/* the flags for the second byte of the buffer */
enum  FlagsSensor1 : uint8_t  {
        FlagVbat = 1 << 0,
        FlagVcc = 1 << 1,
        FlagTPH = 1 << 2,
        FlagLux = 1 << 3,
        FlagWater = 1 << 4,
        FlagSoilTH = 1 << 5,
        };

enum class FlagsSensor2 : uint8_t 
        {
        FlagVbat = 1 << 0,
        FlagVcc = 1 << 1,
        FlagBoot = 1 << 2,
        FlagTPH = 1 << 3,
        FlagLux = 1 << 4,
        FlagWattHours = 1 << 5,
        FlagPulsesPerHour = 1 << 6,
        };

constexpr FlagsSensor2 operator| (const FlagsSensor2 lhs, const FlagsSensor2 rhs)
        {
        return FlagsSensor2(uint8_t(lhs) | uint8_t(rhs));
        };

FlagsSensor2 operator|= (FlagsSensor2 &lhs, const FlagsSensor2 &rhs)
        {
        lhs = lhs | rhs;
        return lhs;
        };

enum class FlagsSensor3 : uint8_t 
        {
        FlagVbat = 1 << 0,
        FlagVcc = 1 << 1,
        FlagBoot = 1 << 2,
        FlagTPH = 1 << 3,
        FlagLux = 1 << 4,
        FlagWater = 1 << 5,
        FlagSoilTH = 1 << 6,
        };

constexpr FlagsSensor3 operator| (const FlagsSensor3 lhs, const FlagsSensor3 rhs)
        {
        return FlagsSensor3(uint8_t(lhs) | uint8_t(rhs));
        };

FlagsSensor3 operator|= (FlagsSensor3 &lhs, const FlagsSensor3 &rhs)
        {
        lhs = lhs | rhs;
        return lhs;
        };

enum class FlagsSensor4 : uint8_t 
        {
        FlagVbat = 1 << 0,
        FlagVcc = 1 << 1,
        FlagBoot = 1 << 2,
        FlagTPH = 1 << 3,
        FlagLux = 1 << 4,
        FlagWaterPressure = 1 << 5,
        };

constexpr FlagsSensor4 operator| (const FlagsSensor4 lhs, const FlagsSensor4 rhs)
        {
        return FlagsSensor4(uint8_t(lhs) | uint8_t(rhs));
        };

FlagsSensor4 operator|= (FlagsSensor4 &lhs, const FlagsSensor4 &rhs)
        {
        lhs = lhs | rhs;
        return lhs;
        };
	
enum class FlagsSensor5 : uint8_t
        {
        FlagVbat = 1 << 0,
        FlagVcc = 1 << 1,
        FlagBoot = 1 << 2,
        FlagTPH = 1 << 3,
        FlagLux = 1 << 4,
        FlagAqi = 1 << 5,
        FlagLogGasR = 1 << 6,
	FlagAqiAccuracyMisc = 1 << 7,
        };

constexpr FlagsSensor5 operator| (const FlagsSensor5 lhs, const FlagsSensor5 rhs)
        {
        return FlagsSensor5(uint8_t(lhs) | uint8_t(rhs));
        };

FlagsSensor5 operator|= (FlagsSensor5 &lhs, const FlagsSensor5 &rhs)
        {
        lhs = lhs | rhs;
        return lhs;
        };


} /* namespace McciCatena */

/**** end of Catena_TxBuffer.h ****/
#endif /* _CATENA_TXBUFFER_H_ */
