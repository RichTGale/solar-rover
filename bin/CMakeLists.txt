add_executable (rover.run ../src/main.c)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(rover.run PRIVATE m)
target_link_libraries(rover.run PRIVATE Threads::Threads)

target_link_libraries (rover.run LINK_PUBLIC rover)
