/*

Module:	Model4811_Download.h

Function:
    cDownload

Copyright and License:
    This file copyright (C) 2021 by

        MCCI Corporation
        3520 Krums Corners Road
        Ithaca, NY  14850

    See accompanying LICENSE file for copyright and license information.

Author:
    Terry Moore, MCCI Corporation	April 2021

*/

#include "Catena_Download.h"

#ifdef ARDUINO_STM32

#include <Catena_BootloaderApi.h>
#include <Catena_Flash.h>
#include <Catena_Log.h>
#include <Catena_Serial.h>
#include <CatenaBase.h>
#include <Catena_WatchdogTimer.h>

using namespace McciCatena;

/****************************************************************************\
|
|	Manifest constants & typedefs.
|
\****************************************************************************/


/****************************************************************************\
|
|	Read-only data.
|
\****************************************************************************/


/****************************************************************************\
|
|	Variables.
|
\****************************************************************************/


/*

Name:   cDownload::begin()

Function:
    Initiate operation of the Download module.

Definition:
    void cDownload::begin(
            cSerialAbstract &rSerial,
            cFlash &rFlash,
            cBootloaderApi &rBootloaderApi
            );

Description:
    Start the download module. For convenience, we ask the user to pass in a
    a cFlash object and cBootloaderApi object.

Returns:
    No result is returned.

*/

void cDownload::begin(cFlash &rFlash, cBootloaderApi &rBootloaderApi)
    {
    // save the pointers.
    this->m_pFlash = &rFlash;
    this->m_pBootloaderApi = &rBootloaderApi;

    // initialize the FSM
    this->m_Fsm.init(*this, &cDownload::fsmDispatch);

    // set up for polling.
    CatenaBase::pCatenaBase->registerObject(this);
    }

/*

Name:   cDownload::poll()

Function:
    Initiate operation of the Download module.

Definition:
    virtual void cDownload::poll(
            void
            ) override;

Description:
    We evaluate the firmware Download FSM

Returns:
    No result is returned.

*/

void cDownload::poll(void)
    {
    this->fsmEval();
    }

/*

Name:   cDownload::fsmDispatch()

Function:
    Handle the FSM updates for the Download

Definition:
    cDownload::State_t
            cDownload::fsmDispatch(
                cDownload::State_t curState,
                bool fEntry
                );

Description:
    The download control engine walks through its states to
    make things happen.

Returns:
    No result is returned.

*/

