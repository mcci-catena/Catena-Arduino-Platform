/*

Module:	Catena_BootloaderApi.cpp

Function:
	cBootloaderApi.

Copyright and License:
	This file copyright (C) 2021 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	See accompanying LICENSE file for copyright and license information.

Author:
	Terry Moore, MCCI Corporation	April 2021

*/

#include "Catena_BootloaderApi.h"

#include <stdint.h>
using namespace McciCatena;

/****************************************************************************\
|
|	Manifest constants & typedefs.
|
\****************************************************************************/

extern "C" {

/// \brief error codes from SVC handler on ARMv6-M systems & such like
typedef enum McciBootloaderPlatform_ARMv6M_SvcError_e
	{
	/// successful processing
	McciBootloaderPlatform_SvcError_OK = 0,

	/// error: verify failure
	McciBootloaderPlatform_SvcError_VerifyFailure = UINT32_C(-3),
	/// error: invalid parameter to SVC
	McciBootloaderPlatform_SvcError_InvalidParameter = UINT32_C(-2),
	/// error: SVC isn't just unimplemented, it's unkown to the bootloader
	McciBootloaderPlatform_SvcError_Unclaimed = UINT32_C(-1),
	} McciBootloaderPlatform_ARMv6M_SvcError_t;

/// \brief SVC request codes for boot loader on ARMv6-M systems & such like
typedef enum McciBootloaderPlatform_ARMv6M_SvcRq_e
	{
	/// Fetch pointer to \c uint32_t "update" flag in EEPROM. \c arg1 is pointer to cell to
	/// receive pointer.
	McciBootloaderPlatform_ARMv6M_SvcRq_GetUpdatePointer = UINT32_C(0x01000000),

	/// Call \ref mcci_tweetnacl_hashblocks_sha512_init. \c arg1 is pointer to hash block.
	McciBootloaderPlatform_ARMv6M_SvcRq_HashInit  /* = UINT32_C(0x01000001) */,

	/// Call \ref mcci_tweetnacl_hashblocks_sha512. \c arg1 points to argument.
	/// \see McciBootloaderPlatform_ARMv6M_SvcRq_HashBlocks_Arg_t
	McciBootloaderPlatform_ARMv6M_SvcRq_HashBlocks  /* = UINT32_C(0x01000002) */,

	/// Call \ref mcci_tweetnacl_hashblocks_finish. \c arg1 points to
	/// argument.
	/// \see McciBootloaderPlatform_ARMv6M_SvcRq_HashFinish_Arg_t
	McciBootloaderPlatform_ARMv6M_SvcRq_HashFinish  /* = UINT32_C(0x01000003) */,

	/// Call \ref mcci_tweetnacl_verify64. \c arg1 and \c arg2 are the pointers
	/// result is set to verifyFailure for failure.
	McciBootloaderPlatform_ARMv6M_SvcRq_Verify64  /* = UINT32_C(0x01000004) */,
	} McciBootloaderPlatform_ARMv6M_SvcRq_t;

/// \brief argument to \ref McciBootloaderPlatform_ARMv6M_SvcRq_HashBlocks
typedef struct McciBootloaderPlatform_ARMv6M_SvcRq_HashBlocks_Arg_s
	{
	void *pHash;
	const uint8_t *pMessage;
	size_t nMessage;
	} McciBootloaderPlatform_ARMv6M_SvcRq_HashBlocks_Arg_t;

/// \brief argumennt to \ref McciBootloaderPlatform_ARMv6M_SvcRq_HashFinish
typedef struct McciBootloaderPlatform_ARMv6M_SvcRq_HashFinish_Arg_s
	{
	void *pHash;
	const uint8_t *pMessage;
	size_t nMessage;
	size_t nOverall;
	} McciBootloaderPlatform_ARMv6M_SvcRq_HashFinish_Arg_t;

///
/// \brief SVC function interface
///
/// \param [in] svcRequest is the request code.
/// \param [in] pErrorCode points to a cell to be filled with the error code
///		(According to the value of \p svcRequest, there may additional
///		OUT parameters)
/// \param [in] arg1 is an extra argument, interpreted according to \p svcRequest
/// \param [in] arg2 is an extra argument, interpreted according to \p svcRequest
///
/// \details
///	This definition lets us take advantage of the ARMv6-M and related
///	implementations; exception handlers are just like regular subroutines
///	(except that results have to be passed back by modifying the saved
///	registers on the stack). We work around this by passing results back
///	in memory.
///
typedef void
(McciBootloaderPlatform_ARMv6M_SvcHandlerFn_t)(
	McciBootloaderPlatform_ARMv6M_SvcRq_t svcRequest,
	McciBootloaderPlatform_ARMv6M_SvcError_t *pErrorCode,
	uint32_t arg1,
	uint32_t arg2
	);

///
/// \brief convert number to systcal function pointer
///
static inline
McciBootloaderPlatform_ARMv6M_SvcHandlerFn_t *toSvc(uint32_t v)
    {
    return (McciBootloaderPlatform_ARMv6M_SvcHandlerFn_t *)v;
    }
} // extern "C"

