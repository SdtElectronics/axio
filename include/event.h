#ifndef AXIO_EVENT
#define AXIO_EVENT

#include "./emitter.h"

namespace axio {
    template <typename Callable>
    struct Event {
        Event(EvtVec evt, Emitter& emt, Callable cb):
            events(evt), emitter(emt), callback(cb) {}

        EvtVec   events;
        Emitter& emitter;
        Callable callback;
    };
} // namespace axio

#endif // AXIO_EVENT
