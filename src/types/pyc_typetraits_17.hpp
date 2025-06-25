/*
    pyc_tostring.hpp

    pythonic cpp library
    any to string converter

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
#include <map>


#include "pyc_pystring.hpp"




//============================================================================
// configs




//============================================================================
// namespace

namespace com::cafrii::pyc {

//============================================================================

//----------------------------------------------------------------------------
// helper tools

//--------
// is_string<T>: T가 문자열 계통인지 여부.
//    std::string 또는 const char* 여부 판단
template <typename T>
struct is_string : std::disjunction<
    std::is_same<std::decay_t<T>, std::string>,
    std::is_same<std::decay_t<T>, const char*>,
    std::is_same<std::decay_t<T>, char*>
> {};
// 축약형
template <typename T>
constexpr bool is_string_v = is_string<T>::value;

//--------
// is_container<T>: 기본 (primary) 템플릿
template <typename T, typename = void>
struct is_container : std::false_type {};

// 템플릿 특수화. std::begin 가능한 T 타입에 대해서만 true_type.
// string 부류의 타입은 base class 에서 false 가 되어 제외됨.
template <typename T>
struct is_container<T, std::void_t<decltype(std::begin(std::declval<T>()))>>
    : std::bool_constant<!is_string_v<T>> {};
// 축약형
template <typename T>
constexpr bool is_container_v = is_container<T>::value;

//--------
// is_pair_like<T>
// T 가 std::pair 처럼 .first/.second 을 지원하는지.
// std::map::value_type 에도 적용 가능.
//
template <typename T, typename = void>
struct is_pair_like : std::false_type {};

template <typename T>
struct is_pair_like<T, std::void_t<
    decltype(std::declval<T>().first),
    decltype(std::declval<T>().second)
>> : std::true_type {};

template <typename T>
constexpr bool is_pair_like_v = is_pair_like<T>::value;

//--------
// is_tuple_like<T>
// T 가 std::tuple 처럼 get<0>, get<1> 을 지원하는지.
// std::tuple, std::array, std::pair 판별 가능.
//
template <typename T, typename = void>
struct is_tuple_like : std::false_type {};

template <typename T>
struct is_tuple_like<T, std::void_t<
    decltype(std::get<0>(std::declval<T>())),
    decltype(std::get<1>(std::declval<T>()))
>> : std::true_type {};

template <typename T>
constexpr bool is_tuple_like_v = is_tuple_like<T>::value;


//--------
// is_map_like<T>
// container 이면서 내부 원소가 pair-like
//
#if 0 // __cplusplus >= 202002L
#error ...
template <typename T>
constexpr bool is_map_like_v =
    is_container_v<T> &&
    requires(T t) {
        typename T::value_type;
        requires requires(typename T::value_type p) {
            typename std::tuple_element_t<0, typename T::value_type>; // p.first
            typename std::tuple_element_t<1, typename T::value_type>; // p.second
        };
    };

#else
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

#endif


//----------------------------------------------------------------------------
// to_string_impl

/*
    기본 타입.
    현재 지원하는 것은 bool 과 numeric.
    std::is_scalar_v<T> 에 해당되는 타입은 더 많이 있는데..
        std::is_arithmetic_v<T>
        std::is_enum_v<T>
        std::is_pointer_v<T>
        std::is_member_pointer_v<T>
        std::is_null_pointer_v<T>

    일단은 이 두 가지부터 지원하도록 한다.
    https://en.cppreference.com/w/cpp/types/is_scalar.html
*/
template <typename T>
std::string to_string_impl(const T& val, std::true_type /* is_scalar */) {
    if constexpr (std::is_same_v<std::decay_t<T>, bool>)
        return val ? "true" : "false";
    else
        return std::to_string(val);
}

