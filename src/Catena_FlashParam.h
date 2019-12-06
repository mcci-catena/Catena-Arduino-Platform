/*

Name:   Catena_FlashParam.h

Function:
    Layout of the flash parameter page for MCCI Catena devices.

Copyright and License:
    See accompanying LICENSE file for copyright and license information.

Author:
    Terry Moore, MCCI Corporation   December 2019

*/

#ifndef _Catena_FlashParam_h_
#define _Catena_FlashParam_h_

#include <cstdint>
#include <cstring>
#include <mcciadk_guid.h>

namespace McciCatena {

class FlashParamsStm32L0_t
    {
public:
    // address of first byte in flash
    static constexpr std::uint32_t kFlashBase = 0x08000000;
    // address of last byte in flash
    static constexpr std::uint32_t kFlashTop  = 0x0802FFFF;
    // number of bytes in a protection page
    static constexpr std::uint32_t kFlashSectorSize = 0x1000;
    // number of bytes in a paramter page
    static constexpr unsigned kFlashPageSize = 128;
    // {b39bc7ac-3e7a-4ac8-92a3-d9535dc6a123}
    static const MCCIADK_GUID_WIRE kPageEndSignature1_Guid;
    static constexpr std::uint32_t getU32(const std::uint8_t *p) { return p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24); }
    static void putU32(std::uint8_t *p, std::uint32_t v)
        {
        p[0] = std::uint8_t(v >>  0);
        p[1] = std::uint8_t(v >>  8);
        p[2] = std::uint8_t(v >> 16);
        p[3] = std::uint8_t(v >> 24);
        }

    // common layout for any descriptor.
    struct ParamDesc_t
        {
        std::uint8_t    uLen;               // the length of this item
        std::uint8_t    uType;              // the type of this item
        };

    // the end-of-memory signature.
    struct PageEndSignature1_t
        {
        std::uint8_t        ParamPointer[4];    // byte pointer to parameter; little-endiand
        MCCIADK_GUID_WIRE   Guid;               // identification of the page -- kGuid.

        std::uint32_t getParamPointer() const
            {
            return getU32(this->ParamPointer);
            }

        void setParamPointer(std::uint32_t ptr)
            {
            putU32(this->ParamPointer, ptr);
            }

        static constexpr bool isValidParamPointer(std::uint32_t p)
            {
            return (kFlashBase <= p && p < kPageEndSignature1Address - sizeof(ParamDesc_t));
            }
        };

    static constexpr std::uint32_t kPageEndSignature1Address = kFlashTop + 1 - sizeof(PageEndSignature1_t);

    enum class ParamDescId : std::uint8_t
        {
        Board = 1,  // this is a board descriptor.
        };

    struct ParamBoard_t : public ParamDesc_t
        {
        std::uint8_t    SerialNumber[8];    // the serial number for this device
        std::uint8_t    Model[4];           // the model ID for this device.
        std::uint8_t    ECN[4];             // ECN info for this device.

        std::uint32_t getModel() const  { return getU32(this->Model); }
        std::uint32_t getECN() const    { return getU32(this->ECN); }

        void setModel(std::uint32_t model)
            {
            putU32(this->Model, model);
            }
        void setECN(std::uint32_t ecn)
            {
            putU32(this->ECN, ecn);
            }
        void getSerialNumber(std::uint8_t mySerial[8]) const
            {
            std::memcpy(mySerial, this->SerialNumber, sizeof(this->SerialNumber));
            }
        void setSerialNumber(const std::uint8_t mySerial[8])
            {
            std::memcpy(this->SerialNumber, mySerial, sizeof(this->SerialNumber));
            }
        };

    };

} // namespace McciCatena

#endif // _Catena_FlashParam_h_