cDownload::State_t
cDownload::fsmDispatch(
    cDownload::State_t curState,
    bool fEntry
    )
    {
    State_t newState;

    newState = State_t::stNoChange;

    if (gLog.isEnabled(gLog.kTrace) && fEntry)
        {
        gLog.printf(gLog.kAlways, "cDownload: enter state %s(%d)\n", getStateName(curState), int(curState));
        }

    switch (curState)
        {
    case State_t::stInitial:
        {
        newState = State_t::stIdle;
        }
        break;

    case State_t::stIdle:
        {
        if (fEntry)
            {
            this->armRequest();
            }

        auto const newRequest = this->checkRequest();

        if (newRequest)
            {
            auto const regionAddress =
                (this->m_pRequest->rq == DownloadRq_t::GetUpdate)
                    ? kUploadFileAddress
                    : kFallbackFileAddress
                    ;
            this->initPageBuffer(regionAddress);

            newState = State_t::stErase;
            }
        }
        break;

    case State_t::stErase:
        {
        const uint32_t regionAddress = this->m_pageAddr;

        if (fEntry)
            {
            const uint32_t regionEnd = regionAddress + kMaxFileSize;

            // erase pages containing the region
            for (auto blockAddress = regionAddress; blockAddress < regionEnd; blockAddress += 64 * 1024)
                {
                gLog.printf(gLog.kVerbose, "erase block at 0x%x...", blockAddress);
                this->m_pFlash->eraseBlock64(blockAddress);
                gLog.printf(gLog.kVerbose, "\n");
                }
            }
        
        // start download
        newState = State_t::stGetData;
        this->m_fileIndex = 0;
        // set size large at first; we'll reduce it once we've read the header
        this->m_fileSize = kMaxFileSize;
        }
        break;

    case State_t::stGetData:
        {
        auto const nAvail = this->m_pRequest->available();
        auto const tNow = millis();

        // for USB downloads, we don't have to handshake. For others,
        // we do, so we want print a '<' when we are looking for data.
        // Call back to the client to take care of it
        if (fEntry)
            {
            // record starting time.
            this->m_readTimer = tNow;

            if (nAvail == 0)
                this->m_pRequest->promptForData();
            }

        if (nAvail >= kTransferChunkBytes)
            {
            newState = State_t::stProgram;
            }
        else if (tNow - this->m_readTimer > kTransferTimeoutMs)
            {
            newState = this->completeRequest(Status_t::kDownloadReadTimeout);
            }
        }
        break;

    case State_t::stProgram:
        {
        if (fEntry)
            {
            // completion; copy data bytes to buffer and program
            // as there are overflows.
            uint8_t data[kTransferChunkBytes];

            auto const nRead = this->m_pRequest->readBytes(data, sizeof(data));

            if (nRead != kTransferChunkBytes)
                {
                newState = this->completeRequest(Status_t::kDownloadReadError);
                break;
                }

            auto const thisIndex = this->m_fileIndex;
            if (thisIndex < this->m_appInfo.kFileIndex + sizeof(this->m_appInfo))
                {
                // copy bytes to the app info header image
                uint8_t *pOut = (uint8_t *)&this->m_appInfo;
                uint8_t *pData = data;
                size_t nCopy;

                nCopy = 0;
                if (thisIndex < this->m_appInfo.kFileIndex)
                    {
                    // bytes at tail might qualify
                    if (thisIndex + nRead > this->m_appInfo.kFileIndex)
                        {
                        size_t nSkip = this->m_appInfo.kFileIndex - thisIndex;
                        pData += nSkip;
                        nCopy = nRead - nSkip;
                        if (nCopy > sizeof(this->m_appInfo))
                            nCopy = sizeof(this->m_appInfo);
                        }
                    }
                else /* start at front of data, but index into app info */
                    {
                    size_t nSkip = thisIndex - this->m_appInfo.kFileIndex;
                    pOut += nSkip;
                    nCopy = this->m_appInfo.kFileIndex - nSkip;
                    if (nCopy < nRead)
                        nCopy = nRead;
                    }
                
                // copy this fragment to the AppInfo image
                if (nCopy > 0)
                    {
                    memcpy(pOut, pData, nCopy);

                    // is it the last fragment?
                    if (pOut + nCopy == (uint8_t *) &this->m_appInfo + sizeof(this->m_appInfo))
                        {
                        // we just filled it. check signature.
                        if (this->m_appInfo.magic != this->m_appInfo.kMagic)
                            {
                            newState = this->completeRequest(Status_t::kDownloadAppInfoBadMagic);
                            break;
                            }
                        // check the file size.
                        if (this->m_appInfo.imagesize < 0x100)
                            {
                            newState = this->completeRequest(Status_t::kDownloadSizeTooSmall);
                            break;
                            }
                        else if (this->m_appInfo.authsize > this->m_appInfo.kMaxAuthSize)
                            {
                            newState = this->completeRequest(Status_t::kDownloadAppInfoBadAuthsize);
                            break;
                            }
                        else if (this->m_appInfo.imagesize > this->kMaxFileSize - this->m_appInfo.authsize)
                            {
                            newState = this->completeRequest(Status_t::kDownloadSizeTooBig);
                            break;
                            }
                        else
                            {
                            this->m_fileSize = this->m_appInfo.imagesize + this->m_appInfo.authsize;
                            gLog.printf(gLog.kVerbose, "file size: 0x%x\n", this->m_fileSize);
                            }
                        }
                    }
                }

            // advance pointer
            auto thisSize = this->m_fileSize - this->m_fileIndex;
            if (thisSize > nRead)
                thisSize = nRead;

            this->m_fileIndex += thisSize;

            // copy bytes to program buffer, programming as it fills
            this->pushBytesToFlash(data, thisSize);
            }

        if (this->m_fileIndex < this->m_fileSize)
            newState = State_t::stGetData;
        else
            newState = State_t::stFinal;
        }
        break;

    case State_t::stFinal:
        {
        if (fEntry)
            {
            // write out the left-overs
            this->flushBytesToFlash();
            }

        bool fOk;

        gLog.printf(gLog.kVerbose, "checking hash:");

        // check the hash
        auto status = this->checkHash(this->m_pageAddr - this->m_fileSize, this->m_fileSize)
                        ? Status_t::kSuccessful
                        : Status_t::kHashCheckFailure
                        ;
 
        // if successful, set the EEPROM flag
        if (status == Status_t::kSuccessful)
            {
            gLog.printf(gLog.kVerbose, " OK\nSet boot flag:");
            uint32_t *pUpdateFlag = this->m_pBootloaderApi->getUpdateFlagAddress();

            if (pUpdateFlag == nullptr)
                status = Status_t::kUpdateFlagLocateFailure;
            else
                {
                /* Unlocking FLASH_PECR register access*/
                if (HAL_IS_BIT_SET(FLASH->PECR, FLASH_PECR_PELOCK))
                    {  
                    WRITE_REG(FLASH->PEKEYR, FLASH_PEKEY1);
                    WRITE_REG(FLASH->PEKEYR, FLASH_PEKEY2);
                    }

                FLASH->PECR &= ~FLASH_PECR_ERASE_Msk;

                __HAL_FLASH_CLEAR_FLAG(
                    FLASH_FLAG_EOP|FLASH_FLAG_WRPERR|FLASH_FLAG_PGAERR|\
                    FLASH_FLAG_SIZERR|FLASH_FLAG_OPTVERR|FLASH_FLAG_RDERR|\
                    FLASH_FLAG_FWWERR|FLASH_FLAG_NOTZEROERR
                    );

                *pUpdateFlag = 0xFFFFFFFFu;
                HAL_FLASH_Lock();

                while (FLASH->SR & FLASH_SR_BSY)
                    /* loop */;

                if (*pUpdateFlag != 0xFFFFFFFFu)
                    {
                    status = Status_t::kUpdateFlagSetFailure;
                    if (! gLog.isEnabled(gLog.kVerbose))
                        gLog.printf(gLog.kError, "Set boot flag:");
                    gLog.printf(gLog.kVerbose | gLog.kError, " failed: 0x%x\n", *pUpdateFlag);
                    }
                else
                    {
                    gLog.printf(gLog.kVerbose, " OK\n");
                    }
                }
            }
        else
            {
            if (! gLog.isEnabled(gLog.kVerbose))
                gLog.printf(gLog.kError, "check hash:");
            gLog.printf(gLog.kError, " failed\n");
            }

        newState = this->completeRequest(status);
        }
        break;

    default:
        newState = State_t::stIdle;
        break;
        }

    return newState;
    }

