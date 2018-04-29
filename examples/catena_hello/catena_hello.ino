/*

Module:  catena_hello.ino

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

// declare the LED object
StatusLed gLed (Catena::PIN_STATUS_LED);

void setup()
        {
        gCatena.begin();

        gCatena.SafePrintf("Hello, world!\n");
        gCatena.SafePrintf("This is a basic demo program for the MCCI Catena-Arduino-Platform library.\n");
        gCatena.SafePrintf("Enter 'help' for a list of commands.\n");
        gCatena.SafePrintf("(remember to select 'Line Ending: Newline' at the bottom of the monitor window.)\n");

        gLed.begin();
        gCatena.registerObject(&gLed);
        gLed.Set(LedPattern::FastFlash);
        }

void loop()
        {
        gCatena.poll();
        }
