/*

Module:  catena_fsm.ino

Function:
        Example of how to build a simple FSM.

Copyright notice and License:
        See LICENSE file accompanying this project at
        https:://github.com/mcci-catena/catena-arduino-platform/

Author:
        Terry Moore, MCCI Corporation	April 2018

*/

#include <Catena.h>
#include <Catena_CommandStream.h>
#include <Catena_FSM.h>
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

/****************************************************************************\
|
|   The Turnstile class and FSM. Refer to the overall README.md for a
|   description of the FSM.
|
\****************************************************************************/

class Turnstile
        {
public:
        // constructor
        Turnstile(Catena &myCatena, StatusLed &lockLed)
                : m_Catena(myCatena),
                  m_Led(lockLed)
                {}

        // states for FSM
        enum class State
                {
                stNoChange = 0, // this name must be present: indicates "no change of state"
                stInitial,      // this name must be presnt: it's the starting state.
                stLocked,       // the turnstile is locked.
                stUnlocked,     // the turnstile is unlocked.
                stFinal,        // this name must be present, it's the terminal state.
                };

        // the begin method initializes the fsm
        void begin()
                {
                if (! this->m_fRunning)
                        this->m_fsm.init(*this, &Turnstile::fsmDispatch);
                else
                        this->m_Catena.SafePrintf("already running!\n");
                }

        // the end method shuts it down
        void end()
                {
                if (this->checkRunning())
                        {
                        this->m_evShutdown = true;
                        while (this->m_fRunning)
                                this->m_fsm.eval();
                        }
                }

        // coin event
        void evCoin()
                {
                if (this->checkRunning())
                        {
                        this->m_evCoin = true;
                        this->m_fsm.eval();
                        }
                }

        // coin event
        void evPush()
                {
                if (this->checkRunning())
                        {
                        this->m_evPush = true;
                        this->m_fsm.eval();
                        }
                }

        // stop event
        void evStop()
                {
                if (this->checkRunning())
                        {
                        this->m_evShutdown = true;
                        this->m_fsm.eval();
                        }
                }

private:
        // the FSM instance
       cFSM<Turnstile, State> m_fsm;

        // verify that FSM is running, and print a message if not.
        bool checkRunning() const
                {
                if (this->m_fRunning)
                        return true;
                else
                        {
                        this->m_Catena.SafePrintf("not running!\n");
                        return false;
                        }
                }

        // "set the lock" - which means display a message
        void setLock(bool fState)
                {
                if (fState)
                        {
                        m_Led.Set(LedPattern::FiftyFiftySlow);
                        m_Catena.SafePrintf("**LOCKED, deposit coin**\n");
                        }
                else
                        {
                        m_Led.Set(LedPattern::FastFlash);
                        m_Catena.SafePrintf("**Unlocked, please proceed**\n");
                        }
                }

        // the FSM dispatch function called by this->m_fsm.
        // this example is here, so we write it here, but in
        // any more complex application you'd have this outside
        // the Class definition, possibly compiled separately.
        State fsmDispatch(State currentState, bool fEntry)
                {
                State newState = State::stNoChange;

                switch (currentState)
                        {

                case State::stInitial:
                        if (fEntry)
                                {
                                // entry is not considered in this state, always move on.
                                }
                        this->m_fRunning = true;
                        newState = State::stLocked;
                        break;

                case State::stLocked:
                        if (fEntry)
                                {
                                this->setLock(true);
                                }
                        if (this->m_evShutdown)
                                {
                                this->m_evShutdown = false;
                                newState = State::stFinal;
                                }
                        else if (this->m_evCoin)
                                {
                                this->m_evCoin = false;
                                newState = State::stUnlocked;
                                }
                        else if (this->m_evPush)
                                {
                                this->m_evPush = false;
                                m_Catena.SafePrintf("Sorry, turnstile is locked. Please deposit a coin.\n");
                                // stay in this state.
                                }
                        else
                                {
                                // stay in this state.
                                }
                        break;

                case State::stUnlocked:
                        if (fEntry)
                                {
                                this->setLock(false);
                                }
                        if (this->m_evShutdown)
                                {
                                newState = State::stLocked;
                                }
                        else if (this->m_evCoin)
                                {
                                this->m_evCoin = false;
                                // stay in this state.
                                m_Catena.SafePrintf("Already unlocked, thanks for the tip!\n");
                                }
                        else if (this->m_evPush)
                                {
                                this->m_evPush = false;
                                m_Catena.SafePrintf("You've passed the turnstile! Have a nice day!\n");
                                newState = State::stLocked;
                                }
                        else
                                {
                                // stay in this state.
                                }
                        break;

                case State::stFinal:
                        // This is called just once; we just clear the
                        // running flag.  The core FSM is responsible for
                        // determining if we're locked or unlocked on
                        // exit. Since we always get here via the stLocked
                        // state, we will, in fact, be locked.
                        if (fEntry)
                                {
                                m_Catena.SafePrintf("turnstile stopped!\n");
                                this->m_fRunning = false;
                                }
                        else
                                {
                                m_Catena.SafePrintf("stFinal but not fEntry shouldn't happen.\n");
                                }

                        // stay in this state.
                        break;

                default:
                        // the default means unknown state.
                        // transition to locked.
                        m_Catena.SafePrintf("unknown state %u!\n", static_cast<unsigned>(currentState));
                        newState = State::stLocked;
                        break;
                        }

                return newState;
                }

        // the Catena reference
        Catena &m_Catena;
        StatusLed &m_Led;

        // the event flags:

        // a coin has been dropped
        bool m_evCoin : 1;

        // someone has pushed on the turnstile.
        bool m_evPush : 1;

        // a shutdown has been requested
        bool m_evShutdown : 1;

        // state flag: true iff the FSM is running.
        bool m_fRunning: 1;
        };

