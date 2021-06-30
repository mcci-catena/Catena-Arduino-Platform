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

#ifdef ARDUINO_STM32

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
public:
    /// \brief parameters
    /// \{
    static constexpr uint32_t kMaxFileSize = (192 - 24) * 1024;     ///< maximum download file size
    static constexpr uint32_t kUploadFileAddress = 256 * 1024;      ///< where the update file lives in SPI flash
    static constexpr uint32_t kFallbackFileAddress = 64 * 1024;     ///< where the fallback file lives in SPI flash
    static constexpr uint32_t kTransferChunkBytes = 128;            ///< how many bytes in a "chunk" -- matches half-page size
    static constexpr uint32_t kTransferTimeoutMs = 2 * 1000;        ///< how long to wait before timing out a download, in seconds.
    /// \}

    /// \brief the request codes for doing an upload
    enum class DownloadRq_t : uint8_t
        {
        Idle = 0,           ///< no request pending
        GetUpdate = 1,      ///< set the update image and do an update
        GetFallback = 2,    ///< set the fallback image
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
    void begin(cFlash &rFlash, cBootloaderApi &rBoot);

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

    /// names of the states.
    static constexpr const char *getStateName(State_t s)
        {
        switch (s)
            {
        case State_t::stNoChange: return "stNoChange";
        case State_t::stInitial: return "stInitial";
        case State_t::stIdle: return "stIdle";
        case State_t::stErase: return "stErase";
        case State_t::stGetData: return "stGetData";
        case State_t::stProgram: return "stProgram";
        case State_t::stFinal: return "stFinal";
        default: return "<<unknown state>>";
            }
        }

    /// dispatch FSM
    /// \param [in] curState is the current state
    /// \param [in] fEntry is true if the state is beign entered.
    State_t fsmDispatch(State_t curState, bool fEntry);

    /// evaluate FSM
    void fsmEval(void)
        {
        this->m_Fsm.eval();
        }

    // the request object
    struct Request_t
        {
        template <typename Tfn>
        struct Closure_t
            {
            Tfn *pFunction;
            void *pUserData;
            void init(Tfn *pFn, void *pData)
                {
                this->pFunction = pFn;
                this->pUserData = pData;
                }
            };

        /// completion function
        typedef void CompletionFn_t(void *pUserData, Status_t status);
        /// query available data
        typedef int QueryAvailableDataFn_t(void *pUserData);
        /// pull data if possible
        typedef void PromptForDataFn_t(void *pUserData);
        /// read bytes
        typedef size_t ReadBytesFn_t(void *pUserData, std::uint8_t *pBuffer, size_t nBuffer);
    
        /// The methods
        Closure_t<CompletionFn_t>           Completion;
        Closure_t<QueryAvailableDataFn_t>   QueryAvailableData;
        Closure_t<PromptForDataFn_t>        PromptForData;
        Closure_t<ReadBytesFn_t>            ReadBytes;

        /// the request code.
        DownloadRq_t    rq;

        int available(void)
            {
            return this->QueryAvailableData.pFunction(this->QueryAvailableData.pUserData);
            }
        void promptForData(void)
            {
            if (this->PromptForData.pFunction != nullptr)
                this->PromptForData.pFunction(this->PromptForData.pUserData);
            }
        size_t readBytes(std::uint8_t *pBuffer, size_t nBuffer)
            {
            return this->ReadBytes.pFunction(this->ReadBytes.pUserData, pBuffer, nBuffer);
            }
        };


    ///
    /// request an abstract download
    ///
    /// \returns \c true if the request was launched and the callback routine
    ///             will be called. \c false if the request was rejected; the
    ///             callback routine will not be called.
    ///
    bool evStart(Request_t &request)
        {
        if (this->m_pRequest != nullptr)
            return false;

        this->m_pRequest = &request;

        this->m_events.f.downloadRequest = true;

        this->fsmEval();
        return true;
        }

    ///
    /// request a simple serial download over the specified serial port.
    ///
    /// \param [in] rq is the request (typically \c DownloadRq_t::GetUpdate oir
    ///                  \c DownloadRq_t::GetFallback.
    /// \param [in] serial is the serial port to be used.
    /// \param [in] request is the request block to use. It is owned by the
    ///                 library until the completion callback (if the request
    ///                 gets launched).
    /// \param [in] pCallback function to be called on completion of request
    ///                 processing, if the request is accepted.
    /// \param [in] pUserData user context to be passed to the callback function.
    ///
    /// \returns \c true if the request was launched and the callback routine
    ///             will be called. \c false if the request was rejected; the
    ///             callback routine will not be called.
    ///
    bool evStartSerialDownload(
        DownloadRq_t rq,
        cSerialAbstract &serial,
        Request_t &request,
        Request_t::CompletionFn_t *pCallback,
        void *pUserData
        );

protected:
    /// complete a download
    State_t completeRequest(Status_t status);

    /// \brief arm the request logic.
    void armRequest(void)
        {
        this->m_events.f.downloadRequest = false;
        }

    /// \brief check whether download has been requested; reset flag.
    /// \return \c true if download requested, \c false otherwise.
    bool checkRequest(void)
        {
        auto const result = this->m_events.f.downloadRequest;

        if (result)
            {
            this->m_events.f.downloadRequest = false;
            }
        return result;
        }

private:
    /// \brief the finite state machine object.
    cFSM<cDownload, State_t> m_Fsm;

    cFlash *m_pFlash;               ///< reference to the flash object.
    cBootloaderApi *m_pBootloaderApi;   ///< reference to the bootloader API object.
    uint32_t m_fileSize;            ///< size of file.
    uint32_t m_fileIndex;           ///< current byte location in file.
    uint32_t m_readTimer;           ///< timeout timer for getData
    Request_t *m_pRequest;          ///< current request or nullptr

    /// \brief event flags for cDownload FSM implementation
    union
        {
        /// view of all flags for quick reset.
        uint32_t v;

        /// flags as individual bits
        struct
            {
            bool    downloadRequest: 1; ///< set non-zero when a request arrives
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
    };


} // McciCatena

#endif // ARDUINO_STM32

#endif // _catena_download_h_
