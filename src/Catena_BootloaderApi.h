/*

Module:	Catena_BootloaderApi.h

Function:
    McciCatena::cBootloaderApi

Copyright and License:
    This file copyright (C) 2021 by

        MCCI Corporation
        3520 Krums Corners Road
        Ithaca, NY  14850

    See accompanying LICENSE file for copyright and license information.

Author:
    Terry Moore, MCCI Corporation	April 2021

*/

#pragma once

#ifndef _catena_bootloaderapi_h_
#define _catena_bootloaderapi_h_

#include <Arduino.h>
#include <cstdint>

namespace McciCatena {

class cBootloaderApi
    {
private:
    enum : uint32_t {
        kNotInitialized = 0,
        kNotFound = 1,
        };

    static constexpr uint32_t kBootloaderAddress = 0x08000000;
    static constexpr uint32_t kSvcIndex = 11;

public:
    cBootloaderApi()
        : m_svcaddr(kNotInitialized)
        {};

    // neither copyable nor movable
    cBootloaderApi(const cBootloaderApi&) = delete;
    cBootloaderApi& operator=(const cBootloaderApi&) = delete;
    cBootloaderApi(const cBootloaderApi&&) = delete;
    cBootloaderApi& operator=(const cBootloaderApi&&) = delete;

    // bootloader things

    /// \brief get the EEPROM flag pointer
    uint32_t *getUpdateFlagAddress(void);
    
    /// \brief init a SHA512 hash buffer
    void hash_init(uint8_t (&hash)[512/8]);
    /// \brief add some bytes to a hash calculation
    size_t hash_blocks(uint8_t (&hash)[512/8], const uint8_t *p, size_t n);
    /// \brief finalize a hash calculation
    void hash_finish(uint8_t (&hash)[512/8], const uint8_t *p, size_t n, size_t msgSize);
    /// \brief constant-time hash compare
    bool hash_verify(const uint8_t (&hash1)[512/8], const uint8_t (&hash2)[512/8]);
 
    /// \brief the public key
    struct PublicKey_t
        {
        std::uint8_t bytes[32];             ///< contents of the public key
        };

    /// \brief the hash
    struct Hash_Sha512_t
        {
        std::uint8_t bytes[512 / 8];        ///< contents of the hash.
        };

    /// \brief the signature
    struct Signature_t
        {
        std::uint8_t bytes[64];             ///< the signature
        };

    /// \brief the app info block
    struct AppInfo_t
        {
        /// \brief the identifying value
        static constexpr uint32_t kMagic = (('M' << 0) | ('A' << 8) | ('P' << 16) | ('0' << 24));
        /// \brief the index of the appinfo block in the image
        static constexpr uint32_t kFileIndex = 48 * 4;
        static constexpr uint32_t kMaxAuthSize = 0x400;

        uint32_t	magic = kMagic;		    ///< the format identifier.
        uint32_t	size = sizeof(*this);	///< size of this stucture, in bytes
        uint32_t	targetAddress { 0 };	///< the target load address
        uint32_t	imagesize { 0 };	    ///< size of the app, in bytes.
                                            ///   Must be multiple of 4.
        uint32_t	authsize { 0 };		    ///< size of authentication data.
                                            /// Overall image size is imagesize
                                            ///   + authsize
        uint32_t	version { 0 };		    ///< version of the image (semantic version)
        uint64_t	posixTimestamp { 0 };	///< Posix timestamp of image
        uint8_t	    comment[16]; 		    ///< the comment
        uint8_t	    reserved56[16] { 0 };	///< reserved, zero.

        static const AppInfo_t *getForRunningApp(void);
        static const AppInfo_t *getForBootloader(void);
        uint32_t getSize() const
            {
            return this->imagesize + this->authsize;
            }
        const PublicKey_t *getPublicKeyRef() const
            {
            return (const PublicKey_t *)(this->targetAddress + this->imagesize);
            }
        const Hash_Sha512_t *getHashRef() const
            {
            return (const Hash_Sha512_t *)(this->targetAddress + this->imagesize + sizeof(PublicKey_t));
            }
        const Signature_t *getSignatureRef() const
            {
            return (const Signature_t *)(this->targetAddress + this->imagesize + sizeof(PublicKey_t) + sizeof(Hash_Sha512_t));
            }
        };

private:
    uint32_t m_svcaddr;     ///< memoized SVC entry point.
    /// \brief return effective SVC entry point
    uint32_t getSvc(void);
    };

} // namespace McciCatena

#endif /* _catena_bootloaderapi_h_ */