/****************************************************************************\
|
|   The variables
|
\****************************************************************************/

// instantiate the global object for the platform.
Catena gCatena;

// instantiate the LED object
StatusLed gLed (Catena::PIN_STATUS_LED);

// instantiate the turnstile
Turnstile gTurnstile (gCatena, gLed);

/*
|| The next few lines give the datastructures needed for extending
|| the command parser.
*/

// forward reference to the command function
cCommandStream::CommandFn cmdCoin;
cCommandStream::CommandFn cmdPush;
cCommandStream::CommandFn cmdBegin;
cCommandStream::CommandFn cmdEnd;

// the individual commmands are put in this table
static const cCommandStream::cEntry sMyExtraCommmands[] =
        {
        { "begin", cmdBegin },
        { "coin", cmdCoin },
        { "push", cmdPush },
        { "end", cmdEnd },
        // other commands go here....
        };

/* a top-level structure wraps the above and connects to the system table */
/* it optionally includes a "first word" so you can for sure avoid name clashes */
static cCommandStream::cDispatch
sMyExtraCommands_top(
        sMyExtraCommmands,          /* this is the pointer to the table */
        sizeof(sMyExtraCommmands),  /* this is the size of the table */
        nullptr                     /* this is no "first word" for all the commands in this table */
        );

/****************************************************************************\
|
|   The code
|
\****************************************************************************/

// setup is called once.
void setup()
        {
        gCatena.begin();

        /* wait 2 seconds */
        auto const now = millis();
        while (millis() - now < 2000)
                /* wait */;

        /* wait for a UART connection */
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


        gCatena.SafePrintf("This is the FSM demo program for the MCCI Catena-Arduino-Platform library.\n");
        gCatena.SafePrintf("Enter 'help' for a list of commands.\n");
        gCatena.SafePrintf("(remember to select 'Line Ending: Newline' at the bottom of the monitor window.)\n");

        gLed.begin();
        gCatena.registerObject(&gLed);
        gTurnstile.begin();
        }

// loop is called repeatedly.
void loop()
        {
        // Tn this app, all we have to do is invoke the Catena
        // polling framework. Everytng wlse will be driven from that.
        gCatena.poll();
        }

/****************************************************************************\
|
|   The commands -- called automatically from the framework after receiving
|   and parsing a command from the Serial console.
|
\****************************************************************************/

/* process "coin" -- args are ignored */
// argv[0] is "coin"
// argv[1..argc-1] are the (ignored) arguments
cCommandStream::CommandStatus cmdCoin(
        cCommandStream *pThis,
        void *pContext,
        int argc,
        char **argv
        )
        {
        pThis->printf("%s\n", argv[0]);
        gTurnstile.evCoin();

        return cCommandStream::CommandStatus::kSuccess;
        }

/* process "push" -- args are ignored */
// argv[0] is "push"
// argv[1..argc-1] are the (ignored) arguments
cCommandStream::CommandStatus cmdPush(
        cCommandStream *pThis,
        void *pContext,
        int argc,
        char **argv
        )
        {
        pThis->printf("%s\n", argv[0]);
        gTurnstile.evPush();

        return cCommandStream::CommandStatus::kSuccess;
        }

/* process "begin" -- args are ignored */
// argv[0] is "stop"
// argv[1..argc-1] are the (ignored) arguments
cCommandStream::CommandStatus cmdBegin(
        cCommandStream *pThis,
        void *pContext,
        int argc,
        char **argv
        )
        {
        pThis->printf("%s\n", argv[0]);
        gTurnstile.begin();

        return cCommandStream::CommandStatus::kSuccess;
        }

/* process "end" -- args are ignored */
// argv[0] is "end"
// argv[1..argc-1] are the (ignored) arguments
cCommandStream::CommandStatus cmdEnd(
        cCommandStream *pThis,
        void *pContext,
        int argc,
        char **argv
        )
        {
        pThis->printf("%s\n", argv[0]);
        gTurnstile.end();

        return cCommandStream::CommandStatus::kSuccess;
        }
