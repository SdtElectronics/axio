#ifndef AXIO_BUFREADER
#define AXIO_BUFREADER

#include <string_view>

#include <unistd.h>

#include "emitter.h"

namespace axio {

    template <std::size_t S>
    class bufReader {
      public:
        bufReader(Dispatcher& parent, uint32_t offset, int fd);

        void read(Emitter& emitter) {
            size = read(emitter.getRawFd(), buf_, S);
            if(size < 0);
        }

        char& operator[](std::size_t index) const noexcept {
            return buf_[index];
        }

        std::string_view data() const noexcept {
            return std::string_view(buf_, size_);
        }

        std::string_view slice(std::size_t begin, int end = 0) const noexcept {
            return std::string_view(buf_, size_ + end);
        }

      private:
        char        buf_[S];
        std::size_t size_;
    };
} // namespace axio

#endif // AXIO_BUFREADER
