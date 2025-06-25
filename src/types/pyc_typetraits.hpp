/*
    pyc_typetraits.hpp

*/

/*
    refer https://en.cppreference.com/w/cpp/preprocessor/replace
    for exact macro value.
*/

#if __cplusplus >= 202002L
// #include "pyc_typetraits_20.hpp" # not ready yet.
#include "pyc_typetraits_17.hpp"

#elif __cplusplus >= 201703L
#include "pyc_typetraits_17.hpp"

#else
#error unsupported c++ compiler version

#endif
