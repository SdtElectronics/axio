set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(socketUNIX
    socketUNIX.cpp
)
add_test(socketUNIXdomian socketUNIX)
target_link_libraries(socketUNIX
  PRIVATE
    axio
    Catch2
    Threads::Threads
)

add_executable(channel
    channel.cpp
)
add_test(Channel channel)
target_link_libraries(channel
  PRIVATE
    axio
    Catch2
    Threads::Threads
)

add_executable(timer
    timer.cpp
)
add_test(Timer timer)
target_link_libraries(timer
  PRIVATE
    axio
    Catch2
    Threads::Threads
)

if(MSVC)
    set_target_properties(
        socketUNIX
        channel
        timer

      PROPERTIES 
        COMPILE_OPTIONS /W4
    )
else()
    set_target_properties(
        socketUNIX
        channel
        timer

      PROPERTIES 
        COMPILE_OPTIONS -Wall -Wextra -Wpedantic
    )
endif()
