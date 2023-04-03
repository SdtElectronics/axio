#ifndef AXIO_STDIN
#define AXIO_STDIN

#include <cerrno>

#include <poll.h>

#include "./emitter.h"

namespace axio {
    class Stdin: public Emitter {
      public:
        enum Events: EvtVec {
          readable = POLLIN,
          error    = POLLERR
        };
        enum InitErr { ioError = EIO };

        Stdin(Dispatcher& parent, uint32_t offset, int fd);

        static int construct() noexcept;

        static constexpr EvtVec defEvts = readable;
    };
} // namespace axio

#endif // AXIO_STDIN