/*
    모든 non-scalar 타입을 다 지원하는 것은 아니고, 필요한 것들을 하나씩 보충한다.
    - 문자열 계열의 타입. 따옴표로 quote 처리.
    - std::vector<bool>의 프록시 타입 처리
*/
template <typename T>
std::string to_string_impl(const T& val, std::false_type /* is_scalar */) {
    if constexpr (is_string_v<T>)
        return "'" + std::string(val) + "'";
    else if constexpr (std::is_convertible_v<T, bool>)
        // std::vector<bool>의 프록시 타입 등 bool로 변환 가능한 타입들
        return static_cast<bool>(val) ? "true" : "false";
    else
        static_assert(sizeof(T) == 0, "Unhandled non-scalar non-string type");
}


//----------------------------------------------------------------------------
// forward decl.

// container
template <typename T>
std::enable_if_t<is_container_v<T> && !is_map_like_v<T>, std::string> to_string(const T& container);

// pair
template <typename A, typename B>
std::string to_string(const std::pair<A, B>& p);

// floating-point number
template <typename T>
std::enable_if_t<std::is_floating_point_v<T>, std::string> to_string(const T& val);

// dispatcher
template <typename T>
// std::enable_if_t<!is_container_v<T>, std::string> to_string(const T& val);
std::enable_if_t<!is_container_v<T> && !std::is_floating_point_v<T>, std::string> to_string(const T& val);


//----------------------------------------------------------------------------
// tuple

template <typename Tuple, std::size_t... I>
std::string tuple_to_string_impl(const Tuple& t, std::index_sequence<I...>) {
    std::stringstream ss;
    ss << "(";
    ((ss << (I == 0 ? "" : ", ") << to_string(std::get<I>(t))), ...);
    ss << ")";
    return ss.str();
}

template <typename... Ts>
std::string to_string(const std::tuple<Ts...>& t) {
    return tuple_to_string_impl(t, std::index_sequence_for<Ts...>{});
}

//----------------------------------------------------------------------------
// pair

template <typename A, typename B>
std::string to_string(const std::pair<A, B>& p) {
    return "(" + to_string(p.first) + ", " + to_string(p.second) + ")";
}

//----------------------------------------------------------------------------
// container

/*
    vector, set
    exclude map-like types.
*/

template <typename T>
std::enable_if_t<is_container_v<T> && !is_map_like_v<T>, std::string>
to_string(const T& container) {
    std::stringstream ss;
    ss << "[";
    for (const auto& elem : container) {
        if (&elem != &*container.begin()) ss << ", ";
        ss << to_string(elem);
    }
    ss << "]";
    return ss.str();
}


//----------------------------------------------------------------------------
// map

/*
    map-likes:  map, unordered_map, ...
*/
template <typename T>
std::enable_if_t<is_map_like_v<T>, std::string>
to_string(const T& m) {
    std::stringstream ss;
    ss << "{";
    for (const auto& kv : m) {
        if (&kv != &*m.begin()) ss << ", ";
        ss << to_string(kv.first) << ": " << to_string(kv.second);
    }
    ss << "}";
    return ss.str();
}



//----------------------------------------------------------------------------
/*
    floating point 의 경우 소숫점 이하 자리의 표시 여부 및 지수형태 표기 방법 등
    동일 숫자를 여러가지 방법으로 표현할 수 있는데, std::format() 이 제일 자연스러운 방법 같음.
*/
template <typename T>
std::enable_if_t<std::is_floating_point_v<T>, std::string>
to_string(const T& val) {
    return std::format("{}", val);
}

/*
    dispatcher
    컨테이너 타입 등 위에서 별도로 overloading 한 타입은 제외
*/
template <typename T>
std::enable_if_t<!is_container_v<T> && !std::is_floating_point_v<T>, std::string>
to_string(const T& val) {
    constexpr bool is_scalar = std::is_arithmetic_v<T>;
    return to_string_impl(val, std::integral_constant<bool, is_scalar>{});
}


//============================================================================
}; // namespace com::cafrii::pyc

//============================================================================
