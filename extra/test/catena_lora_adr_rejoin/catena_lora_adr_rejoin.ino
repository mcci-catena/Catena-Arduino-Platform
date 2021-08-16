/*

Module:  catena_lora_adr_rejoin.ino

Function:
        App for testing ADR rejoin.

Copyright notice and License:
        See LICENSE file accompanying this project.

Author:
        Terry Moore, MCCI Corporation	July 2021

*/

#include <Catena.h>
#include <Catena_Led.h>
#include <arduino_lmic.h>

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

uint8_t uplinkBuffer[] = { 0xCA, 0xFE, 0xBA, 0xBE };
uint32_t uplinkTime;
uint32_t nSent = 0;

void setup()
        {
        gCatena.begin();

        gCatena.SafePrintf("This is the ADR re-join test program for the MCCI Catena-Arduino-Platform library.\n");
        gCatena.SafePrintf("Enter 'help' for a list of commands.\n");
        gCatena.SafePrintf("(remember to select 'Line Ending: Newline' at the bottom of the monitor window.)\n");

        gLed.begin();
        gCatena.registerObject(&gLed);
        gLed.Set(LedPattern::FastFlash);

        gLoRaWAN.begin(&gCatena);
        gCatena.registerObject(&gLoRaWAN);

        if (! gLoRaWAN.IsProvisioned())
                {
                gCatena.SafePrintf("LoRaWAN not provisioned yet. Use the commands to set it up.\n");
                }
        else
                {
                uplinkTime = millis();
                }
        }

void loop()
        {
        gCatena.poll();
        if (! gfTxStarted)
                {
                do      {
                        if (! gLoRaWAN.IsProvisioned())
                                break;

                        if (int32_t(millis() - uplinkTime) >= 0)
                                {
                                if (nSent == 2)
                                        {
                                        // change the LMIC count so we'll force a join, 
                                        // and change subband.
                                        gCatena.SafePrintf(
                                                "Switching to subband 7: %d\n",
                                                LMIC_selectSubBand(7)
                                                );

                                        // put us at DR0
                                        LMIC_setDrTxpow(LORAWAN_DR0, 21);

                                        // set the LMIC's join count
                                        LMIC.adrAckReq = LINK_CHECK_UNJOIN - 3;
                                        }

                                sendUplink(nSent < 2);
                                }
                        } while (0);
                }
        else if (gfTxStarted && gfTxDone)
                {
                gfTxStarted = false;
                if (gfSuccess)
                        gCatena.SafePrintf("Transmit succeeded: LMIC.adrAckReq=%d LMIC.datarate=%d.\n", LMIC.adrAckReq, LMIC.datarate);
                else
                        gCatena.SafePrintf("Message uplink failed! LMIC.adrAckReq=%d LMIC.datarate=%d.\n", LMIC.adrAckReq, LMIC.datarate);

                uplinkTime = millis() + 15 * 1000;
                }
        else
                {
                // waiting for message to arrive.
                }
        }

void sendUplink(bool fConfirmed)
        {
        // send a confirmed uplink
        if (gLoRaWAN.SendBuffer(uplinkBuffer, sizeof(uplinkBuffer), uplinkDone, nullptr, fConfirmed, /* port */ 16))
                {
                gfTxStarted = true;
                ++nSent;
                }
        else
                {
                gCatena.SafePrintf("SendBuffer failed! Try again in 30 seconds\n");
                uplinkTime = millis() + 30 * 1000;
                }
        }

void uplinkDone(void *pCtx, bool fSuccess)
        {
        gfTxDone = true;
        gfSuccess = fSuccess;
        }
