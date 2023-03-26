#include <array>

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "bufReader.h"
#include "channel.h"
#include "event.h"
#include "ExtDispatcher.h"
#include "stdin.h"

#define TEST_SSTR "foobar"

TEST_CASE("Channel") {
    SECTION("RXfromSTDOUT") {
        pollfd fds[1];
        axio::ExtDispatcher dispatcher(fds);
        auto chan = dispatcher.track<axio::ChanRX>(STDOUT_FILENO);

        std::array events = {
            axio::Event(axio::ChanRX::readable, chan, [](axio::Emitter emitter) {
                axio::bufReader<100> buf;
                auto view = buf.read(emitter);
                REQUIRE(view == TEST_SSTR);
                return false;
            })
        };

        axio::write(STDOUT_FILENO, TEST_SSTR);
        dispatcher.dispatch(events.begin(), events.end());
    }

    SECTION("TXtoSTDIN") {
        pollfd fds[1];
        axio::ExtDispatcher dispatcher(fds);
        auto stdi = dispatcher.track<axio::Stdin>();

        axio::ChanTX chan(STDIN_FILENO);
        chan.send(TEST_SSTR);

        axio::bufReader<100> buf;
        auto view = buf.read(stdi);
        REQUIRE(view == TEST_SSTR);
    }
}
