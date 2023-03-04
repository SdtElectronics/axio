#ifndef AXIO_TIMER
#define AXIO_TIMER

#include <cerrno>

#include <poll.h>
#include <sys/timerfd.h>

#include "./emitter.h"

namespace axio {

class Timer: public Emitter {
  public:
    enum Events: EvtVec { fired = POLLIN };
    enum InitErr {
        unpermitted = EPERM,
        outofMemory = ENOMEM
    };
    enum ClockId {
        realtime = CLOCK_REALTIME,
        monotonic = CLOCK_MONOTONIC
    };

    Timer(int fd, uint32_t offset, Dispatcher& parent);

    int setTimeout(time_t sec, time_t nsec) noexcept;
    int setInterval(time_t sec, time_t nsec) noexcept;
    int clear();
    int reset();

    static int construct(ClockId id);

    static constexpr EvtVec defEvts = fired;

  private:
    static constexpr itimerspec inf = {
        /* it_interval = */ { /* .tv_sec = */ 0, /* .tv_nsec = */ 0},
        /* .it_value   = */ { /* .tv_sec = */ 0, /* .tv_nsec = */ 0}
    };
};

} // namespace axio

#endif // AXIO_TIMER
