#include <cstdio>
#include <functional>

#include <unistd.h>

#include "event.h"
#include "timer.h"
#include "stdin.h"
#include "ExtDispatcher.h"

using Event = axio::Event<std::function<bool(axio::Emitter&)> >;

int main() {
    pollfd fds[2];
    axio::ExtDispatcher dispatcher(fds);
    auto timer = dispatcher.track<axio::Timer>(axio::Timer::monotonic);

    auto sin = dispatcher.track<axio::Stdin>();

    Event events[2] = {
        Event(axio::Timer::fired, timer, [](axio::Emitter& emitter) {
            static_cast<axio::Timer&>(emitter).clear();
            printf("No input for 10s, exiting...\n");
            return false;
        }),

        Event(axio::Stdin::readable, sin, [&timer](axio::Emitter& emitter) {
            timer.setTimeout(10, 0);
            auto& in = static_cast<axio::Stdin&>(emitter);
            char buf[100];
            int sz = in.read(buf, 99);
            buf[sz] = '\0';
            printf("%s", buf);
            return true;
        })
    };

    timer.setTimeout(10, 0);
    dispatcher.dispatch(events, events + 2);
    return 0;
}

