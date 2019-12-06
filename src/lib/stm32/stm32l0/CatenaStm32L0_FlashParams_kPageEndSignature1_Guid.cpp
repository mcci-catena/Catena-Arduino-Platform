/*

Name:   CatenaStm32L0_FlashParams_kPageEndSignature1_Guid.cpp

Function:
    Defines the value of the page-end signature GUID.

Copyright and License:
    See accompanying LICENSE file for copyright and license information.

Author:
    Terry Moore, MCCI Corporation   Demember 2019

*/

#include <Catena_FlashParam.h>
#include <mcciadk_baselib.h>

using namespace McciCatena;

#ifdef ARDUINO_ARCH_STM32

const MCCIADK_GUID_WIRE FlashParamsStm32L0_t::kPageEndSignature1_Guid
    {
    MCCIADK_GUID_BYTES_INIT(0xb39bc7ac, 0x3e7a, 0x4ac8, 0x92, 0xa3, 0xd9, 0x53, 0x5d, 0xc6, 0xa1, 0x23)
    };

#endif
