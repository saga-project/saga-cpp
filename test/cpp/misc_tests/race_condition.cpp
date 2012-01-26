//
//  Copyright (c) 2008 João Abecasis
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <saga/saga.hpp>

#include <boost/timer.hpp>
#include <boost/thread.hpp>

#include <iostream>

void test_get_url()
{
    // Nothing fancy here.

    saga::session s;
    saga::filesystem::directory dir(s, "./");

    saga::task t =
        // dir.get_url<saga::task_base::Sync>();
        dir.get_url<saga::task_base::Async>();
        // dir.get_url<saga::task_base::Task>();
}

volatile bool graceful_exit = false;
volatile bool run_trigger = true;
volatile std::size_t loop_count = 0;

void race_trigger()
{
    std::cout
        << "Entering the race triggering loop.\n"
        << std::flush;

    // Trying to trigger a race condition
    while (run_trigger)
    {
        ++loop_count;
        test_get_url();
    }

    // Success!
    graceful_exit = true;

    std::cout
        << "Exiting the race triggering loop.\n"
        << std::flush;
}

int main()
{
    boost::timer timer;

    std::cout <<
        "This test tries to trigger a mythic race condition in the engine.\n"
        "Purportedly, the creature was witnessed and killed a couple of\n"
        "times before, but new sightings are reported every now and then.\n\n"

        "This test is here as a reminder to the adventurous!...\n\n\n"
        << std::flush;

    std::cout
        << "Starting background thread with race trigger.\n"
        << std::flush;

    boost::thread thr(race_trigger);

    {
        boost::xtime t;
        boost::xtime_get(&t, boost::TIME_UTC);

        std::cout
            << "Giving the trigger some time to work its magic.\n"
            << std::flush;

        t.sec += 58;
        boost::thread::sleep(t);

        std::cout
            << "Alright, that's enough!\n"
            << std::flush;

        run_trigger = false;

        std::cout
            << "... waiting for background thread to exit gracefully.\n"
            << std::flush;

        t.sec += 2;
        boost::thread::sleep(t);
    }

    std::cout
        << "That's it. We're out of here.\n\n"
        << std::flush;

    // Don't join the thread, it must be killed if it didn't complete in the
    // allotted time.

    std::cout
        << " === "
        << loop_count << " iterations in "
        << timer.elapsed() << " seconds.\n" << std::flush;

    exit(!graceful_exit);
}
