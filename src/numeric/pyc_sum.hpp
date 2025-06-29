/*
    pyc_sum.hpp

    pythonic cpp library
    generic sum (accumulation)

    Author: yhlee
    Copyright © 2025
*/

//============================================================================

#pragma once

#ifndef __cplusplus
#error this header file is for c++
#endif

//============================================================================


#include "types/pyc_pystring.hpp"
#include "types/pyc_typetraits.hpp"




//============================================================================
// configs


// #define PYCFG_USE_UTF8CPP



//============================================================================
// namespace

namespace com::cafrii::pyc {

//============================================================================


//----------------------------------------------------------------------------
/*
    sum()
    add all elements of continer.
*/

template <typename T, typename RT>
RT sum(const T& container, RT initval) {
    static_assert(is_iterable<T>::value, "sum() function requires an iterable type.");

    RT total = initval;
    if constexpr (is_map_like_v<T>) {
        for (const auto& pair : container)
            total += pair.first;
    } else {
        for (const auto& value : container)
            total += value;
    }
    return total;
}

//----------------------------------------------------------------------------
/*
    example usage:
        sum<long>(v)
    container 의 타입은 deduct 되어 사용.
*/
template <typename RT, typename T>
RT sum(const T& container) {
    // RT 의 기본값으로 초기화 한 후 계산.
    return sum(container, RT{});
}


//----------------------------------------------------------------------------
/*
    sum()
    note: overflow may occur during sum operation.
*/
template <typename T>
auto sum(const T& container)
{
    static_assert(is_iterable_v<T>, "sum(): Container must be iterable.");

#if 0
    // 다음 코드는 아주 그럴사 해 보이지만,
    // map-like 하지 않은 모든 타입 T에 대해서도 T::key_type 검사를 시도하기 때문에
    // 실제로는 사용할 수 없다.
    //
    using ElemT = std::conditional_t<
        is_map_like_v<T>,
        typename T::key_type,           // map의 경우 key만
        typename T::value_type          // 일반 컨테이너는 값 자체
    >;
#endif

#if 0
    // 다음과 같은 조건 체크가 들어가면 좋겠으나..
    static_assert(std::is_default_constructible_v<ElemT>,
                  "sum(): Element type must be default constructible.");
    static_assert(std::is_copy_constructible_v<ElemT>,
                  "sum(): Element type must be copy constructible.");
    static_assert(std::is_copy_assignable_v<ElemT>,
                  "sum(): Element type must be copy assignable.");
    static_assert(std::is_constructible_v<ElemT, int>,
                  "sum(): Element type must be constructible from 0.");
    static_assert(std::is_convertible_v<decltype(std::declval<ElemT>() + std::declval<ElemT>()), ElemT>,
                  "sum(): Element type must support '+' operator.");
#endif

    if constexpr (is_map_like_v<T>) {
        using ElemT = typename T::key_type;
        ElemT result = ElemT{0};
        for (const auto& pair : container)
            result += pair.first;
        return result;
    }
    else {
        using ElemT = typename T::value_type;
        ElemT result = ElemT{0};
        for (const auto& item : container)
            result += item;
        return result;
    }
}




//============================================================================
}; // namespace com::cafrii::pyc

//============================================================================
