#ifndef AXIO_DISPATCHER
#define AXIO_DISPATCHER

#include <cerrno>
#include <stdexcept>
#include <vector>

#include <poll.h>

#include "./emitter.h"

namespace axio {
    class DispatcherError: public std::runtime_error {
      public:
        inline DispatcherError(): std::runtime_error("") {}
        enum ErrorType {
            invalidAddr = EFAULT,
            interrupted = EINTR,
            badFdNumber = EINVAL,
            outofMemory = ENOMEM
        };

        inline ErrorType type() const noexcept {
            return static_cast<ErrorType>(errno);
        };
    };

    template <typename Emitter>
    class InitError: public std::runtime_error {
      public:
        inline InitError(): std::runtime_error("") {}
        typename Emitter::InitErr type() const noexcept {
            return static_cast<typename Emitter::InitErr>(errno);
        };
    };

    class Dispatcher {
      public:
        inline Dispatcher() noexcept {}
        inline Dispatcher(pollfd* base, std::size_t size) noexcept:
            base_(base), size_(size) {}

        Dispatcher(Dispatcher& other) = delete;
        Dispatcher& operator=(Dispatcher& other) = delete;

        template <typename EvtIter>
        void dispatch(EvtIter&& begin, EvtIter&& end, int timeout = -1) {
            while(true) {
                int ready = poll(base_, size_, timeout);
                if(ready < 0) throw DispatcherError();

                for(auto current = begin; current != end; ++current) {
                    pollfd& evfd = base_[current->offset];
                    if(current->events & evfd.revents) {
                        if(!current->callback(
                            Emitter(*this, current->offset, evfd.fd))) return;
                        int cleared = (evfd.revents -= current->events) == 0;
                        if((ready -= cleared) == 0) break;
                    }
                };
            }
        }

        friend class Emitter;

      protected:
        virtual void drop(uint32_t offset) = 0;

        pollfd*     base_;
        std::size_t size_;
    };
} // namespace axio

#endif // AXIO_DISPATCHER
