#

set (PYCP_SRCS
    pyc_compare.hpp
    pyc_big_integer.cpp pyc_big_integer.hpp
)
add_library(types STATIC ${PYCP_SRCS})

target_include_directories(types PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR})
