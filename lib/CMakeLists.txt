add_library (mycutils ../../src/mycutils.h ../../src/mycutils.c)
add_library (rpiutils ../../src/rpiutils.h ../../src/rpiutils.c)
add_library (stepper_motor ../../src/stepper_motor.h ../../src/stepper_motor.c)
add_library (brushed_motor ../../src/brushed_motor.h ../../src/brushed_motor.c)
add_library (ldr ../../src/ldr.h ../../src/ldr.c)
add_library (button ../../src/button.h ../../src/button.c)
add_library (drive ../../src/drive.h ../../src/drive.c)
add_library (rack ../../src/rack.h ../../src/rack.c)
add_library (interface ../../src/interface.h ../../src/interface.c)
add_library (rover ../../src/rover.h ../../src/rover.c)

target_link_libraries(rpiutils LINK_PUBLIC pi-gpio mycutils)
target_link_libraries(brushed_motor LINK_PUBLIC pi-gpio)
target_link_libraries(stepper_motor LINK_PUBLIC pi-gpio mycutils)
target_link_libraries(ldr LINK_PUBLIC pi-gpio)
target_link_libraries(button LINK_PUBLIC mycutils pi-gpio)
target_link_libraries(drive LINK_PUBLIC brushed_motor)
target_link_libraries(rack LINK_PUBLIC button ldr stepper_motor mycutils)
target_link_libraries(interface LINK_PUBLIC drive rack mycutils rpiutils)
target_link_libraries(rover LINK_PUBLIC interface drive rack mycutils)

target_include_directories (rover PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
