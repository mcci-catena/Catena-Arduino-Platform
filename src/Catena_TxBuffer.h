/*

Module:  Catena_TxBuffer.h

Function:
        namespace McciCatena, class TxBuffer_t;

Copyright notice:
        See accompanying license file.

Author:
        Terry Moore, MCCI Corporation	December 2016

*/

#ifndef _CATENA_TXBUFFER_H_		/* prevent multiple includes */
#define _CATENA_TXBUFFER_H_

#include <cstdint>

namespace McciCatena {

// the underlying types for the shared statics
class AbstractTxBufferBase_t
        {
public:
        /// \brief convert float to 24-bit representation
        static uint32_t f2sflt24(float f); 
 
        /// convert a float in [0..1) to signed 12-bit float.
        ///
        /// \note This a wrapper for the LMIC utility of the same name;
        ///     doing it this way reduces namespace pollution.
        static uint16_t f2sflt12(float f);

        /// convert a float in [0..1) to signed 16-bit float.
        ///
        /// \note This a wrapper for the LMIC utility of the same name;
        ///     doing it this way reduces namespace pollution.
        static uint16_t f2sflt16(float f);

        /// convert a float in [0..1) to unsigned 12-bit float.
        ///
        /// \note This a wrapper for the LMIC utility of the same name;
        ///     doing it this way reduces namespace pollution.
        static uint16_t f2uflt12(float f);

        /// convert a float in [0..1) to unsigned 16-bit float.
        ///
        /// \note This a wrapper for the LMIC utility of the same name;
        ///     doing it this way reduces namespace pollution.
        static uint16_t f2uflt16(float f);
        };

// build a transmit buffer
template <std::size_t N=32>
class AbstractTxBuffer_t : public AbstractTxBufferBase_t
        {
private:
        uint8_t buf[N];    // this sets the largest buffer size
        uint8_t *p;
public:
        AbstractTxBuffer_t() : p(buf) {};
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
        // put a 4-byte unsigned integer.
        void put4u(uint32_t v)
                {
                put((uint8_t)(v >> 24));
                put((uint8_t)(v >> 16));
                put((uint8_t)(v >> 8));
                put((uint8_t)v);
                }
        // put a 4-byte signed integer
        void put4s(int32_t v)
                {
                // just treat the same as uint32_t.
                put4u((uint32_t)v);
                }
        // put a 4-byte IEEE floating point
        void put4f(float v)
                {
                union { uint32_t ui; float f; } x;
                x.f = v;
                put4u(x.ui);
                }
        /// put a 3-byte 24-bit floating point
        void put3f(float v)
                {
                put3(f2sflt24(v));
                }
        // get address of next byte to be filled
        uint8_t *getp(void)
                {
                return p;
                }
        std::size_t getn(void)
                {
                return p - buf;
                }
        uint8_t *getbase(void)
                {
                return buf;
                }
        uint8_t *putflag(uint8_t v)
                {
                uint8_t * const pResult = p;
                put(v);
                return pResult;
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

// for backwards compatibilty
using TxBuffer_t = AbstractTxBuffer_t<32>;

/* the magic byte at the front of the buffer */
enum : uint8_t   {
        FormatSensor1 = 0x11,
        FormatSensor2 = 0x14,
        FormatSensor3 = 0x15,
        FormatSensor4 = 0x16,
        FormatSensor5 = 0x17,
        FormatSensor6 = 0x18,
        FormatSensor7 = 0x19,
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

inline FlagsSensor2 operator|= (FlagsSensor2 &lhs, const FlagsSensor2 &rhs)
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

inline FlagsSensor3 operator|= (FlagsSensor3 &lhs, const FlagsSensor3 &rhs)
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

inline FlagsSensor4 operator|= (FlagsSensor4 &lhs, const FlagsSensor4 &rhs)
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

inline FlagsSensor5 operator|= (FlagsSensor5 &lhs, const FlagsSensor5 &rhs)
        {
        lhs = lhs | rhs;
        return lhs;
        };

enum class FlagsSensor6 : uint8_t
        {
        FlagVbat  = 1 << 0,
        FlagBoot  = 1 << 1,
        FlagWater = 1 << 2,
        };

constexpr FlagsSensor6 operator| (const FlagsSensor6 lhs, const FlagsSensor6 rhs)
        {
        return FlagsSensor6(uint8_t(lhs) | uint8_t(rhs));
        };

inline FlagsSensor6 operator|= (FlagsSensor6 &lhs, const FlagsSensor6 &rhs)
        {
        lhs = lhs | rhs;
        return lhs;
        };

enum class FlagsSensor7 : uint8_t
        {
        FlagVbat  = 1 << 0,
        FlagBoot  = 1 << 1,
        FlagError = 1 << 2,
        FlagMainPower = 1 << 3,
        FlagMainDemand = 1 << 4,
        FlagBranchPower = 1 << 5,
        FlagBranchDemand = 1 << 6,
        };

constexpr FlagsSensor7 operator| (const FlagsSensor7 lhs, const FlagsSensor7 rhs)
        {
        return FlagsSensor7(uint8_t(lhs) | uint8_t(rhs));
        };

inline FlagsSensor7 operator|= (FlagsSensor7 &lhs, const FlagsSensor7 &rhs)
        {
        lhs = lhs | rhs;
        return lhs;
        };

} /* namespace McciCatena */

/**** end of Catena_TxBuffer.h ****/
#endif /* _CATENA_TXBUFFER_H_ */