/****************************************************************************\
|
|	Read-only data.
|
\****************************************************************************/

extern "C" {
    extern char _Mcci_AppInfo;
}

/****************************************************************************\
|
|	Variables.
|
\****************************************************************************/

/****************************************************************************\
|
|	Code.
|
\****************************************************************************/

uint32_t *cBootloaderApi::getUpdateFlagAddress()
    {
    auto const pSvc = toSvc(this->getSvc());
    McciBootloaderPlatform_ARMv6M_SvcError_t err;
    uint32_t *result;

    err = McciBootloaderPlatform_SvcError_Unclaimed;
    result = nullptr;

    if (pSvc != nullptr)
        {
        (*pSvc)(
            McciBootloaderPlatform_ARMv6M_SvcRq_GetUpdatePointer,
            &err,
            (uint32_t)&result,
            0);
        }

    if (err != McciBootloaderPlatform_SvcError_OK)
        result = nullptr;

    return result;
    }

void cBootloaderApi::hash_init(uint8_t (&hash)[512/8])
    {
    auto const pSvc = toSvc(this->getSvc());
    McciBootloaderPlatform_ARMv6M_SvcError_t err;
    err = McciBootloaderPlatform_SvcError_Unclaimed;

    if (pSvc != nullptr)
        {
        (*pSvc)(
            McciBootloaderPlatform_ARMv6M_SvcRq_HashInit, 
            &err,
            (uint32_t)&hash[0],
            0);
        }

    if (err != McciBootloaderPlatform_SvcError_OK)
        {
        memset(hash, 0, sizeof(hash));
        }
    }

size_t cBootloaderApi::hash_blocks(
    uint8_t (&hash)[512/8],
    const uint8_t *p,
    size_t n
    )
    {
    auto const pSvc = toSvc(this->getSvc());
    McciBootloaderPlatform_ARMv6M_SvcError_t err;
    size_t result;

    err = McciBootloaderPlatform_SvcError_Unclaimed;
    result = 0;

    if (pSvc != nullptr)
        {
        McciBootloaderPlatform_ARMv6M_SvcRq_HashBlocks_Arg_t arg;

        arg.pHash = hash;
        arg.pMessage = p;
        arg.nMessage = n;
        (*pSvc)(
            McciBootloaderPlatform_ARMv6M_SvcRq_HashBlocks,
            &err,
            (uint32_t) &arg,
            0
            );
        if (err == McciBootloaderPlatform_SvcError_OK)
            result = arg.nMessage;
        }

    return result;
    }

void cBootloaderApi::hash_finish(
    uint8_t (&hash)[512/8],
    const uint8_t *p,
    size_t n,
    size_t msgSize
    )
    {
    auto const pSvc = toSvc(this->getSvc());
    McciBootloaderPlatform_ARMv6M_SvcError_t err;

    err = McciBootloaderPlatform_SvcError_Unclaimed;

    if (pSvc != nullptr)
        {
        McciBootloaderPlatform_ARMv6M_SvcRq_HashFinish_Arg_t arg;

        arg.pHash = hash;
        arg.pMessage = p;
        arg.nMessage = n;
        arg.nOverall = msgSize;
        (*pSvc)(
            McciBootloaderPlatform_ARMv6M_SvcRq_HashFinish,
            &err,
            (uint32_t) &arg,
            0
            );
        }
    }

bool cBootloaderApi::hash_verify(
    const uint8_t (&hash1)[512/8],
    const uint8_t (&hash2)[512/8]
    )
    {
    auto const pSvc = toSvc(this->getSvc());
    McciBootloaderPlatform_ARMv6M_SvcError_t err;

    err = McciBootloaderPlatform_SvcError_Unclaimed;

    if (pSvc != nullptr)
        {
        (*pSvc)(
            McciBootloaderPlatform_ARMv6M_SvcRq_Verify64,
            &err,
            (uint32_t) hash1,
            (uint32_t) hash2
            );
        }

    return err == McciBootloaderPlatform_SvcError_OK;
    }

uint32_t cBootloaderApi::getSvc(void)
    {
    if (this->m_svcaddr == kNotFound)
        return 0;
    else if (this->m_svcaddr != kNotInitialized)
        return this->m_svcaddr;

    // look for the SVC pointer.
    uint32_t const vects = SCB->VTOR;
    uint32_t result;

    this->m_svcaddr = kNotFound;
    if (vects == kBootloaderAddress)
        /* not using bootloader */
        return 0;

    const auto svcAddr = ((uint32_t *)kBootloaderAddress)[kSvcIndex];
    if ((svcAddr & 1) == 0)
        return 0;

    this->m_svcaddr = svcAddr;
    return svcAddr;
    }

cBootloaderApi::AppInfo_t const *
cBootloaderApi::AppInfo_t::getForRunningApp(
    void
    )
    {
    return (const AppInfo_t *)&_Mcci_AppInfo;
    }

cBootloaderApi::AppInfo_t const *
cBootloaderApi::AppInfo_t::getForBootloader(
    void
    )
    {
    return (const AppInfo_t *)(kBootloaderAddress + 4 * 48);
    }
