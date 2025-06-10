#
add_executable(test_big_integer test_big_integer.cpp)

target_link_libraries(test_big_integer PRIVATE PythonicCppLib)

add_test(NAME PythonicCppLibTests COMMAND test_big_integer)
