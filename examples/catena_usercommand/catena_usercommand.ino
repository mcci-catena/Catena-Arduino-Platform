/*

Module:  catena_usercommand.ino

Function:
        Example of how to add a user commmand.

Copyright notice and License:
        See LICENSE file accompanying this project.

Author:
        Terry Moore, MCCI Corporation	April 2018

*/

#include <Catena.h>
#include <Catena_CommandStream.h>
#include <Catena_Led.h>

/*
|| This header file fixes things related to symbolic types, to allow us to
|| build with Visual Micro. It's not needed if using a .cpp file, or if
|| the command processor table and functions are all external. It's also not
|| needed unless you're using Visual Micro.
*/
#include <Catena_CommandStream_vmicro_fixup.h>

// save lots of typing by using the McciCatena namespace.
using namespace McciCatena;

// instantiate the global object for the platform.
Catena gCatena;

// instantiate the LED object
StatusLed gLed (Catena::PIN_STATUS_LED);

/*
|| The next few lines give the datastructures needed for extending
|| the command parser.
*/

// forward reference to the command function
cCommandStream::CommandFn cmdHello;

// the individual commmands are put in this table
static const cCommandStream::cEntry sMyExtraCommmands[] =
        {
        { "hello", cmdHello },
        // other commands go here....
        };

/* a top-level structure wraps the above and connects to the system table */
/* it optionally includes a "first word" so you can for sure avoid name clashes */
static cCommandStream::cDispatch
sMyExtraCommands_top(
        sMyExtraCommmands,          /* this is the pointer to the table */
        sizeof(sMyExtraCommmands),  /* this is the size of the table */
        "application"                   /* this is the "first word" for all the commands in this table*/
        );

/*
|| finally, the code.
*/

// setup is called once.
void setup()
        {
        gCatena.begin();

        /* wait 5 seconds */
        auto const now = millis();
        while (millis() - now < 5000)
                /* wait */;

        /* wait for a UAR connection */
        while (! Serial)
                /* wait */;

        /* add our application-specific commands */
        gCatena.addCommands(
                /* name of app dispatch table, passed by reference */
                sMyExtraCommands_top,
                /*
                || optionally a context pointer using static_cast<void *>().
                || normally only libraries (needing to be reentrant) need
                || to use the context pointer.
                */
                nullptr
                );


        gCatena.SafePrintf("This is the user-command demo program for the MCCI Catena-Arduino-Platform library.\n");
        gCatena.SafePrintf("Enter 'help' for a list of commands.\n");
        gCatena.SafePrintf("Enter 'application hello' to exercise the minimial extra command.\n");
        gCatena.SafePrintf("(remember to select 'Line Ending: Newline' at the bottom of the monitor window.)\n");

        gLed.begin();
        gCatena.registerObject(&gLed);
        gLed.Set(LedPattern::FastFlash);
        }

// loop is called repeatedly.
void loop()
        {
        // Tn this app, all we have to do is invoke the Catena
        // polling framework. Everytng wlse will be driven from that.
        gCatena.poll();
        }

/* process "application hello" -- args are ignored */
// argv[0] is "hello"
// argv[1..argc-1] are the (ignored) arguments
cCommandStream::CommandStatus cmdHello(
        cCommandStream *pThis,
        void *pContext,
        int argc,
        char **argv
        )
        {
        pThis->printf("Hello, world!\n");

        return cCommandStream::CommandStatus::kSuccess;
        }
