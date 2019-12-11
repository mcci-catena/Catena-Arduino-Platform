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
    static constexpr std::uint16_t getU16(const std::uint8_t *p) { return p[0] | (p[1] << 8); }
    static void putU32(std::uint8_t *p, std::uint32_t v)
        {
        p[0] = std::uint8_t(v >>  0);
        p[1] = std::uint8_t(v >>  8);
        p[2] = std::uint8_t(v >> 16);
        p[3] = std::uint8_t(v >> 24);
        }
    static void putU16(std::uint8_t *p, std::uint16_t v)
        {
        p[0] = std::uint8_t(v >>  0);
        p[1] = std::uint8_t(v >>  8);
        }

    // common layout for any descriptor.
    enum class ParamDescId : std::uint8_t
        {
        Board = 1,  // this is a board descriptor.
        };

   class ParamDesc_t
        {
    public:
        // default constructor
        ParamDesc_t() {}
        // allow user to select id, length.
        ParamDesc_t(ParamDescId id, std::uint8_t size)
            : uLen(size)
            , uType(std::uint8_t(id))
            {}

    public:
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

    class ParamBoard_t : public ParamDesc_t
        {
    public:
        ParamBoard_t()
            : ParamDesc_t(ParamDescId::Board, sizeof(ParamBoard_t))
            {}
        static constexpr size_t nSerial = 8;

    private:
        std::uint8_t    SerialNumber[nSerial]; // the serial number for this device
        std::uint8_t    Assembly[4];        // the MCCI assembly number for this device -- normally 12300####.
        std::uint8_t    Model[2];           // the model ID for this device.
        std::uint8_t    ModNumber[2];       // the marketing "mod number"
        std::uint8_t    Rev;                // the revision. 0 == a.
        std::uint8_t    Dash;               // the dash nubmer.

    public:
        std::uint32_t getAssembly() const   { return getU32(this->Assembly); }
        std::uint16_t getModel() const      { return getU16(this->Model); }
        std::uint16_t getModNumber() const  { return getU16(this->ModNumber); }
        std::uint8_t getRev() const         { return this->Rev; }
        char getRevChar() const             { return this->getRev() + 'A'; }
        std::uint8_t getDash() const        { return this->Dash; }

        void setAssembly(std::uint32_t assembly)
            {
            putU32(this->Assembly, assembly);
            }
        void setModel(std::uint16_t model)
            {
            putU16(this->Model, model);
            }
        void setModNumber(std::uint16_t modNumber)
            {
            putU16(this->ModNumber, modNumber);
            }
        void setRev(std::uint8_t rev)
            {
            this->Rev = rev;
            }
        bool setRevChar(char c)
            {
            if (! (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z')))
                return false;

            setRev((c - 'A') & 0x1F);
            return true;
            }
        void setDash(std::uint8_t dash)
            {
            this->Dash = dash;
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
