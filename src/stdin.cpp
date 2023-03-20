#include <unistd.h>

#include "./dispatcher.h"
#include "stdin.h"

namespace axio {
    Stdin::Stdin(Dispatcher& parent, uint32_t offset, int fd):
        Emitter(parent, offset, fd) {}

    int Stdin::construct() noexcept { return STDIN_FILENO; }
}
