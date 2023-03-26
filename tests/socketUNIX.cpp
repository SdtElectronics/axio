#include <array>
#include <thread>

#include <cstdio>

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "event.h"
#include "socket.h"
#include "bufReader.h"
#include "ExtDispatcher.h"

#define TEST_SSTR "foobar"

TEST_CASE("UNIX domain socket") {
    SECTION("basics") {
        pollfd fds[1];
        axio::ExtDispatcher dispatcher(fds);
        auto server = dispatcher.track<axio::Socket>(axio::Socket::UNIX, axio::Socket::packet);

        axio::Socket::UNIXAddress addr("/tmp/axio_test.sock");

        if(!server.listen(addr, 1)) FAIL("listen failed");

        std::array events = {
            axio::Event(axio::Socket::acceptable, server, [](axio::Emitter emitter) {
                axio::Socket::Client client;
                if(!static_cast<axio::Socket&>(emitter).accept(client)) FAIL("accept failed");
                axio::write(client.fd, TEST_SSTR);
                return false;
            })
        };

        std::thread ct([&addr]() {
            pollfd fds[1];
            axio::ExtDispatcher dispatcher(fds);
            auto client = dispatcher.track<axio::Socket>(axio::Socket::UNIX, axio::Socket::packet);

            if(!client.connect(addr)) FAIL("connect failed");
            std::array events = {
                axio::Event(axio::Socket::readable, client, [](axio::Emitter emitter) {
                    axio::bufReader<100> buf;
                    auto view = buf.read(emitter);
                    REQUIRE(view == TEST_SSTR);
                    static_cast<axio::Socket&>(emitter).drop();
                    return false;
                })
            };

            dispatcher.dispatch(events.begin(), events.end());
        });


        dispatcher.dispatch(events.begin(), events.end());
        server.drop();
        ct.join();
    }
}