void
cDownload::pushBytesToFlash(
    const uint8_t *pBuffer,
    size_t nBuffer
    )
    {
    while (nBuffer > 0)
        {
        auto const nPageBuffer = this->m_nPageBuffer;
        auto nCopy = sizeof(this->m_pagebuffer) - nPageBuffer;
        if (nCopy > nBuffer)
            nCopy = nBuffer;

        memcpy(this->m_pagebuffer + nPageBuffer, pBuffer, nCopy);
        pBuffer += nCopy;
        nBuffer -= nCopy;
        this->m_nPageBuffer += nCopy;
        if (this->m_nPageBuffer == sizeof(this->m_pagebuffer))
            {
            this->flushBytesToFlash();
            // above resets this->m_nPageBuffer to zero and advances m_pageAddr.
            }
        }
    }

void
cDownload::flushBytesToFlash(
    void
    )
    {
    if (this->m_nPageBuffer != 0)
        {
        this->m_pFlash->program(this->m_pageAddr, this->m_pagebuffer, this->m_nPageBuffer);
        this->m_pageAddr += this->m_nPageBuffer;
        this->m_nPageBuffer = 0;
        }
    }

void
cDownload::initPageBuffer(
    uint32_t bytePosition
    )
    {
    this->m_pageAddr = bytePosition;
    this->m_nPageBuffer = 0;
    }

