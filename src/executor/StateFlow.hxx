/** \copyright
 * Copyright (c) 2013, Stuart W Baker
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \file StateFlow.hxx
 *
 * Defines a type of state machine flow used within class Service.
 *
 * @author Stuart W Baker
 * @date 25 December 2013
 */

#ifndef _StateFlow_hxx_
#define _StateFlow_hxx_

#include <type_traits>

#include "executor/Allocator.hxx"
#include "executor/Message.hxx"
#include "utils/BufferQueue.hxx"

#define STATE(_fn)                                                             \
    (Callback)(&std::remove_reference<decltype(*this)>::type::_fn)

/** Begin the definition of a StateFlow.
 * @param _name the class name of the StateFlow derived object
 * @param _priorities number of input queue priorities
 */
#define STATE_FLOW_START(_name, _priorities)                                   \
    class _name : public StateFlow<_priorities>                                \
    {                                                                          \
    public:                                                                    \
        _name(Service *service) : StateFlow<_priorities>(service)              \
        {                                                                      \
        }                                                                      \
                                                                               \
        ~_name()                                                               \
        {                                                                      \
        }                                                                      \
                                                                               \
    private:                                                                   \
        Action entry(Message *msg);                                            \
                                                                               \
        _name();                                                               \
                                                                               \
        DISALLOW_COPY_AND_ASSIGN(_name);

/** Begin the definition of a StateFlow that includes timeouts.
 * @param _name the class name of the StateFlow derived object
 * @param _priorities number of input queue priorities
 */
#define STATE_FLOW_START_WITH_TIMER(_name, _priorities)                        \
    class _name : public StateFlow<_priorities>                                \
    {                                                                          \
    public:                                                                    \
        _name(Service *service)                                                \
            : StateFlow<_priorities>(service),                                 \
              timer(TIMEOUT_FROM(service, state_flow_timeout), service, this), \
              timerMsg(NULL)                                                   \
        {                                                                      \
        }                                                                      \
                                                                               \
        ~_name()                                                               \
        {                                                                      \
        }                                                                      \
                                                                               \
        void timeout()                                                         \
        {                                                                      \
            timerMsg ? me()->send(timerMsg) :;                                 \
            timerMsg = NULL;                                                   \
        }                                                                      \
                                                                               \
        void trigger()                                                         \
        {                                                                      \
            timer.trigger();                                                   \
        }                                                                      \
                                                                               \
    private:                                                                   \
        Action entry(Message *msg);                                            \
                                                                               \
        Action timeout_and_call(Callback c, Message *msg, long long period)    \
        {                                                                      \
            msg->id(msg->id() | Message::IN_PROCESS_MSK);                      \
            timerMsg = msg;                                                    \
            timer.start(period);                                               \
            return Action(c);                                                  \
        }                                                                      \
                                                                               \
        Timer timer;                                                           \
        Message *timerMsg;                                                     \
                                                                               \
        bool early()                                                           \
        {                                                                      \
            return timer.early();                                              \
        }                                                                      \
                                                                               \
        _name();                                                               \
                                                                               \
        DISALLOW_COPY_AND_ASSIGN(_name);

/** Declare a state callback in a StateFlow.
 * @param _state the method name of the StateFlow state callback
 */
#define STATE_FLOW_STATE(_state) Action _state(Message *msg);

/** End the definition of a StateFlow.
 */
#define STATE_FLOW_END()                                                       \
    }                                                                          \
    ;

/** Runs incoming Messages through a State Flow.
 */
class StateFlowBase
{
protected:
    /** Constructor.
     * @param service Service that this state flow is part of
     * @param size number of queues in the list
     */
    StateFlowBase(Service *service) : service(service), state(STATE(terminated))
    {
    }

    /** Destructor.
     */
    ~StateFlowBase()
    {
    }

    /* forward prototype */
    class Action;

    /** State Flow callback prototype
     */
    typedef Action (StateFlowBase::*Callback)(Message *);

    /** Return type for a state flow callback.
     */
    class Action
    {
    public:
        /** Constructor.
         */
        Action(Callback s) : nextState(s)
        {
        }

        /** Get the next state for the StateFlowAction.
         */
        Callback next_state()
        {
            return nextState;
        }

    private:
        /** next state in state flow */
        Callback nextState;
    };

    /** Entry into the StateFlow activity.  Pure virtual which must be
     * defined by derived class.
     * @param msg Message belonging to the state flow
     * @return function pointer to next state
     */
    virtual Action entry(Message *msg) = 0;

    /** @returns the current message we are processing. */
    Message *message()
    {
        reutrn currentMessage_;
    }

    /*========== ACTION COMMANDS ===============*/
    /* StateFlow implementations will have to use one of the following commands
     * to return from a state handler to indicate what action to take. */

    /** Call the current state again.
     * @return function pointer to current state handler
     */
    Action again()
    {
        return Action(state);
    }

    /** Terminate current StateFlow activity.  The message instance is not
     * released before termination.  This is usefull if the message will be
     * reused for the purpose of sending to another StateFlow.
     * @return function pointer to terminated method
     */
    Action exit()
    {
        return STATE(terminated);
    }

    /** Terminate current StateFlow activity. after releasing the message.
     * @param msg to release
     * @return function pointer to terminated method
     */
    Action release_and_exit(Message *msg)
    {
        msg->free();
        return exit();
    }

    /** Imediately call the next state upon return.
     * @param c Callback "state" to move to
     * @return function pointer to passed in callback
     */
    Action call_immediately(Callback c)
    {
        return Action(c);
    }

    /** Wait for resource to become available before proceeding to next state.
     * @param c Callback "state" to move to
     * @param msg Message instance we are waiting on
     * @return function pointer to passed in callback
     */
    Action wait_and_call(Callback c, Message *msg)
    {
        msg->id(msg->id() | Message::IN_PROCESS_MSK);
        return Action(c);
    }

    /** Wait for resource to become available before proceeding to next state.
     * if an immediate allocation can be made, an immediate call to the next
     * state will be made.
     * @param c Callback "state" to move to
     * @param msg Message instance we are waiting on
     * @return function pointer to passed in callback
     */
    template <class T>
    Action allocate_and_call(Allocator<T> *a, Callback c, Message *msg)
    {
        return a->allocate_immediate(msg) ? call_immediately(c) : Action(c);
    }

    /** Imediately queue up the next callback for this flow through the
     * executor.
     * Similar to @ref call_immediately, except we place this flow on the back
     * of the Executor queue.
     * @param c Callback "state" to move to
     * @param msg Message instance we are acting upon
     * @return function pointer to passed in callback
     */
    Action yield_and_call(Callback c, Message *msg);

    /** Return a pointer to the service I am bound to.
     * @return pointer to service
     */
    Service *me()
    {
        return service;
    }

    /** Timeout expired.  The expectation is that a derived class will implement
     * this if it desires timeouts.
     */
    virtual void timeout()
    {
    }

private:
    /** Insert a message into one of the work queues.
     * @param msg Message to insert
     * @param index queue index to insert into
     */
    virtual void insert(Message *msg, unsigned index) = 0;

    /** Pull a message out of one of the work queues.
     * @return message out of one of the work queues, NULL if none available
     */
    virtual Message *next() = 0;

    /** Service this StateFlow belongs to */
    Service *service;

    /** Terminate current StateFlow activity.  This method only exists for the
     * purpose of providing a unique address pointer.
     * @param msg unused
     * @return should never return
     */
    Action terminated(Message *msg);

    /** Process an incoming message.
     * @param msg message to process
     * @param priority priority of message
     */
    void process(Message *msg, unsigned priority);

    /** current active state in the flow */
    Callback state_;

    /** The message we are currently processing */
    Message *currentMessage_;

    /** Default constructor.
     */
    StateFlowBase();

    DISALLOW_COPY_AND_ASSIGN(StateFlowBase);
};

template <unsigned NUM_PRIO>
class StateFlow : public StateFlowBase, public QList<Message, NUM_PRIO>
{
public:
    /** Constructor.
     * @param service Service that this state flow is part of
     * @param size number of queues in the list
     */
    StateFlow(Service *service)
        : StateFlowBase(service), QList<Message, NUM_PRIO>()
    {
    }

    /** Destructor.
     */
    ~StateFlow()
    {
    }

protected:
    /** Entry into the StateFlow activity.  Pure virtual which must be
     * defined by derived class.
     * @param msg Message belonging to the state flow
     * @return function pointer to next state
     */
    virtual Action entry(Message *msg) = 0;

private:
    /** Insert a message into one of the work queues.
     * @param msg Message to insert
     * @param index queue index to insert into
     */
    void insert(Message *msg, unsigned index)
    {
        QList<Message, NUM_PRIO>::insert(msg, index >= NUM_PRIO ? NUM_PRIO - 1
                                                                : index);
    }

    /** Pull a message out of one of the work queues.
     * @return message out of one of the work queues, NULL if none available
     */
    Message *next()
    {
        return QList<Message, NUM_PRIO>::next().item;
    }

    /** Default constructor.
     */
    StateFlow();

    DISALLOW_COPY_AND_ASSIGN(StateFlow);
};

#endif /* _StateFlow_hxx_ */