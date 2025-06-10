/*
    pyc_big_integer.hpp

    pythonic cpp library
    big integer class

    Author: yhlee
    Copyright © 2025
*/

//============================================================================

#pragma once

#ifndef __cplusplus
#error this header file is for c++
#endif

//============================================================================


#include <string_view>
// #include <string>
#include <vector>
#include <utility> // in_range

#include "pyc_compare.hpp"


//============================================================================
// configs





//============================================================================
// namespace
namespace com::cafrii::pyc {

//============================================================================

/*
    there are many similar implementations for open internet.
    - https://beeslog.tistory.com/214
    - https://stackoverflow.com/a/53310256  InfInt class
    - https://codeforces.com/blog/entry/16380  BigInt class
*/

/*

*/
class BigInt
{
    // class 내부에 공통적으로 영향을 끼치는 using namespace 대신, 꼭 필요한 일부 타입만 차용한다.
    using Digit = uint8_t;
        // actually, any type T where digit 0..9 can be stored is allowed.
    using dvector = std::vector<Digit>;

    using string = std::string;
    using cstring = const std::string &;

protected:
    /*
        each digit in number is stored as one element of vector array.
        big number is stored reverse order.
        for example, integer 2025 is stored as { 5, 2, 0, 2 }.
    */
    dvector m_nums;

    // internall accessor. it do not check boundness!
    Digit& operator[](int k) { return m_nums[k]; }
    const Digit& operator[](int k) const { return m_nums[k]; }

public:
    // debugging
    string ToStr(cstring opts="") const;
    string Describe() const;

public:
    static const int kDefWidth = 8;

public:
    // ctor
    BigInt(): BigInt(0ULL) {}

    BigInt(unsigned long long nr);
    BigInt(int nr): BigInt((unsigned long long)nr) {}
    // BigInt(0) will be treated as integer 0, not 'NULL'.

    BigInt(const string& s);
    BigInt(const char* cs): BigInt(string(cs)) {}

    // dtor
    virtual ~BigInt() {}

    // copy ctor
    BigInt(const BigInt& other): m_nums(other.m_nums) {}
    // move ctor
    BigInt(BigInt&& other): m_nums(std::move(other.m_nums)) {}

public:
    // assign, move
    BigInt& operator=(const BigInt& other);
    BigInt& operator=(BigInt&& other) noexcept;

public:
    // representation
    bool IsZero() const;
    // returns actually allocated memory sizes
    int Capacity() const;
    // number of net digits.
    int Width() const;



public:
    // pre/postfix increment/decrement
    BigInt& operator++();
    BigInt  operator++(int);
    BigInt& operator--();
    BigInt  operator--(int);


public:
    // binary arithmetic operator
    BigInt& operator+=(const BigInt& rhs) {
        Add_(rhs); return *this;
    }
    friend BigInt operator+(BigInt lhs, const BigInt& rhs) {
        lhs += rhs; return lhs;
    }
    BigInt& operator-=(const BigInt& rhs) {
        Subtract_(rhs); return *this;
    }
    friend BigInt operator-(BigInt lhs, const BigInt& rhs) {
        lhs -= rhs; return lhs;
    }
    // in-place add/subtract
    BigInt& Add_(const BigInt& rhs);
    BigInt& Subtract_(const BigInt& rhs);

public:
    // comparison operator
    bool Less(const BigInt& rhs) const;
    bool Equal(const BigInt& rhs) const;
    // other binary comparison operators are at outside of this class.

public:
    // conversion
#if 0
    template <typename T>
    bool CanFit() const {
        static_assert(std::numeric_limits<T>::is_integer, "T must be an integer type");

        if (IsZero()) return true;

        const auto minVal = std::numeric_limits<T>::min();
        const auto maxVal = std::numeric_limits<T>::max();

        // TODO..
    }
#endif

protected:
    // strip unnecessary digits.
    BigInt& Normalize_();


public:
    // reserve space.
    void Extend_(int capacity);

public:
    // other convenient tools

