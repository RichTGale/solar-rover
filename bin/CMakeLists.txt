add_executable (rover_exec.run ../src/rover_exec.c)
add_executable (rover_sp ../src/rover_sp.c)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(rover_exec.run PRIVATE m)
target_link_libraries(rover_sp PRIVATE Threads::Threads)
target_link_libraries(rover_sp PRIVATE m)

target_link_libraries (rover_exec.run LINK_PUBLIC mycutils subproc rover)
target_link_libraries (rover_sp LINK_PUBLIC pi-gpio mycutils rover)
