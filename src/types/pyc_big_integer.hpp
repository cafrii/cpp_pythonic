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
    - https://www.geeksforgeeks.org/bigint-big-integers-in-c-with-example/

    in github, following repo received stars.
    - https://github.com/faheel/BigInt
*/

/*
    Big Integer manipulation class
    which support arbitrary precision integer numbers.

*/
class BigInt
{
    // class 내부에 공통적으로 영향을 끼치는 using namespace 대신, 꼭 필요한 일부 타입만 차용한다.
    using Digit = uint8_t;
        // actually, any type T where digit 0..9 can be stored is allowed.
    using DgtVec = std::vector<Digit>;

    using string = std::string;
    using cstring = const std::string &;

protected:
    /*
        number is stored as sign-flag and magnitude.

        each digit in magnitude is stored as one element of vector array.
        big number is stored reverse order.
        for example, integer 2025 is stored as { 5, 2, 0, 2 }.

        unnecessary trailing 0 is not allowed.
            ex: {3,2,1,0,0} is not allowed. it should be {3,2,1}.
        in case of zero, magnitude contains one zero digit. {0}.

        negative zero is not allowed. ex: {0} with m_sign=true is not allowed.

    */
    bool m_sign = false;  // true if negative
    DgtVec m_digits;

    // internall accessor. it does not check boundness!
    Digit& operator[](int k) { return m_digits[k]; }
    const Digit& operator[](int k) const { return m_digits[k]; }

public:
    // debugging
    string ToStr(cstring opts="") const;
    string Describe() const;

public:
    // static const int kDefWidth = 8; // not used.

public:
    // ctor
    BigInt();
    BigInt(long long nr);
    BigInt(int nr): BigInt((long long)nr) {}
    // BigInt(0) will be treated as integer 0, not 'NULL'.

    BigInt(const string& s);
    BigInt(const char* cs): BigInt(string(cs)) {}

    // dtor
    virtual ~BigInt() {}

    // copy ctor
    BigInt(const BigInt& other);
    // move ctor
    BigInt(BigInt&& other);

public:
    // assign, move
    BigInt& operator=(const BigInt& other);
    BigInt& operator=(BigInt&& other) noexcept;

public:
    // representation
    bool IsZero() const;
    bool IsNegative() const { return m_sign; }

    // returns actually allocated memory sizes
    int Capacity() const;

    // number of net digits.
    int Width() const { return Width(m_digits); }
    static int Width(const DgtVec& digits);

    // return cloned-copy
    BigInt Clone() const { return *this; }

    BigInt&& Move() { return std::move(*this); }

public:
    // unary arithmetic
    BigInt operator+() const { return *this; };
    BigInt operator-() const { return Clone().Inv_(); };

