#
#
add_subdirectory(types)
# add_subdirectory(core)
# add_subdirectory(util)

add_library(PythonicCppLib INTERFACE)

target_link_libraries(PythonicCppLib INTERFACE types)
# target_link_libraries(PythonicCppLib INTERFACE types core util)

target_include_directories(PythonicCppLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
