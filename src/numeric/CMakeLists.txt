#

set (PYCP_SRCS
    pyc_numeric.cpp
    pyc_sum.hpp
)
add_library(numeric STATIC ${PYCP_SRCS})

target_include_directories(numeric PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR})
