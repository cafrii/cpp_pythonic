/*
    pyc_typetraits_17.hpp

    pythonic cpp library
    type traits helper functions, for cpp17 and later.

    Author: yhlee
    Copyright © 2025
*/

//============================================================================

#pragma once

#ifndef __cplusplus
#error this header file is for c++
#endif

//============================================================================


#include <sstream>
#include <tuple>


#include "pyc_pystring.hpp"



//============================================================================
// configs




//============================================================================
// namespace

namespace com::cafrii::pyc {

//============================================================================

/*


    먼저 기본 (primary) 템플릿을 false_type 으로 선언한 후
    특정 조건을 갖춘 특수화를 추가한다.

*/

//----------------------------------------------------------------------------
// is_string<T>
//
// T가 문자열 계통인지 여부. std::string 또는 const char* 여부 판단

template <typename T>
struct is_string : std::disjunction<
    std::is_same<std::decay_t<T>, std::string>,
    std::is_same<std::decay_t<T>, const char*>,
    std::is_same<std::decay_t<T>, char*>
> {};

template <typename T>
constexpr bool is_string_v = is_string<T>::value;


//----------------------------------------------------------------------------
// has_key_type<T>, has_mapped_type<T>
/*
    T::key_type 과 T::mapped_type 의 존재 여부를 검사한다.
*/
template <typename, typename = void>
struct has_key_type : std::false_type {};

template <typename T>
struct has_key_type<T, std::void_t<typename T::key_type>> : std::true_type {};

template <typename, typename = void>
struct has_mapped_type : std::false_type {};

template <typename T>
struct has_mapped_type<T, std::void_t<typename T::mapped_type>> : std::true_type {};

template <typename T> constexpr bool has_key_type_v = has_key_type<T>::value;
template <typename T> constexpr bool has_mapped_type_v = has_mapped_type<T>::value;


//----------------------------------------------------------------------------
// is_container<T>

/*
    T 타입의 값을 std::begin()에 사용할 수 있는지의 여부. (begin-able)

    단, std::string 부류의 타입은 begin()-able 하지만,
    일반적으로 container 로 간주되진 않으므로, 미리 제외시켜 둔다.

    container 는 크게 두 부류로 나뉜다.
    - sequence container : 0이상의 자연수 number로 인덱싱.
    - associated container : key로 인덱싱.
        - 다시 map 과 set 로 나뉨.

    is_xxx_like 와 같은 각각의 타입 검사용 함수 템플릿이 존재한다.
*/

template <typename T, typename = void>
struct is_container : std::false_type {};

template <typename T>
struct is_container<T, std::void_t<decltype(std::begin(std::declval<T>()))>>
    : std::bool_constant<!is_string_v<T>> {};

template <typename T> constexpr bool is_container_v = is_container<T>::value;


//----------------------------------------------------------------------------
// is_pair_like<T>

/*
    T 가 std::pair 처럼 .first/.second 을 지원하는지.

    특이사항:
        std::map::value_type 에도 적용 할 수 있음.
*/
template <typename T, typename = void>
struct is_pair_like : std::false_type {};

template <typename T>
struct is_pair_like<T, std::void_t<
    decltype(std::declval<T>().first),
    decltype(std::declval<T>().second)
>> : std::true_type {};

template <typename T> constexpr bool is_pair_like_v = is_pair_like<T>::value;


//----------------------------------------------------------------------------
// is_tuple_like<T>

/*
    T 가 std::tuple 처럼 get<0>, get<1> 을 지원하는지.
    예시:
        std::tuple, std::array, std::pair

    주의: std::pair 또한 tuple_like 에 포함된다.
*/
template <typename T, typename = void>
struct is_tuple_like : std::false_type {};

template <typename T>
struct is_tuple_like<T, std::void_t<
    decltype(std::get<0>(std::declval<T>())),
    decltype(std::get<1>(std::declval<T>()))
>> : std::true_type {};

template <typename T>
constexpr bool is_tuple_like_v = is_tuple_like<T>::value;


//----------------------------------------------------------------------------
// is_sequence_like<T>

/*
    begin() 가능한 컨테이너 타입 중 T::key_type 이 없는 타입.
    예시:
        vector, list, deque, array..
    참고:
        vector, array, list, deque,..는 모두 T::value_type 이 있다.
        하지만 value_type을 검사에 사용하긴 어렵다. 좀 흔한 이름이어서 의도하지 않은 다른 타입도
        포함될 위험이 있다.
        한 예로 assiciated container 인 set 도 value_type 을 가지고 있다.
*/
template <typename T, typename = void>
struct is_sequence_like : std::false_type {};

template <typename T>
struct is_sequence_like<T, std::void_t<typename T::value_type>>
    : std::bool_constant<
        is_container_v<T> &&
        !has_key_type_v<T>
    > {};

template <typename T>
constexpr bool is_sequence_like_v = is_sequence_like<T>::value;


//----------------------------------------------------------------------------
// is_map_like<T>
//
/*
    key, value 쌍으로 구성된 map 유형
    예시:
        모든 종류의 map 타입
        map, multimap, unordered_map, unordered_multimap
*/
template <typename T, typename = void>
struct is_map_like : std::false_type {};

/*
    아래 방법은 T 타입이 map-like 인지를 판별하는데 있어서 다음의 세 조건을 사용한다.
    - is_container_v<T>. 즉, T 가 begin()-able.
    - T::value_type 이 존재.
    - T::value_type 이 .first, .second 필드를 포함. (즉, pair-like)

    하지만 이 방식으로는 vector<pair<..>> 를 map-like 로 오인하게 된다.
    생김새로는 map 처럼 간주하는 것이 괜찮은 상황도 있을 수 있겠지만 (어쨌든 key-value pair들의 집합 형태이니),
    여러가지 map 고유의 특징을 갖추지 않으므로 일반적으로는 문제가 된다.
    특히, 우리의 경우는 to_string 에 사용할 목적으로 이들을 정의하는 것인데,
    map 과 container 는 string 표시 형식이 다르니, 이 문제를 무시할 수 없다.
*/
#if 0
template <typename T>
struct is_map_like<T, std::void_t<typename T::value_type>>
    : std::bool_constant<
        is_container_v<T> &&
        is_pair_like_v<typename T::value_type>
    > {};

#else
/*
    좀 더 엄격한 map-like 판별을 위해 다음의 특징을 활용한다.
    - T::key_type, T::mapped_type 의 존재 여부를 확인
*/
template <typename T>
struct is_map_like<T, std::void_t<
    typename T::key_type,
    typename T::mapped_type
>> : std::true_type {};

#endif

template <typename T>
constexpr bool is_map_like_v = is_map_like<T>::value;


//----------------------------------------------------------------------------
// is_set_like<T>

template <typename T, typename = void>
struct is_set_like : std::false_type {};

/*
    T::key_type 은 있지만 T::mapped_type 은 없는 타입.
    예시:
    std::set, std::unordered_set, std::multiset, std::unordered_multiset
*/
template <typename T>
struct is_set_like<T, std::void_t<typename T::key_type>>
    : std::bool_constant<
        is_container_v<T> &&
        !has_mapped_type_v<T>
    > {};

template <typename T> constexpr bool is_set_like_v = is_set_like<T>::value;


//----------------------------------------------------------------------------



//============================================================================
}; // namespace com::cafrii::pyc

//============================================================================

