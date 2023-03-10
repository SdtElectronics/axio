#ifndef AXIO_STDIN
#define AXIO_STDIN

#include <cerrno>

#include <poll.h>

#include "./emitter.h"

namespace axio {
    class Stdin: public Emitter {
      public:
        enum Events: EvtVec { readable = POLLIN };
        enum InitErr { ioError = EIO };

        Stdin(Dispatcher& parent, uint32_t offset, int fd);

        int read(char* buf, size_t size);

        static int construct() noexcept;

        static constexpr EvtVec defEvts = readable;
    };
} // namespace axio

#endif // AXIO_STDIN
