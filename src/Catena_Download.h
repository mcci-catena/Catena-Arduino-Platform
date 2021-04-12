/*

Module:	Catena_Download.h

Function:
    McciCatena::cDownload

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

#ifndef _catena_download_h_
#define _catena_download_h_

#include <cstdarg>
#include <Catena_BootloaderApi.h>
#include <Catena_Flash.h>
#include <Catena_FSM.h>
#include <Catena_limits.h>
#include <Catena_PollableInterface.h>
#include <Catena_Serial.h>

namespace McciCatena {

///
/// \brief Download from a Serial-like object
///
class cDownload : public cPollableObject
    {
protected:
    static constexpr uint32_t kMaxFileSize = (192 - 24) * 1024;
    static constexpr uint32_t kUploadFileAddress = 256 * 1024;
    static constexpr uint32_t kFallbackFileAddress = 64 * 1024;
    static constexpr uint32_t kTransferChunkBytes = 128;
    static constexpr uint32_t kTransferTimeoutMs = 2 * 1000;

    enum class DownloadRq_t : uint8_t
        {
        Idle = 0,
        GetUpdate = 1,
        GetFallback = 2,
        };

public:
    // constructor
    cDownload() {}

    // neither copyable nor movable
    cDownload(const cDownload&) = delete;
    cDownload& operator=(const cDownload&) = delete;
    cDownload(const cDownload&&) = delete;
    cDownload& operator=(const cDownload&&) = delete;

    /// set up to begin operation
    void begin(cSerialAbstract &rSerial, cFlash &rFlash, cBootloaderApi &rBoot);

    /// poll function (from framework); drives FSM
    virtual void poll() override;

    /// results of downloading
    enum class Status_t : uint32_t
        {
		kSuccessful = 0,	        ///< successful
		kInvalidRequest,	        ///< invalid request code
		kDownloadSizeTooBig,        ///< invalid download size
		kDownloadReadTimeout,       ///< timeout reading image
		kDownloadReadError,	        ///< error reading image
        kDownloadAppInfoBadMagic,   ///< bad magic value in download
        kDownloadAppInfoBadAuthsize,///< bad authsize value in download
		kDownloadSizeTooSmall,      ///< download must be >= 256+96 bytes
		kHashCheckFailure,	        ///< hashcode check failure
		kUpdateFlagLocateFailure,	///< could not locate update flag
		kUpdateFlagSetFailure,		///< could not set update flag
        };

    /// states of the finite state machine.
    enum class State_t : int
        {
        stNoChange = -1,    ///< no change
        stInitial = 0,      ///< initial
        stIdle,             ///< idle
        stErase,            ///< erase the flash
        stGetData,          ///< download
        stProgram,          ///< program
        stFinal,            ///< finish up
        };

    /// dispatch FSM
    /// \param [in] curState is the current state
    /// \param [in] fEntry is true if the state is beign entered.
    State_t fsmDispatch(State_t curState, bool fEntry);

    /// evaluate FSM
    void fsmEval(void)
        {
        this->m_Fsm.eval();
        }

    /// completion function
    typedef void CompletionFn(void *pUserData, Status_t status);

    /// request a download
    bool evStart(bool fUpdateNotFallback, CompletionFn *pDoneFn, void *pUserData)
        {
        if (this->m_fBusy)
            return false;

        this->m_fBusy = true;
        this->m_pCompletionFn = pDoneFn;
        this->m_pUserData = pUserData;

        this->m_events.f.downloadRequest =
            fUpdateNotFallback ? DownloadRq_t::GetUpdate : 
                        DownloadRq_t::GetFallback ;

        this->fsmEval();
        return true;
        }

protected:
    /// complete a download
    State_t completeRequest(Status_t status);

    /// \brief arm the request logic.
    void armRequest(void)
        {
        this->m_events.f.downloadRequest = DownloadRq_t::Idle;
        }

    /// \brief check whether download has been requested; reset flag.
    /// \return \c true if download requested, \c false otherwise.
    DownloadRq_t checkRequest(void)
        {
        auto const result = this->m_events.f.downloadRequest;

        if (result != DownloadRq_t::Idle)
            {
            this->m_events.f.downloadRequest = DownloadRq_t::Idle;
            }
        return result;
        }

private:
    /// \brief the finite state machine object.
    cFSM<cDownload, State_t> m_Fsm;

    cSerialAbstract *m_pSerial;     ///< reference to the serial object for the downloader.
    cFlash *m_pFlash;       ///< reference to the flash object.
    cBootloaderApi *m_pBootloaderApi;   ///< reference to the bootloader API object.
    uint32_t m_fileSize;    ///< size of file.
    uint32_t m_fileIndex;   ///< current byte location in file.
    uint32_t m_readTimer;   ///< timeout timer for getData
    CompletionFn *m_pCompletionFn;  ///< completion function for request
    void *m_pUserData;              ///< context for request.

    /// \brief event flags for cDownload FSM implementation
    union
        {
        /// view of all flags for quick reset.
        uint32_t v;

        /// flags as individual bits
        struct
            {
            DownloadRq_t  downloadRequest: 8;     ///< set non-zero when it's time to download
            } f;
        } m_events;

    cBootloaderApi::AppInfo_t m_appInfo;

    uint8_t m_pagebuffer[256];  ///< one page worth of data for SPI flash
    uint32_t m_nPageBuffer;     ///< number of bytes in page buffer.
    uint32_t m_pageAddr;        ///< SPI flash destination for pagebuffer.

    void pushBytesToFlash(const uint8_t *pBuffer, size_t nBufffer);
    void flushBytesToFlash();
    void initPageBuffer(uint32_t byteIndex);
    bool checkHash(uint32_t fileIndex, uint32_t fileSize);

    uint8_t m_fBusy;        ///< if true, processing a request.
    };

} // McciCatena

#endif // _catena_download_h_
