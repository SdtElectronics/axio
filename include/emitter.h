#ifndef AXIO_EMITTER
#define AXIO_EMITTER

#include "./dispatcher.h"

namespace axio {
    using EvtVec = short;

    class Emitter {
      public:
        inline Emitter(int fd, uint32_t offset, Dispatcher& parent):
            fd_(fd), offset_(offset), parent_(parent) {};

        inline Emitter(Emitter&& other):
            fd_(other.fd_), offset_(other.offset_), parent_(other.parent_) {
            other.fd_ = -fd_;
        };

        Emitter(Emitter& other) = delete;
        Emitter& operator=(Emitter& other) = delete;

        inline void update(int fd) noexcept {
            parent_.base_[offset_].fd = fd;
            fd_ = fd;
        }

        inline int getRawFd() const noexcept {
            return fd_;
        }

        inline EvtVec& getPending() const noexcept {
            return parent_.base_[offset_].revents;
        }

        inline void listen(EvtVec events) noexcept {
            parent_.base_[offset_].events = events;
        }

        inline ~Emitter() noexcept {
            update(-fd_);
            close(fd_);
        };

      protected:
        int         fd_;
        uint32_t    offset_;
        Dispatcher& parent_;
    };
} // namespace axio

#endif // AXIO_EMITTER
