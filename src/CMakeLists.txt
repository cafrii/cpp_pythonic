#
#
add_subdirectory(types)
add_subdirectory(numeric)
# add_subdirectory(util)

add_library(PythonicCppLib INTERFACE)

target_link_libraries(PythonicCppLib INTERFACE types)
target_link_libraries(PythonicCppLib INTERFACE numeric)
# target_link_libraries(PythonicCppLib INTERFACE types core util)

target_include_directories(PythonicCppLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
