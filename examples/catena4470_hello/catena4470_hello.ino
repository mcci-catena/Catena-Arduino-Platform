/*

Module:  catena4470_hello.ino

Function:
        Simple hello-world (and compile-test) app for Catena 4470.

Copyright notice and License:
        See LICENSE file accompanying this project.
 
Author:
        Terry Moore, MCCI Corporation	April 2018

*/

#include <Catena4470.h>
#include <Catena_Led.h>

using namespace McciCatena;
using Catena = Catena4470;

// declare the global object for the platform.
Catena gCatena;

// declare lorawan (so we can provision)
Catena::LoRaWAN gLoRaWAN;

// declare the LED object
StatusLed gLed (Catena::PIN_STATUS_LED);

void setup()
        {
        gCatena.begin();

        gCatena.SafePrintf("Hello, world!\n");
        gCatena.SafePrintf("This is a Catena 4470 demo program for the MCCI Catena-Arduino-Platform library.\n");
        gCatena.SafePrintf("Enter 'help' for a list of commands.\n");
        gCatena.SafePrintf("(remember to select 'Line Ending: Newline' at the bottom of the monitor window.)\n");

	gLoRaWAN.begin(&gCatena);
	gCatena.registerObject(&gLoRaWAN);

        gLed.begin();
        gCatena.registerObject(&gLed);
        gLed.Set(LedPattern::FastFlash);
        }

void loop()
        {
        gCatena.poll();
        }
