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
#include <Catena_Led.h>

using namespace McciCatena;

// declare the global object for the platform.
Catena gCatena;
Catena::LoRaWAN gLoRaWAN;

// declare the LED object
StatusLed gLed (Catena::PIN_STATUS_LED);

// declare the callback function.
Arduino_LoRaWAN::SendBufferCbFn uplinkDone;

bool gfSuccess;
bool gfTxStarted;
bool gfTxDone;

uint8_t uplinkBuffer[] = { /* port */ 0x10, 0xCA, 0xFE, 0xBA,0xBE };

void setup()
        {
        gCatena.begin();

        gCatena.SafePrintf("Hello, world (with LoRa)!\n");
        gCatena.SafePrintf("This is a basic demo program for the MCCI Catena-Arduino-Platform library.\n");
        gCatena.SafePrintf("Enter 'help' for a list of commands.\n");
        gCatena.SafePrintf("(remember to select 'Line Ending: Newline' at the bottom of the monitor window.)\n");

        gLed.begin();
        gCatena.registerObject(&gLed);
        gLed.Set(LedPattern::FastFlash);

        gLoRaWAN.begin(&gCatena);
        gCatena.registerObject(&gLoRaWAN);

        if (! gLoRaWAN.IsProvisioned())
                gCatena.SafePrintf("LoRaWAN not provisioned yet. Use the commands to set it up.\n");
        else
                {
                // send a confirmed uplink
                if (gLoRaWAN.SendBuffer(uplinkBuffer, sizeof(uplinkBuffer), uplinkDone, nullptr, true))
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
        gfTxDone = true;
        gfSuccess = fSuccess;
        }
