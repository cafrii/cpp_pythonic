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


#include "types/pyc_typetraits.hpp"




//============================================================================
// configs


// #define PYCFG_USE_UTF8CPP



//============================================================================
// namespace

namespace com::cafrii::pyc {

//============================================================================


//----------------------------------------------------------------------------
// helper tools

/*
    요소 타입 추출 (map의 경우 key, 그 외는 value_type, tuple은 첫 번째 요소 타입)
*/
template <typename T, typename = void>
struct sum_value_type {
    using type = typename std::iterator_traits<decltype(std::begin(std::declval<T>()))>::value_type;
};

template <typename T>
struct sum_value_type<T, std::void_t<typename T::key_type>> {
    using type = typename T::key_type;
};



/*
    sum 연산을 + 연산이 의미가 있는 산술 타입으로만 한정할 것인지는 선택의 문제임.
*/
template <typename T>
constexpr bool is_summable = std::is_arithmetic_v<typename sum_value_type<T>::type>;



//----------------------------------------------------------------------------
/*
    sum()
    add all elements of continer.

    RT를 따로 지정하지 않으면 유추된 기본 타입 사용
*/

template <typename T,
    typename RT = typename sum_value_type<T>::type,
    std::enable_if_t<
        is_iterable_v<T> &&
        is_summable<T>, int> = 0
>
RT sum(const T& container, RT initval = RT{}) {
    static_assert(is_iterable<T>::value, "sum() function requires an iterable type.");

#if 0
    static_assert(std::is_default_constructible_v<RT>,
                  "sum(): Element type must be default constructible.");
    static_assert(std::is_copy_constructible_v<RT>,
                  "sum(): Element type must be copy constructible.");
    static_assert(std::is_copy_assignable_v<RT>,
                  "sum(): Element type must be copy assignable.");
    static_assert(std::is_constructible_v<RT, int>,
                  "sum(): Element type must be constructible from 0.");
    static_assert(std::is_convertible_v<decltype(std::declval<RT>() + std::declval<RT>()), ElemT>,
                  "sum(): Element type must support '+' operator.");
#endif


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
    이 signature는 모호한 부분이 있으므로 삭제!

    sum<long>(..) 는 long 이 container 타입이 아니므로
    RT 타입일 거라고 유추할 수는 있지만
    첫번째 템플릿 인자는 모든 경우에 컨테이너 T 타입,
    또는 tuple 이나 pair 타입 등 덧셈의 대상이 되는 집합형 타입이
    오는 것으로 확정하는 것이 모호성을 줄일 수 있다.

    sum<long>(container) 대신에
    sum(container, 0L) 과 같이 쓰는 것이 더 명확하다.
*/

#if 0
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
#endif //


//----------------------------------------------------------------------------
/*
    sum()
    note: overflow may occur during sum operation.

    앞서 정의된 하나의 sum() 함수로 통일되었음. 이 선언은 제거.
        template <typename T, typename RT>
        RT sum(const T& container, RT initval = RT{})

*/

#if 0
template <typename T>
auto sum(const T& container) {
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
#endif // 0




//============================================================================
}; // namespace com::cafrii::pyc

//============================================================================
