/*

Module:  catena_hello_lora.ino

Function:
        Simple hello-world (and compile-test) app

Copyright notice and License:
        See LICENSE file accompanying this project.

Author:
        Terry Moore, MCCI Corporation	April 2018

*/

#include <Catena.h>
#include <Catena_AppObjects.h>
#include <Catena_Led.h>

using namespace McciCatena;

// declare the global object for the platform.
Catena gCatena(
        Version_t{CATENA_ARDUINO_PLATFORM_VERSION},
        __FILE__,
        "Simple hello-world sketc with LoRaWAN"
        );

// declare the callback function.
Arduino_LoRaWAN::SendBufferCbFn uplinkDone;

bool gfSuccess;
bool gfTxStarted;
bool gfTxDone;

uint8_t uplinkBuffer[] = { 0xCA, 0xFE, 0xBA, 0xBE };

void setup()
        {
        gCatena.usingLoRaWAN();
        gCatena.setup();

        gCatena.SafePrintf("Hello, world (with LoRa)!\n");
        gCatena.SafePrintf("This is a basic demo program for the MCCI Catena-Arduino-Platform library.\n");

        gLed.Set(LedPattern::FastFlash);

        if (! gLoRaWAN.IsProvisioned())
                gCatena.SafePrintf("LoRaWAN not provisioned yet. Use the commands to set it up.\n");
        else
                {
                // send a confirmed uplink
                if (gLoRaWAN.SendBuffer(uplinkBuffer, sizeof(uplinkBuffer), uplinkDone, nullptr, true, /* port */ 16))
                        gfTxStarted = true;
                else
                        gCatena.SafePrintf("SendBuffer failed!\n");
                }
        }

void loop()
        {
        gCatena.poll();
        if (gfTxStarted && gfTxDone)
                {
                gfTxStarted = false;
                if (gfSuccess)
                        gCatena.SafePrintf("Transmit succeeded.\n");
                else
                        gCatena.SafePrintf("Message uplink failed!\n");
                }
        }

void uplinkDone(void *pCtx, bool fSuccess)
        {
        MCCIADK_API_PARAMETER(pCtx);

        gfTxDone = true;
        gfSuccess = fSuccess;
        }
