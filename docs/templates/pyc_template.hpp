/*
    pyc_module_name.hpp

    pythonic cpp library
    some class

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


// #define PYCFG_USE_UTF8CPP



//============================================================================
// namespace

namespace com::cafrii::pyc {

//============================================================================

/*
    최종적으로는 선언과 구현이 분리 되면 좋겠으나,
    처음 시작할 때에는 간단하게 header-only로 시작하는 것이 좋을 수 있다.

    처음에 헤더 전용으로 구현이 된 클래스를 나중에 구현을 분리하고자하는 경우
    아래의 템플릿 구조에 맞춰서 분리를 하면 된다.
*/

/*

*/
class SomeClass
{
public:
    SomeClass();
    virtual ~SomeClass();

}; // SomeClass


//============================================================================
}; // namespace com::cafrii::pyc

//============================================================================

#ifdef __PYC_LIB_IMPLEMENTATION

namespace com::cafrii::pyc {
//============================================================================


SomeClass::SomeClass()
{

}
SomeClass::~SomeClass()
{

}


//============================================================================
}; // namespace com::cafrii::pyc

#endif // __PYC_LIB_IMPLEMENTATION

