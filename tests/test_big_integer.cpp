/*
    test_big_integer.cpp


*/

#include <string_view>
#include <cstdio>
#include <cstring>

#include "pyc_compare.hpp"
#include "pyc_big_integer.hpp"

/*
    how to test?

    mkdir -p out && cd out
    cmake ..
    make && ./tests/test_big_integer bigint

*/

namespace pyc = com::cafrii::pyc;
using namespace pyc;

#define ASSERT2(cond, action, msg, ...) \
	do { if (!(cond)) { printf("!! err, line %d: " msg "\n", __LINE__, ##__VA_ARGS__); \
	fflush(stdout); action; } } while(0)
#define ASSERT(cond, msg, ...) ASSERT2(cond, exit(1), msg, ##__VA_ARGS__)


int test_compare(int argc, char **argv)
{
    ASSERT(InRange(5, 1, 10), "fail");
    ASSERT(InRange('1', '1', '3'), "fail");

    return 0;
}

int test_bigint(int argc, char **argv)
{
    BigInt bi_null;
    ASSERT(bi_null.ToStr() == "0", "not zero?");

    BigInt bi_zero{0};
    /*
    생성자 "pyc::BigInt::BigInt"의 인스턴스 중 두 개 이상이 인수 목록과 일치합니다.C/C++(309)
    test_big_integer.cpp(35, 5): 함수 "pyc::BigInt::BigInt(unsigned long long num)" (선언됨 줄 215 / "/Users/yhlee/work/github/cafrii/omega2/pycpp/src/types/pyc_big_integer.hpp")
    test_big_integer.cpp(35, 5): 함수 "pyc::BigInt::BigInt(const char *cs)" (선언됨 줄 92 / "/Users/yhlee/work/github/cafrii/omega2/pycpp/src/types/pyc_big_integer.hpp")
    test_big_integer.cpp(35, 5): 인수 형식이 (int)입니다.
    */
    ASSERT(BigInt(0).ToStr() == "0", "not zero?");

    BigInt bi = "123456";
    BigInt bi2 = "0123456";

    printf("bi : %s\n", bi.Describe().c_str());
    printf("bi2: %s\n", bi2.Describe().c_str());

    ASSERT(bi2.ToStr() == "123456", "fail");
    printf("bi = %s\n", bi.ToStr().c_str());

    {
        BigInt bi1 = 123;
        BigInt bi2{bi1};
        BigInt bi3 = bi2; // assign
        ASSERT(bi1 == bi2, "copy ctor");
        ASSERT(bi1 == bi3, "copy ctor");

        BigInt bi4(std::move(bi2));
        ASSERT(bi1 == bi4, "move ctor");
        printf("ctor ok\n");
    }

    ASSERT(BigInt(0).IsZero(), "zero?");
    ASSERT(!BigInt(123).IsZero(), "not zero");
    ASSERT(BigInt(123).Width() == 3, "width");

    {
        BigInt bi_nz{"-0"};
        printf("%s\n", bi_nz.Describe().c_str());

        ASSERT(bi_nz.IsZero(), "zero");
        ASSERT(!bi_nz.IsNegative(), "negative"); // -0 is not negative!
        ASSERT(bi_nz == BigInt(0), "zero");
        ASSERT(bi_nz == BigInt("0"), "zero");
        printf("negative zero ok\n");

        BigInt bi3{"-123"};
        printf("%s\n", bi3.Describe().c_str());

        ASSERT(!bi3.IsZero(), "zero");
        ASSERT(bi3.IsNegative(), "negative"); // -0 is not negative!
        ASSERT(bi3 == BigInt(-123), "-number");
        printf("negative int ok\n");
    }

    {
        BigInt bi = 123;
        printf("assign ok\n");
    }

    ASSERT(BigInt("123") == BigInt("123"), "equal");
    ASSERT(BigInt("234") == BigInt("000234"), "equal");
    ASSERT(BigInt("345") == BigInt("345xx"), "equal");
    ASSERT(BigInt(456) == BigInt("456"), "equal");
    printf("equal ok\n");

    ASSERT(BigInt(123) < BigInt(124), "less");
    ASSERT(BigInt(999) < BigInt(1111), "less");
    ASSERT(BigInt("000999") < BigInt(3333), "less");
    ASSERT(BigInt(2222) > BigInt(88), "greater");
    printf("compare ok\n");

    ASSERT(BigInt(123) + BigInt(1) == BigInt(124), "add");
    ASSERT(BigInt(1234) + 1 == BigInt(1235), "add");
    ASSERT(BigInt(12345) + 11 == 12356, "add");
    ASSERT(123 + BigInt(999) == 1122, "add");
    printf("add ok\n");

    ASSERT(BigInt(123) - BigInt(1) == BigInt(122), "sub");
    ASSERT(BigInt(1234) - 1 == BigInt(1233), "sub");
    ASSERT(BigInt(12345) - 11 == 12334, "sub");
    ASSERT(999 - BigInt(123) == 876, "sub");
    printf("subtract ok\n");

    {
        BigInt bi = 123;
        bi += 2;
        ASSERT(bi == 125, "add=");
        bi -= 5;
        ASSERT(bi == 120, "sub=");
        printf("add= ok\n");
    }

    ASSERT(++BigInt(123) == 124, "pre-increment");
    ASSERT(BigInt(123)++ == 123, "post-increment");
    ASSERT(--BigInt(123) == 122, "pre-decrement");
    ASSERT(BigInt(123)-- == 123, "post-decrement");
    printf("incr/decr ok\n");


    return 0;
}


int main(int argc, char **argv)
{
	printf("test of pyc big integer\n");

	if (argc >= 2 && !strcmp(argv[1], "compare"))
		return test_compare(argc-1, ((argv[1] = argv[0]), argv+1));
	if (argc >= 2 && !strcmp(argv[1], "bigint"))
		return test_bigint(argc-1, ((argv[1] = argv[0]), argv+1));

	printf("usage: %s mode [args..]\n", argv[0]);
	printf("   compare\n");
	printf("   bigint\n");
	return 0;
}


