add_library (mycutils ../../src/mycutils.h ../../src/mycutils.c)
add_library (subproc ../../src/subproc.h ../../src/subproc.c)
add_library (motor ../../src/motor.h ../../src/motor.c)
add_library (rover ../../src/rover.h ../../src/rover.c)

target_link_libraries(rover LINK_PUBLIC motor)

target_include_directories (mycutils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories (subproc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories (motor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories (rover PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
