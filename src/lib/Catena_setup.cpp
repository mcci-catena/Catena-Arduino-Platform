/*

Module:	Catena_setup.cpp

Function:
    Implmentation of the app framework objects

Copyright and License:
    This file copyright (C) 2021 by

        MCCI Corporation
        3520 Krums Corners Road
        Ithaca, NY  14850

    See accompanying LICENSE file for copyright and license information.

Author:
    Terry Moore, MCCI Corporation	November 2021

*/

#include <Catena_AppObjects.h>

#include <Catena_BootloaderApi.h>
#include <Catena_Download.h>
#include <Catena_Flash.h>
#include <Catena_Fram.h>
#include <Catena_Led.h>
#include <Catena_Log.h>
#include <Catena_Serial.h>
#include <SPI.h>
#include <Wire.h>
#include <cinttypes>

using namespace McciCatena;

/****************************************************************************\
|
|	Manifest constants & typedefs.
|
\****************************************************************************/

#if CATENA_PLATFORM_HAS_FLASH && CATENA_PLATFORM_HAS_BOOTLOADER

    // forward reference to the command function
    static cCommandStream::CommandFn cmdUpdate;

#endif


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

#if CATENA_PLATFORM_HAS_FLASH
# if CATENA_PLATFORM_FLASH_TYPE == CATENA_PLATFORM_FLASH_TYPE_AS25SF081

#   include <Catena_Flash_at25sf081.h>
    static cFlash_AT25SF081 sFlash_AT25SF081(D5);
    cFlash &McciCatena::gFlash = sFlash_AT25SF081;

# elif CATENA_PLATFORM_FLASH_TYPE == CATENA_PLATFORM_FLASH_TYPE_MX25V8035F

#   include <Catena_Mx25v8035f.h>
    static Catena_Mx25v8035f sFlash_Mx25v8035f(D19);
    cFlash &McciCatena::gFlash = sFlash_Mx25v8035f;

# endif
#endif

#if CATENA_PLATFORM_HAS_BOOTLOADER
cBootloaderApi McciCatena::gBootloaderApi;
#endif

#if CATENA_PLATFORM_HAS_FLASH && CATENA_PLATFORM_HAS_BOOTLOADER
cDownload McciCatena::gDownload;

// the individual commmands are put in this table
static const cCommandStream::cEntry sDownloadCommmands[] =
        {
        { "fallback", cmdUpdate },
        { "update", cmdUpdate },
        // other commands go here....
        };

/* a top-level structure wraps the above and connects to the system table */
/* it optionally includes a "first word" so you can for sure avoid name clashes */
static cCommandStream::cDispatch
sDownloadCommands_top(
        sDownloadCommmands,              /* this is the pointer to the table */
        sizeof(sDownloadCommmands),      /* this is the size of the table */
        "system"                         /* this is the "first word" for all the commands in this table */
        );
#endif

/* instantiate the standard status light */
StatusLed McciCatena::gLed (LED_BUILTIN);

#if CATENA_PLATFORM_HAS_SPI2_AS_EXTERNAL

/* instantiate SPI */
SPIClass McciCatena::gSPI2(
        Catena::PIN_SPI2_MOSI,
        Catena::PIN_SPI2_MISO,
        Catena::PIN_SPI2_SCK
        );

#endif

/* instantiate a serial object */
cSerial<decltype(Serial)> McciCatena::gSerial(Serial);

/****************************************************************************\
|
|   Code.
|
\****************************************************************************/


/*

Name:	Catena::setup()

Function:
    Set up a Catena.

Definition:
    bool Catena::setup(
        void
        );

Description:
    This function does the platform-specific work for all the canonical Catena
    peripherals for a given platform.

Returns:
    true for success.

Notes:


*/

#define FUNCTION "Catena::setup"

bool
Catena::setup(
    void
    )
    {
    if (this->m_setupRun)
        return this->m_setupResult;

    /// TODO(tmm@mcci.com) this should really be in begin().
    delay(this->enable_3v3Boost(true));
    delay(this->enable_i2cVdd(true));

    this->begin();

    /* register the LED */
    gLed.begin();
    this->registerObject(&gLed);

    auto const OperatingFlags = this->GetOperatingFlags();

    /* do the serial port raindance */
    if (! (OperatingFlags & static_cast<uint32_t>(Catena::OPERATING_FLAGS::fUnattended)))
        {
        if (this->get_consoleIsUsb())
            {
            while (! Serial.dtr())
                yield();
            }
        }

    Serial.begin(115200);
    this->setup_banner_pre();

#if CATENA_PLATFORM_HAS_DOWNLOAD
    /* add our application-specific commands */
    this->addCommands(
            /* name of app dispatch table, passed by reference */
            sDownloadCommands_top,
            /*
            || optionally a context pointer using static_cast<void *>().
            || normally only libraries (needing to be reentrant) need
            || to use the context pointer.
            */
            nullptr
            );
#endif

    this->setup_flash();

    if (this->get_flashFound())
        {
#if CATENA_PLATFORM_HAS_DOWNLOAD
        gDownload.begin(gFlash, gBootloaderApi);
#endif
        }

    /* run the list of initializers */
    bool fResult = true;

    for (auto pObject = this->m_pInitializationList; fResult && pObject != nullptr; pObject = pObject->getNext())
        {
        fResult = pObject->dispatch(this);
        }

    this->setup_banner_post();

    this->m_setupResult = fResult;
    this->m_setupRun = true;

    return this->m_setupResult;
    }

#undef FUNCTION

