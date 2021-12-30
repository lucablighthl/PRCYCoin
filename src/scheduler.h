// Copyright (c) 2015 The Bitcoin Core developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
=======
// Copyright (c) 2017-2019 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_SCHEDULER_H
#define BITCOIN_SCHEDULER_H

//
// NOTE:
<<<<<<< HEAD
// boost::thread / boost::function / boost::chrono should be ported to
// std::thread / std::function / std::chrono when we support C++11.
//
#include <boost/function.hpp>
#include <boost/chrono/chrono.hpp>
#include <boost/thread.hpp>
#include <map>

=======
// boost::thread should be ported to std::thread
// when we support C++11.
//
#include <condition_variable>
#include <functional>
#include <list>
#include <map>

#include "sync.h"

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
//
// Simple class for background tasks that should be run
// periodically or once "after a while"
//
// Usage:
//
// CScheduler* s = new CScheduler();
// s->scheduleFromNow(doSomething, 11); // Assuming a: void doSomething() { }
<<<<<<< HEAD
// s->scheduleFromNow(boost::bind(Class::func, this, argument), 3);
// boost::thread* t = new boost::thread(boost::bind(CScheduler::serviceQueue, s));
//
// ... then at program shutdown, clean up the thread running serviceQueue:
// t->interrupt();
=======
// s->scheduleFromNow(std::bind(Class::func, this, argument), 3);
// boost::thread* t = new boost::thread(std::bind(CScheduler::serviceQueue, s));
//
// ... then at program shutdown, make sure to call stop() to clean up the thread(s) running serviceQueue:
// s->stop();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// t->join();
// delete t;
// delete s; // Must be done after thread is interrupted/joined.
//
<<<<<<< HEAD
class CScheduler {
public:
    CScheduler();

    ~CScheduler();

    typedef boost::function<void(void)> Function;

    // Call func at/after time t
    void schedule(Function f, boost::chrono::system_clock::time_point t);

    // Convenience method: call f once deltaSeconds from now
    void scheduleFromNow(Function f, int64_t deltaSeconds);

    // Another convenience method: call f approximately
    // every deltaSeconds forever, starting deltaSeconds from now.
    // To be more precise: every time f is finished, it
    // is rescheduled to run deltaSeconds later. If you
    // need more accurate scheduling, don't use this method.
    void scheduleEvery(Function f, int64_t deltaSeconds);

    // To keep things as simple as possible, there is no unschedule.
=======

class CScheduler
{
public:
    CScheduler();
    ~CScheduler();

    typedef std::function<void(void)> Function;

    // Call func at/after time t
    void schedule(Function f, std::chrono::system_clock::time_point t);

    // Convenience method: call f once deltaMilliSeconds from now
    void scheduleFromNow(Function f, int64_t deltaMilliSeconds);

    // Another convenience method: call f approximately
    // every deltaMilliSeconds forever, starting deltaMilliSeconds from now.
    // To be more precise: every time f is finished, it
    // is rescheduled to run deltaMilliSeconds later. If you
    // need more accurate scheduling, don't use this method.
    void scheduleEvery(Function f, int64_t deltaMilliSeconds);

    // To keep things as simple as possible, there is no unschedule.

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    // Services the queue 'forever'. Should be run in a thread,
    // and interrupted using boost::interrupt_thread
    void serviceQueue();

    // Tell any threads running serviceQueue to stop as soon as they're
    // done servicing whatever task they're currently servicing (drain=false)
    // or when there is no work left to be done (drain=true)
<<<<<<< HEAD
    void stop(bool drain = false);

    // Returns number of tasks waiting to be serviced,
    // and first and last task times
    size_t getQueueInfo(boost::chrono::system_clock::time_point &first,
                        boost::chrono::system_clock::time_point &last) const;

private:
    std::multimap <boost::chrono::system_clock::time_point, Function> taskQueue;
    boost::condition_variable newTaskScheduled;
    mutable boost::mutex newTaskMutex;
    int nThreadsServicingQueue;
    bool stopRequested;
    bool stopWhenEmpty;

    bool shouldStop() { return stopRequested || (stopWhenEmpty && taskQueue.empty()); }
=======
    void stop(bool drain=false);

    // Returns number of tasks waiting to be serviced,
    // and first and last task times
    size_t getQueueInfo(std::chrono::system_clock::time_point &first,
                        std::chrono::system_clock::time_point &last) const;

    // Returns true if there are threads actively running in serviceQueue()
    bool AreThreadsServicingQueue() const;

private:
    mutable Mutex newTaskMutex;
    std::condition_variable newTaskScheduled;
    std::multimap<std::chrono::system_clock::time_point, Function> taskQueue GUARDED_BY(newTaskMutex);
    int nThreadsServicingQueue GUARDED_BY(newTaskMutex);
    bool stopRequested GUARDED_BY(newTaskMutex);
    bool stopWhenEmpty GUARDED_BY(newTaskMutex);
    bool shouldStop() const EXCLUSIVE_LOCKS_REQUIRED(newTaskMutex) { return stopRequested || (stopWhenEmpty && taskQueue.empty()); }
};

/**
 * Class used by CScheduler clients which may schedule multiple jobs
 * which are required to be run serially. Jobs may not be run on the
 * same thread, but no two jobs will be executed
 * at the same time and memory will be release-acquire consistent
 * (the scheduler will internally do an acquire before invoking a callback
 * as well as a release at the end). In practice this means that a callback
 * B() will be able to observe all of the effects of callback A() which executed
 * before it.
 */
class SingleThreadedSchedulerClient {
private:
    CScheduler *m_pscheduler;

    RecursiveMutex m_cs_callbacks_pending;
    std::list<std::function<void (void)>> m_callbacks_pending;
    bool m_are_callbacks_running = false;

    void MaybeScheduleProcessQueue();
    void ProcessQueue();

public:
    explicit SingleThreadedSchedulerClient(CScheduler *pschedulerIn) : m_pscheduler(pschedulerIn) {}

    /**
     * Add a callback to be executed. Callbacks are executed serially
     * and memory is release-acquire consistent between callback executions.
     * Practially, this means that callbacks can behave as if they are executed
     * in order by a single thread.
     */
    void AddToProcessQueue(std::function<void (void)> func);

    // Processes all remaining queue members on the calling thread, blocking until queue is empty
    // Must be called after the CScheduler has no remaining processing threads!
    void EmptyQueue();

    size_t CallbacksPending();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

#endif
