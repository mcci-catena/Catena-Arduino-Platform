/*

Module:  Catena_TxBuffer.cpp

Function:
        class McciCatena::TxBuffer_t

Copyright notice:
        See accompanying LICENSE file.

Author:
        Terry Moore, MCCI Corporation	April 2019

Notes:
        This module exists primarily to provide linkage of the static
        methods to the LMIC implementations.

*/

#include <Catena_TxBuffer.h>
#include <arduino_lmic.h>
#include <cmath>

namespace McciCatena {

uint16_t AbstractTxBufferBase_t::f2sflt12(float f)
        {
        return LMIC_f2sflt12(f);
        }

uint16_t AbstractTxBufferBase_t::f2sflt16(float f)
        {
        return LMIC_f2sflt16(f);
        }

uint16_t AbstractTxBufferBase_t::f2uflt12(float f)
        {
        return LMIC_f2uflt12(f);
        }

uint16_t AbstractTxBufferBase_t::f2uflt16(float f)
        {
        return LMIC_f2uflt16(f);
        }

/*

Name:	AbstractTxBufferBase_t::f2sflt24()

Function:
        Convert a float to a 24-bit form based on IEEE-754.

Definition:
        uint32_t AbstractTxBufferBase_t::f2sflt24(
                float f
                );

Description:
        This function returns a value strictly in the range 0..0xFFFFFF, containing
        a representation of the input value as a 24-bit floating point number.

        The bits of the result have the following interpretation:

        |  23  |  22..16  |   15..0
        |:----:|:--------:|:---------:
        | sign | exponent | mantissa

        This format (which was defined by MCCI based on IEEE 754) can represent
        magnitudes in the approximate range 2.168e-19 < abs(x) < 1.8445e19, with
        a little more than 5 decimal digits of precision. It can also represent
        +0, -0, +inf, -inf, and NaN. Denormals are supported, allowing numbers
        to be represented with decreasing precision down to 3.309e-24 < abs(x).

        The sign bit is set if the number is negative, reset otherwise. Note that
        negative zero is possible.

        The 7-bit exponent is the binary exponent to be applied to the mantissa,
        less 63. (Thus an exponent of zero is represented by 0x3F in this field.)

        The mantissa is the most significant 16 bits of the binary fraction,
        starting at bit -1 (the "two-to-the-1/2" bit). The effective mantissa
        is the represented mantissa, plus 1.0.

        An exponent value of 0x7F combined with a non-zero mantissa is special;
        it means that the number is a "not-a-number" value. All incoming C++ NANs
        are mapped onto a single NAN value, 0x7F8000. If the mantissa is
        zero, then such values represent positive or negative infinity (as
        determined by the sign).

        An exponent value of 0x00 is also special. If the mantissa is zero, then
        the number represents positive or negative zero (as determined by the
        sign). If the mantissa is non-zero, then the effective mantissa is the
        represented mantissa (as a fraction in (0, 1.0), without the added 1.0
        of a regular number, scaled by $2^{-62}$. Numbers of this kind are
        called _denormals_.

Returns:
        An integer in the range 0..0xFFFFFF.

Notes:
        No errors are possible; all possible float values will result in a
        defined result.

*/

#define FUNCTION "AbstractTxBufferBase_t::f2sflt24"

uint32_t AbstractTxBufferBase_t::f2sflt24(float f)
        {
        int iExp;
        int iOutputExp;
        float normalValue;
        uint32_t sign;
        uint32_t outputFractionLimit;

        // handle NAN
        if (std::isnan(f))
                {
                // the bits in a NAN mantissa are all "important", and
                // we have to throw the bottom bits away. So just change all
                // NAN to a common value.
                return 0x7F8000u;
                }

        sign = 0;
        if (std::signbit(f))
                {
                // set the "sign bit" of the result
                sign = 1u << 23;
                }

        if (std::isinf(f))
                {
                // infinity maps directly, but has to be handled before we call
                // frexpf()
                return 0x7F0000 | sign;
                }

        // get a normalized value (in [0.5..1), unless denormalized), and exponent.          
        normalValue = frexpf(f, &iExp);
  
        if (sign != 0)
                // work with the absolute value of normalValue.
                normalValue = -normalValue;

        // handle positive and negative zero
        if (normalValue == 0.0f)
                return sign;

        // abs(f) is in [0..1).
        // Output format exponent is 0..0x7F. 0x7F is NaN; 0x7E is max, 0 is min,
        // corresponding to 2^63 to 2^-63 times f, but normalized f is in [1..2).
        // So output exp is one smaller.
        outputFractionLimit = 0x1FFFFu;
        iOutputExp = iExp - 1;
        if (iOutputExp > 63)
                return sign | 0x7F0000u;
        else if (iOutputExp < -62)
                {
                // start denormalizing.
                normalValue = ldexp(normalValue, iOutputExp + 62);
                iOutputExp = -63;
                outputFractionLimit = 0xFFFFu;
                }

        // We need to get the top 17 bits of the fraction. The easy way to do this
        // is to adjust the exponent again, and then convert to an int.
        uint32_t outputFraction = ldexpf(normalValue, 17) + 0.5f;

        // round-off might cause overflow; denorms get handled differently.
        if (outputFraction > outputFractionLimit)
                {
                // handle regular cases.
                if (iOutputExp != -63)
                        {
                        // shift right and round.
                        outputFraction = (outputFraction + 1) >> 1;
                        iOutputExp += 1;
                        }
                // handle denorms.
                else
                        {
                        // in this case, the nubmer is actually 0x10000;
                        // so it's no longer a weird nubmer. But we dont'
                        // want to divide the outputFraction by 1.
                        ++iOutputExp;
                        }
                }

        if (iOutputExp > 63)
                return sign | 0x7F0000u;

        // now, compose the final result.
        return (uint32_t)(sign | ((iOutputExp + 63u) << 16) | (outputFraction & 0xFFFFu));
        }

#undef FUNCTION

} // namespace McciCatena