    BigInt Add(BigInt& rhs) {
        return BigInt(*this).Add_(rhs);
    }


}; // BigInt


// comparison operator
inline bool operator< (const BigInt& lhs, const BigInt& rhs) {
    return lhs.Less(rhs);
}
inline bool operator> (const BigInt& lhs, const BigInt& rhs) {
    return rhs < lhs;
}
inline bool operator<=(const BigInt& lhs, const BigInt& rhs) {
    return !(lhs > rhs);
}
inline bool operator>=(const BigInt& lhs, const BigInt& rhs) {
    return !(lhs < rhs);
}

inline bool operator==(const BigInt& lhs, const BigInt& rhs) {
    return lhs.Equal(rhs);
}
inline bool operator!=(const BigInt& lhs, const BigInt& rhs) {
    return !(lhs == rhs);
}


//============================================================================
}; // namespace com::cafrii::pyc

//============================================================================

#ifdef __PYC_LIB_IMPLEMENTATION

namespace com::cafrii::pyc {
//============================================================================


//-------------------------------------
// ctor/dtor

/*
    number to big integer

    UINT64_MAX is 18446744073709551615.
    maximum 20 digits at most.
*/
BigInt::BigInt(unsigned long long num)
{
    m_nums.reserve(20);
    do {
        m_nums.push_back(num % 10);
        num /= 10;
    } while (num);
}

/*
    string to big integer
    accept string only until it is valid digit number.
*/
BigInt::BigInt(const string& sn)
{
    // find first non-digit character.
    auto it = std::find_if(sn.begin(), sn.end(),
            [](char c){ return !InRange<char>(c, '0', '9'); });
    if (it == sn.begin())
        return; // there is no valid digit in provided string.
    auto len = (it - sn.begin());
    Extend_(len);
    Digit* p = &(*this)[len-1];
    for (auto it2=sn.begin(); it2!=it; it2++) {
        *p-- = (*it2) - '0';
    }
}

//-------------------------------------
// internal representation

/*
    as of now, number zero shoud be {0, } in array.
    actually empty array is not legal, but allow here.
*/
bool BigInt::IsZero() const
{
    return
        m_nums.empty() ||
        (m_nums.size() == 1 && m_nums[0] == 0) ||
        false;
}

/*
    returns actually allocated memory sizes.
    ie, how much can it grow without re-allocation?
    (unit: number of elements)
*/
int BigInt::Capacity() const
{
    return (int)m_nums.size();
}

/*
    returns actually used memory sizes (unit: number of elements)
    this count will skip trailing zeros in memory order.
    ex: Width("00123") == 3
        Width("12340") == 5
        Width("000") == 1
    purely zero "0" will count as 1. ie, Width() always > 0.
*/
int BigInt::Width() const
{
    // 끝자리 0을 제외한, 실제로 0아닌 숫자의 길이.
    int k = m_nums.size()-1;
    for (; k>=0 && !m_nums[k]; k--) (void)0;
    // k는 0아닌 숫자를 포함하는 마지막 요소 인덱스. 0아닌 숫자가 없으면 -1.
    if (k < 0) k = 0;
    return k+1;
}





//-------------------------------------
// assign/move

// copy assign
/*
    할당 연산자 operator=에는 특별한 속성이 있습니다. 자세한 내용은 복사 할당 및 이동 할당을 참조하세요.
    표준 복사 할당 연산자는 자체 할당 시 안전해야 하며, 참조로 좌변을 반환해야 합니다.
*/
BigInt& BigInt::operator=(const BigInt& other)
{
    // Guard self assignment
    if (this == &other)
        return *this;

    // resource reuse will be considered in vector class level.
    m_nums = other.m_nums;
    return *this;
}

/*
    표준 move assign은 이동된 객체를 유효한 상태(즉, 클래스 불변성이 그대로 유지된 상태)로 남겨두고,
    아무것도 하지 않거나, 최소한 자체 할당 시 객체를 유효한 상태로 남겨두고,
    비-const를 참조하여 lh를 반환하며, noexcept가 될 것으로 예상됩니다:
*/
BigInt& BigInt::operator=(BigInt&& other) noexcept
{
    // Guard self assignment
    if (this == &other)
        return *this; // delete[]/size=0 would also be ok

    m_nums = std::move(other.m_nums);
    return *this;
}


//-------------------------------------
// increment / decrement

// prefix increment
BigInt& BigInt::operator++()
{
    Add_(BigInt("1"));
    return *this; // return new value by reference
}
// postfix increment
BigInt BigInt::operator++(int)
{
    BigInt old = *this; // copy old value
    operator++();  // prefix increment
    return old;    // return old value
}
// prefix decrement
BigInt& BigInt::operator--()
{
    Subtract_(BigInt("1"));
    return *this;
}
// postfix decrement
BigInt BigInt::operator--(int)
{
    BigInt old = *this; // copy old value
    operator--();  // prefix decrement
    return old;    // return old value
}


//-------------------------------------
/*
    binary arithmetic operators

    아래와 같이 복합 대입 연산자를 먼저 구현하는 것이 일반적이다.
    see Binary arithmetic operators, in https://en.cppreference.com/w/cpp/language/operators.html .

*/

/*
    in-place add operation.
*/
BigInt& BigInt::Add_(const BigInt& rhs)
{
    int len1 = Width();
    int len2 = rhs.Width();
    int len = std::max(len1, len2);
    this->Extend_(len + 1);

    int carry = 0;
    for (int k=0; k<len; k++) {
        int operand = k < len2 ? rhs.m_nums[k] : 0;
        m_nums[k] += (operand + carry);
        if (m_nums[k] < 10)
            carry = 0;
        else {
            m_nums[k] = m_nums[k] % 10;
            carry = 1;
        }
    }
    if (carry) {
        m_nums[len] = 1;
    }
    return *this;

} // Add_

/*
    in-place subtract operation.
*/
BigInt& BigInt::Subtract_(const BigInt& rhs)
{
    if (*this < rhs) { // underflow?
        // do we support negative big number?
        throw("underflow, not supported yet");
        return *this;
    }
    int len1 = Width();
    int len2 = rhs.Width();
    int len = std::max(len1, len2);
    this->Extend_(len + 1);

    int val, carry = 0;
    for (int k=0; k<len; k++) {
        int operand = k < len2 ? rhs.m_nums[k] : 0;
        val = m_nums[k] - operand + carry;
        if (val >= 0) {
            m_nums[k] = val;
            carry = 0;
        }
        else {
            m_nums[k] = val + 10;
            carry = -1;
        }
    }
    // if (carry) ... // it should not happen.
    return *this;

} // Subtract_

//-------------------------------------
/*
    comparison operator
*/
bool BigInt::Less(const BigInt& rhs) const
{
    int width1 = Width();
    int width2 = rhs.Width();
    if (width1 < width2)
        return true;
    else if (width1 > width2)
        return false;

    auto &v1 = m_nums;
    auto &v2 = rhs.m_nums;

    // lexicographical_compare returns true
    // if the first range is lexicographically less than the second.
    return std::lexicographical_compare(
        v1.rbegin(), v1.rend(),
        v2.rbegin(), v2.rend());
}

bool BigInt::Equal(const BigInt& rhs) const
{
    int width1 = Width();
    int width2 = rhs.Width();
    if (width1 != width2)
        return false;

    // for (int k=0; k<width1; k++) {
    //     if (m_nums[k] != rhs.m_nums[k])
    //         return false;
    // }
    // return true;

    return std::memcmp(&m_nums[0], &rhs.m_nums[0], width1) == 0;
}

//-------------------------------------
// conversion



//-------------------------------------
// modifications

/*
    reserve space.
    it guarantees that 'capacity' elements can be saved after calling it.
*/
void BigInt::Extend_(int capacity)
{
    if (Capacity() < capacity) {
        m_nums.resize(capacity, 0);
    }
}

/*
    trim trailing zeros in num array.
    ie, trim leading zeros in big number.
*/
BigInt& BigInt::Normalize_()
{
    m_nums.resize(Width());
    return *this;
}


//-------------------------------------
// debugging

/*
    opts: TODO
*/
std::string BigInt::ToStr(cstring opts) const
{
    int w = Width();
    std::vector<char> buf(w);
    auto& v = m_nums;
    for (int k=0; k<w; k++) {
        buf[k] = m_nums[w-k-1] + '0';
    }
    return &buf[0];
}

std::string BigInt::Describe() const
{
    string res = ToStr();
    res += " width:" + std::to_string(Width());
    res += " capacity:" + std::to_string(Capacity());
    return res;
}





//============================================================================
}; // namespace com::cafrii::pyc

#endif // __PYC_LIB_IMPLEMENTATION