    BigInt Abs() const;

protected:
    // in-place inversed sign operation
    BigInt& Inv_();
    BigInt& Sign_(bool bSign);

public:
    // pre/postfix increment/decrement
    BigInt& operator++();
    BigInt  operator++(int);
    BigInt& operator--();
    BigInt  operator--(int);


public:
    // binary arithmetic operator
    BigInt& operator+=(const BigInt& rhs) {
        return Add_(rhs);
    }
    friend BigInt operator+(BigInt lhs, const BigInt& rhs) {
        lhs += rhs; return lhs;
    }
    BigInt& operator-=(const BigInt& rhs) {
        return Subtract_(rhs);
    }
    friend BigInt operator-(BigInt lhs, const BigInt& rhs) {
        lhs -= rhs; return lhs;
    }
protected:
    BigInt& AddMag_(const DgtVec& rhs, bool bInvSign=false, bool bNormalize=true);
    BigInt& SubtractMag_(const DgtVec& rhs,  bool bInvSign=false, bool bNormalize=true);

public:
    // in-place add/subtract
    BigInt& Add_(const BigInt& rhs);
    BigInt& Subtract_(const BigInt& rhs);

public:
    // comparison operator
    bool Less(const BigInt& rhs) const;
    bool Equal(const BigInt& rhs) const;
    // other binary comparison operators are at outside of this class.

protected:
    // compare magnitude only
    bool LessMag(const DgtVec& rhs) const;
    bool EqualMag(const DgtVec& rhs) const;

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
    default ctor
*/
BigInt::BigInt():
    m_sign(false), m_digits({0})
{
}

/*
    copy/move ctor
*/
BigInt::BigInt(const BigInt& other):
    m_sign(other.m_sign), m_digits(other.m_digits)
{
}
BigInt::BigInt(BigInt&& other):
    m_sign(other.m_sign), m_digits(std::move(other.m_digits))
{
}


/*
    number to big integer

    UINT64_MAX is 18446744073709551615.
    maximum 20 digits at most.
*/
BigInt::BigInt(long long num)
{
    if (num < 0) {
        m_sign = true;
        num = -num;
    }
    m_digits.reserve(20); // note that it does not increase m_digits vector memory!
    do {
        m_digits.push_back(num % 10);
        num /= 10;
    } while (num);
    // no need to normalize.
}

/*
    string to big integer
    accept string only until it is valid digit number.

    initial + sign is allowed.
*/
BigInt::BigInt(const string& sn): BigInt()
{
    bool bNegative = false;
    auto beg = sn.begin();
    if (beg != sn.end()) {
        if (*beg == '-') {
            bNegative = true;
            beg++;
        }
        else if (*beg == '+') { // +는 선택 사항.
            beg++;
        }
    }
    // find first non-digit character.
    auto it = std::find_if(beg, sn.end(),
            [](char c){ return !InRange<char>(c, '0', '9'); });
    if (it == beg)
        return; // there is no valid digit in provided string.
    auto len = (it - beg);
    Extend_(len);
    Digit* p = &(*this)[len-1];
    for (auto it2=beg; it2!=it; it2++) {
        *p-- = (*it2) - '0';
    }
    m_sign = bNegative;
    Normalize_();
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
        m_digits.empty() || // <- actually this is illegal state.
        (m_digits.size() == 1 && m_digits[0] == 0) ||
        false;
}

/*
    returns actually allocated memory sizes.
    ie, how much can it grow without re-allocation?
    (unit: number of elements)
*/
int BigInt::Capacity() const
{
    return (int)m_digits.size();
}

/*
    returns actually used memory sizes (unit: number of elements)
    this count will skip trailing zeros in memory order.
    ex: Width("00123") == 3
        Width("12340") == 5
        Width("000") == 1
    purely zero "0" will count as 1. ie, Width() always > 0.

    부호는 제외하고, 절대값 숫자의 표현 크기를 계산한다.
    뒤집혀 저장된 상태에서, 끝자리 0을 제외한 실제로 0아닌 숫자의 길이를 리턴한다.
    Width() 리턴값은 항상 >= 1 이다.
*/
// static
int BigInt::Width(const DgtVec& digits)
{
    int k = digits.size()-1;
    for (; k>=0 && !digits[k]; k--) (void)0;
    // k is the last index which is trailing non-zero.
    // if there are no non-zero digit, it will be -1.
    // k는 0아닌 숫자를 포함하는 마지막 요소 인덱스. 0아닌 숫자가 없으면 -1.
    if (k < 0) k = 0;
    return k+1; // convert index to length
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
    m_digits = other.m_digits;
    m_sign = other.m_sign;
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

    m_digits = std::move(other.m_digits);
    m_sign = other.m_sign;
    return *this;
}


//-------------------------------------
// unary arithmetic

/*
    return absolute value as BigInt instance.
    it always return cloned.
*/
BigInt BigInt::Abs() const
{
    return Clone().Sign_(false);
}

/*
    make inverse (toggle sign flag), in-place.
*/
BigInt& BigInt::Inv_()
{
    if (IsZero()) return *this;
    m_sign = !m_sign;
    return *this;
}
/*
    set sign flag, in-place.
*/
BigInt& BigInt::Sign_(bool bSign)
{
    m_sign = bSign;
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

    see Binary arithmetic operators, in https://en.cppreference.com/w/cpp/language/operators.html
*/

/*
    add magnitude part.
    *this will be modified. (in-place op.)

    result is returned after normalization (can be optionally disabled).
*/
BigInt& BigInt::AddMag_(const DgtVec& digits, bool bInvSign, bool bNormalize)
{
    // assume digits digit vector is normalized.
    int len2 = (int)digits.size();
    int len = std::max((int)m_digits.size(), len2);
    this->Extend_(len + 1);

    int carry = 0;
    for (int k=0; k<len; k++) {
        int operand = k < len2 ? digits[k] : 0;
        m_digits[k] += (operand + carry);
        if (m_digits[k] < 10)
            carry = 0;
        else {
            m_digits[k] = m_digits[k] % 10;
            carry = 1;
        }
    }
    if (carry) {
        m_digits[len] = 1;
    }
    if (bInvSign) Inv_();
    if (bNormalize) Normalize_();
    return *this;
} // AddMag_

/*
    in-place add operation.
    it will be normalized after operation.
*/
BigInt& BigInt::Add_(const BigInt& rhs)
{
#if 0
    if (!m_sign && rhs.m_sign) { // positive + negative
        // (NOT OPTIMUM!)
        //    return Subtract_(-rhs);
        //
        // if use just use Subtract_(), rhs is cloned twice!!
        //   ex: 3 + (-5) = 3 - cloned(5) = clone(clone(5)) - 3
        // so, SubtractMag_ is preferred after checking.
        //
        if (LessMag(rhs.m_digits)) {
            // ex: 3 + (-5) = cloned(-5) - 3 = -(cloned(5) - 3)
            auto res = rhs; // copy
            *this = res.SubtractMag_(m_digits, false).Move();
            return *this;
        }
        else {
            // ex: 5 + (-3) = 5 - 3
            return SubtractMag_(rhs.m_digits, false);
        }
    }
    if (m_sign && !rhs.m_sign) { // negative + positive
        // return Inv_().Subtract_(rhs).Inv_();
        if (LessMag(rhs.m_digits)) {
            // ex: -3 + 5 => -(cloned(5) - 3)
            auto res = rhs; // copy
            *this = res.SubtractMag_(m_digits, false).Move();
            return *this;
        }
        else {
            // ex: -5 + 3 => -(5 - 3)
            return SubtractMag_(rhs.m_digits, false);
        }
    }
    // two signs are equal.
    return AddMag_(rhs.m_digits, false); // keeping sign flag

#else
    if (m_sign == rhs.m_sign) { // same sign
        return AddMag_(rhs.m_digits);
    }
    // different sign
    if (LessMag(rhs.m_digits)) { // rhs has bigger magnitude
        // -11 + 222
        auto bigger = rhs; // copy
        printf("bigger %s\n", bigger.ToStr().c_str());
        *this = bigger.SubtractMag_(m_digits).Move();
        return *this;
    }
    else {
        return SubtractMag_(rhs.m_digits, false);
    }

#endif
} // Add_

/*
    in-place subtract magnitude-only operation.
    warning:
        it should be called only when
        this magnitude is greater than or equal to others.
*/
BigInt& BigInt::SubtractMag_(const DgtVec& digits, bool bInvSign, bool bNormalize)
{
    if (LessMag(digits)) {
        throw("underflow!");
        return *this;
    }
    int len2 = Width(digits);
    int len = std::max(Width(), len2);
    this->Extend_(len + 1);

    int val, carry = 0;
    for (int k=0; k<len; k++) {
        int operand = k < len2 ? digits[k] : 0;
        val = m_digits[k] - operand + carry;
        if (val >= 0) {
            m_digits[k] = val;
            carry = 0;
        }
        else {
            m_digits[k] = val + 10;
            carry = -1;
        }
    }
    // if (carry) ... // it should not happen.
    if (bInvSign) Inv_();
    if (bNormalize) Normalize_();
    return *this;
} // SubtractMag_


/*
    in-place subtract operation.

*/
BigInt& BigInt::Subtract_(const BigInt& rhs)
{
    // different sign
#if 0
    if (!m_sign && rhs.m_sign) { // positive - negative
        return AddMag_(rhs.m_digits, false);
    }
    else if (m_sign && !rhs.m_sign) { // negative - positive
        return AddMag_(rhs.m_digits, false);
    }
#else
    if (!m_sign == rhs.m_sign) { // different sign
        // final sign always follows first operand (this).
        return AddMag_(rhs.m_digits);
    }
#endif
    // same sign
    if (LessMag(rhs.m_digits)) {
        auto bigger = rhs; // copy
        *this = bigger.SubtractMag_(m_digits, true).Move(); // invert sign
        return *this;
    }
    else {
        return SubtractMag_(rhs.m_digits, false);
    }
} // Subtract_


//-------------------------------------
/*
    comparison operator
*/

/*
    compare magnitude only between this->digits and rhs.
    return true if this->digits are less than rhs.

    assume both digitvec is normalized.
*/
bool BigInt::LessMag(const DgtVec& rhs) const
{
    int width1 = Width();
    int width2 = Width(rhs);
    if (width1 < width2)
        return true;
    else if (width1 > width2)
        return false;

    auto &v1 = m_digits;
    auto &v2 = rhs;

    // lexicographical_compare returns true
    // if the first range is lexicographically less than the second.
    return std::lexicographical_compare(
        v1.rbegin(), v1.rend(),
        v2.rbegin(), v2.rend());
}

bool BigInt::EqualMag(const DgtVec& rhs) const
{
    int width1 = Width();
    int width2 = Width(rhs);
    if (width1 != width2)
        return false;

    return std::memcmp(&m_digits[0], &rhs[0], width1) == 0;
}


/*
    if (*this < rhs) return true
*/
bool BigInt::Less(const BigInt& rhs) const
{
    if (!m_sign && !rhs.m_sign) // both positive
        return LessMag(rhs.m_digits);
    else if (m_sign && rhs.m_sign) // both negative
        // ex: -5 <? -3  => 3 <? 5 => true
        //     -4 <? -4  => 4 <? 4 => false
        return rhs.LessMag(m_digits);
    else // different sign
        return m_sign;
}

bool BigInt::Equal(const BigInt& rhs) const
{
    return m_sign == rhs.m_sign && EqualMag(rhs.m_digits);
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
        m_digits.resize(capacity, 0);
    }
}

/*
    trim trailing zeros in num array.
    ie, trim leading zeros in big number.
*/
BigInt& BigInt::Normalize_()
{
    m_digits.resize(Width());

    if (m_sign && m_digits.size() == 1 && m_digits[0] == 0)
        m_sign = false;
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
    auto& v = m_digits;
    for (int k=0; k<w; k++) {
        buf[k] = m_digits[w-k-1] + '0';
    }
    return &buf[0];
}

std::string BigInt::Describe() const
{
    string res = ToStr();
    res += ", width:" + std::to_string(Width());
    res += ", capacity:" + std::to_string(Capacity());
    res += ", ";
    for (int k=0; k<(int)m_digits.size(); k++) {
        res += (k>0?" ":"") + std::to_string(m_digits[k]);
    }
    if (m_sign)
        res += ", negative";
    return res;
}





//============================================================================
}; // namespace com::cafrii::pyc

#endif // __PYC_LIB_IMPLEMENTATION

