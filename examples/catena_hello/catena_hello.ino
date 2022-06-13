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
#include <Catena_AppObjects.h>
#include <Catena_Led.h>

using namespace McciCatena;

// declare the global object for the platform.
Catena gCatena(Version_t{CATENA_ARDUINO_PLATFORM_VERSION}, __FILE__, "Simple hello-world sketch");

void setup()
        {
        gCatena.setup();

        gCatena.SafePrintf("Hello, world!\n");
        gCatena.SafePrintf("This is a basic demo program for the MCCI Catena-Arduino-Platform library.\n");

        gLed.Set(LedPattern::FastFlash);
        }

void loop()
        {
        gCatena.poll();
        }
