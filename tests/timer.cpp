#include <array>
#include <chrono>

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "event.h"
#include "timer.h"
#include "ExtDispatcher.h"

TEST_CASE("Timer") {
    SECTION("setTimeout") {
        pollfd fds[1];
        axio::ExtDispatcher dispatcher(fds);
        auto timer = dispatcher.track<axio::Timer>(axio::Timer::monotonic);

        if(timer.setTimeout(0, 50000) != 0) FAIL("setTimeout failed");
        auto future = std::chrono::steady_clock::now() + std::chrono::milliseconds(42);

        std::array events = {
            axio::Event(axio::Timer::fired, timer, [future](axio::Emitter emitter) {
                static_cast<axio::Timer&>(emitter).clear();
                REQUIRE(future > std::chrono::steady_clock::now());
                return false;
            })
        };

        dispatcher.dispatch(events.begin(), events.end());
    }
}
