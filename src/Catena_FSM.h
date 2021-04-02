/*

Name:	Catena_FSM.h

Function:
        Header file for Catena::cFSM

Copyright:
        See accompanying LICENSE file.

Author:
        Terry Moore, MCCI Corporation	April 2019

*/

#ifndef _Catena_FSM_h_
#define _Catena_FSM_h_

#pragma once

namespace McciCatena {

template <typename TParent, typename TState>
class cFSM
    {
public:
    // default constructor.
    cFSM() {};

    // neither copyable nor movable
    cFSM(const cFSM&) = delete;
    cFSM& operator=(const cFSM&) = delete;
    cFSM(const cFSM&&) = delete;
    cFSM& operator=(const cFSM&&) = delete;

    // initialize the FSM; note use of member function.
    void init(TParent &parent, TState (TParent::*dispatch)(TState, bool));

    // get the current state.
    TState getState(void) const
        {
        return this->m_state;
        }

    // evaluate the FSM
    void eval(void);

private:
    // record state
    void setState(TState newState)
        {
        this->m_state = newState;
        }

    // parent object
    TParent *m_parent;
    // current state
    TState m_state;
    // dispatch pointer
    TState (TParent::*m_dispatch)(TState, bool);

    // the FSM flags
    union
        {
        // view as an unsigned so we can clear easily
        unsigned val;
        struct
            {
            // true if currently executing in the FSM
            bool    Active: 1;
            // true if a reevaluation has been requested
            bool    Reevaluate: 1;
            } f;
        } flags;
    }; // end class cFSM

/*

Name:   McciCatena::cFSM<TParent, TState>::init()

Function:
    Initialize a Finite State Machine.

Definition:
    public:
    void McciCatena::cFSM<TParent, TState>::init(
        TParent &parent,
        TState (TParent::*dispatchFn)(
            TState currentState,
            bool fEntry
            )
        );

Description:
    This function initializes an FSM. This is
    the outer evaluator; it's job is to keep applying
    fsmDispatch() until there's no more work to be done.
    While doing that, it watches for recursive calls
    to eval(), and schedules additional executions
    until everything settles down.

    TState is an enumeration class type with at least
    two members:

        TState::stInitial is the initial state.

        TState::stNoChange is the state returned from
        the FSM dispatcher to indicate "no change of
        state"

    The dispatch function is a method of the parent class;
    you may name it anything you like, but it must have
    prototype:

        TState TParent::myDispatch(
            TState currentState,
            bool fEntry
            );

    It will be called (with `fEntry` == true) each time
    the state changes, and additional times with `fEntry`
    == false, whenever the framework decides that the
    finite state machine needs to be re-evaluated,
    including whenever cFSM::eval() is called.

    This dispatch function shall either return
    TState::stNoChange if the state is not to be changed,
    or a new state value; see cFSM::eval().

Returns:
    No result is returned.

*/

template <typename TParent, typename TState>
void cFSM<TParent, TState>::init(
    TParent &parent,
    TState (TParent::*dispatch)(TState, bool)
    )
    {
    this->m_parent = &parent;
    this->m_dispatch = dispatch;
    this->setState(TState::stInitial);
    this->flags.val = 0;
    this->eval();
    }

/*

Name:   McciCatena::cFSM<TParent, TState>::eval()

Function:
    Evaluate a Finite State Machine.

Definition:
    template <typename TParent, typename TState>
    public:
    void McciCatena::cFSM<TParent, TState>::eval(void);

Description:
    This function evaluates an FSM. This is
    the outer evaluator; its job is to keep applying
    the parent's fsmDispatch() until there's no more
    work to be done.

    While doing that, it watches for recursive calls
    to eval(), and schedules additional executions
    until everything settles down.

    <TState> is an enumeration class type with at least
    two members:

        TState::stInitial is the initial state.

        TState::stNoChange is the state returned from
        the FSM dispatcher to indicate "no change of
        state"

    <TParent> is the type of the parent object for this
    finite state machine; it's used so we can call the
    dispatch function:

        TState TParent::dispatch(
            TState currentState,
            bool fEntry         // true if just entering
                                // currentState.
            );

Returns:
    No result is returned.

*/

template <typename TParent, typename TState>
void cFSM<TParent, TState>::eval(void)
    {
    // request an evaluation.
    this->flags.f.Reevaluate = true;

    // detect and note recursion.
    if (this->flags.f.Active)
        {
        return;
        }

    // otherwise note that we're active.
    this->flags.f.Active = true;

    // iterate until the state doesn't change. Initially,
    // the state is the same as the previous state.
    for (bool fStateChanged = false; fStateChanged || this->flags.f.Reevaluate; )
        {
        this->flags.f.Reevaluate = false;

        // get the current state; if final, and not transitioning to final,
        // don't evaluate; FSM is shut down.
        auto const oldState = this->getState();
        if (oldState == TState::stFinal && !fStateChanged)
            break;

        // call the state dispatcher.
        auto const newState =
                (this->m_parent->*this->m_dispatch)(
                        oldState,
                        fStateChanged
                        );

        // act on the results.
        if (newState != TState::stNoChange)
            {
            fStateChanged = true;
            this->setState(newState);
            }
        else
            {
            fStateChanged = false;
            }
        }

    // done: not active.
    this->flags.f.Active = false;
    }

} // namespace McciCatena

#endif // _Catena_FSM_h_
