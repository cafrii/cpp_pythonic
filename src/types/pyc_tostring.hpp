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
#include "pyc_typetraits.hpp"




//============================================================================
// configs




//============================================================================
// namespace

namespace com::cafrii::pyc {

//============================================================================


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
