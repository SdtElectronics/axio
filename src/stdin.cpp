#include <unistd.h>

#include "stdin.h"

namespace axio {
    Stdin::Stdin(int fd, uint32_t offset, Dispatcher& parent):
        Emitter(fd, offset, parent) {}

    int Stdin::read(char* buf, size_t size) {
        return ::read(fd_, buf, size);
    }

    int Stdin::construct() noexcept { return STDIN_FILENO; }
}
