#
add_executable(test_big_integer test_big_integer.cpp)
add_executable(test_stringifier test_stringifier.cpp)
add_executable(test_numeric test_numeric.cpp)
add_executable(test_types test_types.cpp)

target_include_directories(test_types PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(test_big_integer PRIVATE PythonicCppLib)
target_link_libraries(test_stringifier PRIVATE PythonicCppLib)
target_link_libraries(test_numeric PRIVATE PythonicCppLib)
target_link_libraries(test_types PRIVATE PythonicCppLib)

add_test(NAME PythonicCppLibTests COMMAND test_big_integer)