void Catena::putDashes(unsigned n)
    {
    for (unsigned i = 0; i < n; ++i)
        this->SafePrintf("-");
    this->SafePrintf("\n");
    }

void Catena::setup_banner_pre(void)
    {
    char sVersion[Version_t::kVersionBufferSize];
    this->getAppVersion().toBuffer(sVersion, sizeof(sVersion));

    auto const pSketchName = this->getSketchName();
    auto const pSketchDescription = this->getSketchDescription();

    this->putDashes(0);   // newline
    this->putDashes();

    this->SafePrintf("This is %s V%s.\n", pSketchName ? pSketchName : "<unknown sketch>", sVersion);
    if (pSketchDescription != nullptr)
        this->SafePrintf("%s\n", pSketchDescription);

    this->SafePrintf("Board: %s  SYSCLK: %u MHz  USB: %sabled\n",
                        this->CatenaName(),
                        unsigned(this->GetSystemClockRate() / (1000 * 1000)),
                        this->get_consoleIsUsb() ? "en" : "dis"
                    );
    }

void Catena::setup_banner_post(void)
    {
    this->SafePrintf("Enter 'help' for a list of commands.\n");

    this->putDashes();
    this->putDashes(0);    // newline
    }

bool Catena::setup_flash(void)
    {
    this->set_flashFound(false);

#if CATENA_PLATFORM_HAS_FLASH
    auto const pFlashSpi = this->get_flashBus();
    if (pFlashSpi == nullptr)
        return false;

    auto &flashSpi = *pFlashSpi;

    gLog.printf(cLog::kVerbose, "Init Flash\n");
    flashSpi.begin();

    if (gFlash.begin(&flashSpi))
        {
        uint8_t ManufacturerId;
        uint16_t DeviceId;

        gFlash.readId(&ManufacturerId, &DeviceId);
        gLog.printf(
            cLog::kVerbose,
            "FLASH found, ManufacturerId=%02x, DeviceId=%04x\n",
            ManufacturerId, DeviceId
            );

        this->set_flashFound(true);
        }
    else
        {
        gFlash.end();
        flashSpi.end();
        gLog.printf(
            cLog::kError,
            "No Flash chip found, wiring problem?\n"
            );
        }
#endif

    return this->get_flashFound();
    }

bool Catena::has_flash() const
    {
    return CATENA_PLATFORM_HAS_FLASH;
    }

cFlash *Catena::get_flash() const
    {
    #if ! CATENA_PLATFORM_HAS_FLASH
        return nullptr;
    #else
        return &gFlash;
    #endif
    }

SPIClass *Catena::get_flashBus() const
    {
    #if ! CATENA_PLATFORM_HAS_FLASH
        return nullptr;
    #elif CATENA_PLATFORM_HAS_SPI2_AS_EXTERNAL
        return &gSPI2;
    #else
        return SPI;
    #endif
    }

bool Catena::has_fram() const
    {
    return CATENA_PLATFORM_HAS_FRAM;
    }

uint32_t Catena::get_framSize() const
    {
    return CATENA_PLATFORM_FRAM_SIZE;
    }

::TwoWire *Catena::get_framBus() const
    {
    #if ! CATENA_PLATFORM_HAS_FRAM
        return nullptr;
    #else
        return &Wire;
    #endif
    }

#if CATENA_PLATFORM_HAS_FLASH && CATENA_PLATFORM_HAS_BOOTLOADER

/* process "update" / "fallback" -- args are ignored */
// argv[0] is "update"/"fallbck"
// argv[1..argc-1] are the (ignored) arguments
static cCommandStream::CommandStatus cmdUpdate(
        cCommandStream *pThis,
        void *pContext,
        int argc,
        char **argv
        )
        {
        MCCIADK_API_PARAMETER(pContext);
        MCCIADK_API_PARAMETER(argc);

        pThis->printf(
                "Update firmware: echo off, timeout %" PRIu32 " seconds\n",
                (cDownload::kTransferTimeoutMs + 500) / 1000
                );

        struct context_t
                {
                cCommandStream *pThis;
                bool fWorking;
                cDownload::Status_t status;
                cCommandStream::CommandStatus cmdStatus;
                cDownload::Request_t request;
                };

        context_t context { pThis, true };

        auto doneFn =
                [](void *pUserData, cDownload::Status_t status) -> void
                        {
                        context_t * const pCtx = (context_t *)pUserData;

                        cCommandStream * const pThis = pCtx->pThis;
                        cCommandStream::CommandStatus cmdStatus;

                        cmdStatus = cCommandStream::CommandStatus::kSuccess;

                        if (status != cDownload::Status_t::kSuccessful)
                                {
                                pThis->printf(
                                        "download error, status %u\n",
                                        unsigned(status)
                                        );
                                cmdStatus = cCommandStream::CommandStatus::kIoError;
                                }

                        pCtx->cmdStatus = cmdStatus;
                        pCtx->fWorking = false;
                        };

        if (gDownload.evStartSerialDownload(
                argv[0][0] == 'u' ? cDownload::DownloadRq_t::GetUpdate
                                  : cDownload::DownloadRq_t::GetFallback,
                gSerial,
                context.request,
                doneFn,
                (void *) &context)
                )
                {
                while (context.fWorking)
                        Catena::pCatenaBase->poll();

                return context.cmdStatus;
                }

        pThis->printf(
                "download launch failure\n"
                );

        return cCommandStream::CommandStatus::kInternalError;
        }

#endif /* CATENA_PLATFORM_HAS_FLASH && CATENA_PLATFORM_HAS_BOOTLOADER */

/**** end of Catena_setup.cpp ****/
