/*
    pyc_pystring.hpp

    pythonic cpp library
    string class

    Author: yhlee
    Copyright © 2025
*/

//============================================================================

#pragma once

#ifndef __cplusplus
#error this header file is for c++
#endif

//============================================================================

#include <string>


//============================================================================
// configs



//============================================================================
// namespace

namespace com::cafrii::pyc {

//============================================================================

/*
    at this time, our own string type is not prepared.

    use std::string instead.

*/

using pystring = std::string;


//============================================================================
}; // namespace com::cafrii::pyc

//============================================================================

#ifdef __PYC_LIB_IMPLEMENTATION

namespace com::cafrii::pyc {
//============================================================================


//============================================================================
}; // namespace com::cafrii::pyc

#endif // __PYC_LIB_IMPLEMENTATION

