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

namespace McciCatena {

template<>
uint16_t AbstractTxBuffer_t<>::f2sflt12(float f)
        {
        return LMIC_f2sflt12(f);
        }
template<>
uint16_t AbstractTxBuffer_t<>::f2sflt16(float f)
        {
        return LMIC_f2sflt16(f);
        }
template<>
uint16_t AbstractTxBuffer_t<>::f2uflt12(float f)
        {
        return LMIC_f2uflt12(f);
        }
template<>
uint16_t AbstractTxBuffer_t<>::f2uflt16(float f)
        {
        return LMIC_f2uflt16(f);
        }

} // namespace McciCatena
