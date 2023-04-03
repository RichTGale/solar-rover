add_library (mycutils ../../src/mycutils.h ../../src/mycutils.c)
add_library (term ../../src/term.h ../../src/term.c)
add_library (app ../../src/app.h ../../src/app.c)

target_link_libraries(term LINK_PUBLIC mycutils)
target_link_libraries(app LINK_PUBLIC pi-gpio term)

target_include_directories (mycutils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories (term PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories (app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
