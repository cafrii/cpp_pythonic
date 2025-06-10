/*
    pyc_compare.hpp

    pythonic cpp library
    comparison operations

    Author: yhlee
    Copyright © 2025
*/

//============================================================================

#pragma once

#ifndef __cplusplus
#error this header file is for c++
#endif

//============================================================================


// #include <string>




//============================================================================
// configs


// #define PYCFG_USE_UTF8CPP



//============================================================================
// namespace

namespace com::cafrii::pyc {

//============================================================================


// lo <= val <= hi, inclusive edge
template <typename T>
constexpr bool InRange(const T& val, const T& lo, const T& hi) {
    return (val >= lo && val <= hi);
}

// lo < val < hi, exclusive edge
template <typename T>
constexpr bool InRangeExcl(const T& val, const T& lo, const T& hi) {
    return (val > lo && val < hi);
}


//============================================================================
}; // namespace com::cafrii::pyc

//============================================================================

#ifdef __PYC_LIB_IMPLEMENTATION

namespace com::cafrii::pyc {
//============================================================================



//============================================================================
}; // namespace com::cafrii::pyc

#endif // __PYC_LIB_IMPLEMENTATION