bool
cDownload::checkHash(
    uint32_t flashBaseAddr,
    uint32_t fileSize
    )
    {
    uint8_t hash[512/8];
    uint32_t trailerSize = this->m_appInfo.authsize - 32;
    size_t nRemaining;
    const uint8_t *pRemaining;

    if (fileSize <= trailerSize)
        return false;

    this->m_pBootloaderApi->hash_init(hash);

    auto const flashEndAddr = flashBaseAddr + fileSize - trailerSize;

    nRemaining = 0;
    pRemaining = this->m_pagebuffer;

    for (auto i = flashBaseAddr; i < flashEndAddr; )
        {
        gIwdgTimer.refreshWatchdog();
        uint32_t n = flashEndAddr - i;

        if (n > sizeof(this->m_pagebuffer))
            n = sizeof(this->m_pagebuffer);

        // this can take awhile, so let the rest of the system have a
        // peek in.
        CatenaBase::pCatenaBase->poll();

        gIwdgTimer.refreshWatchdog();
        this->m_pFlash->read(i, this->m_pagebuffer, n);

        gIwdgTimer.refreshWatchdog();
        nRemaining = this->m_pBootloaderApi->hash_blocks(hash, this->m_pagebuffer, n);

        gIwdgTimer.refreshWatchdog();

        // advance
        i += n;

        // in case it's the last time, set pRemaining.
        auto const nConsumed = n - nRemaining;
        pRemaining = this->m_pagebuffer + nConsumed;
        }

    // finish up the hash.    
    this->m_pBootloaderApi->hash_finish(hash, pRemaining, nRemaining, flashEndAddr - flashBaseAddr);

    // now grab the security info (including the public key)
    this->m_pFlash->read(flashEndAddr - 32, this->m_pagebuffer, trailerSize + 32);

    // this awkward couple of lines lets us work around the problem
    // of casting the pointer into pagebuffer to a pointer to a hash.
    // Hard to do it directly.
    struct hash_t { uint8_t bytes[512 / 8]; };
    const hash_t *const pProgHash = (hash_t *)&this->m_pagebuffer[32];

    if (this->m_pBootloaderApi->hash_verify(hash, pProgHash->bytes))
        return true;
    else
        return false;
    }

cDownload::State_t
cDownload::completeRequest(
    Status_t status
    )
    {
    Request_t * const pRequest = this->m_pRequest;
    if (pRequest != nullptr)
        {
        this->m_pRequest = nullptr;
        (*pRequest->Completion.pFunction)(pRequest->Completion.pUserData, status);
        }

    return State_t::stIdle;
    }

bool
cDownload::evStartSerialDownload(
    DownloadRq_t rq,
    cSerialAbstract &serial,
    Request_t &request,
    Request_t::CompletionFn_t *pCallback,
    void *pUserData
    )
    {
    // set up the callback.
    request.Completion.init(pCallback, pUserData);

    // set up the query for available data
    request.QueryAvailableData.init(
        [] (void *pUserData) -> int
            {
            cSerialAbstract * const pSerial = (cSerialAbstract *)pUserData;

            return pSerial->available();
            },
        (void *)&serial
        );

    // set up the "prompt"
    request.PromptForData.init(
        [] (void *pUserData) -> void
            {
            cSerialAbstract * const pSerial = (cSerialAbstract *)pUserData;

            pSerial->write("<", 1);
            },
        (void *)&serial
        );

    // set up the transfer from source to our buffer.
    request.ReadBytes.init(
        [] (void *pUserData, std::uint8_t *pData, size_t nData) -> size_t
            {
            cSerialAbstract * const pSerial = (cSerialAbstract *)pUserData;

            return pSerial->stream().readBytes(pData, nData);
            },
        (void *)&serial
        );

    // set up the request code.
    request.rq = rq;

    // launch the request.
    return evStart(request);
    }

#endif /* ARDUINO_STM32 */
