/*
    test_stringifier.cpp


*/


#include "pyc_tostring.hpp"

// #include "pyc_stringifier.hpp"
// 정확한 파일 이름은 미정.

#include "test_common.hpp"

#include <set>
#include <unordered_set>



/*
    how to test?

    mkdir -p out && cd out
    cmake ..
    make && ./tests/test_stringifier

*/

namespace pyc = com::cafrii::pyc;
// using namespace pyc;
using namespace std;

int test_tostring(int argc, char **argv)
{
    // 숫자
    ASSERT(pyc::to_string(123) == "123", "int_to_str");

    printf("%s\n", pyc::to_string(3.14).c_str());
    ASSERT(pyc::to_string(3.14) == "3.14", "double_to_str");
    ASSERT(pyc::to_string(3.14f) == "3.14", "float_to_str");
    ASSERT(pyc::to_string(3.14L) == "3.14", "longdouble_to_str");
    // printf("%s\n", pyc::to_string(1E5).c_str());
    // ASSERT(pyc::to_string(1.10E5) == "1.1e+5", "double_exp_to_str");
    // 표현이 일관되지 않아서, 예측하기는 어려움.

    ASSERT(pyc::to_string(true) == "true", "bool");
    ASSERT(pyc::to_string(false) == "false", "bool");
    ASSERT(pyc::to_string(1+2==3) == "true", "bool");
    ASSERT(pyc::to_string(4 > 5) == "false", "bool");

    // 문자
    printf("%s\n", pyc::to_string("abc").c_str());
    ASSERT(pyc::to_string("abc") == "'abc'", "str_to_str");

    {   //-------- 벡터 컨테이너
        vector<int> v1 = {1, 2, 3, 4, 5};
        string s = pyc::to_string(v1);
        ASSERT(s == "[1, 2, 3, 4, 5]", "vector");

        vector<string> v2 = {"a", "b", "c"};
        ASSERT(pyc::to_string(v2) == "['a', 'b', 'c']", "vector-string");

        vector<bool> v3 = {true, false};
        // printf("%s\n", pyc::to_string(v3).c_str());
        ASSERT(pyc::to_string(v3) == "[true, false]", "vector-bool");

        vector<vector<int>> v4 = {{1,2,3,}, {4,5,6,},};
        // printf("%s\n", pyc::to_string(v4).c_str());
        ASSERT(pyc::to_string(v4) == "[[1, 2, 3], [4, 5, 6]]", "vector-vector");
    }
    {   // tuple / pair
        pair<int,int> p1 = {1, 2};
        ASSERT(pyc::to_string(p1) == "(1, 2)", "pair");

        tuple<int,int,int> t1 = {1, 2, 3};
        ASSERT(pyc::to_string(t1) == "(1, 2, 3)", "tuple");
    }
    {   // Pair를 담은 벡터
        vector<pair<int, string>> vp = {{1, "one"}, {2, "two"}};
        printf("%s\n", pyc::to_string(vp).c_str());
        ASSERT(pyc::to_string(vp) == "[(1, 'one'), (2, 'two')]", "vector-of-pair");
    }
    {   // 중첩된 튜플을 담은 벡터
        vector<tuple<int,int,string>> vt = {{1, 2, "apple"}, {3, 4, "banana"}};
        printf("%s\n", pyc::to_string(vt).c_str());
        ASSERT(pyc::to_string(vt) == "[(1, 2, 'apple'), (3, 4, 'banana')]", "vector-of-tuple");
    }
    {   // 셋
        set<int> s = {5, 2, 8, 1};
        printf("%s\n", pyc::to_string(s).c_str());
        ASSERT(pyc::to_string(s) == "[1, 2, 5, 8]", "set");
    }
    {
        std::unordered_set<int> us{2, 7, 1, 8, 2, 8};

    }
    {   // 맵
        map<string, int> m = {{"a", 1}, {"b", 2}, {"c", 3}};
        printf("%s\n", pyc::to_string(m).c_str());
        ASSERT(pyc::to_string(m) == "{'a': 1, 'b': 2, 'c': 3}", "map");
    }
    {    // 맵의 값으로 벡터가 들어간 복잡한 구조
        map<string, vector<int>> mv = {
            {"A", {1, 2, 3}},
            {"B", {4, 5}}
        };
        printf("%s\n", pyc::to_string(mv).c_str());
        ASSERT(pyc::to_string(mv) == "{'A': [1, 2, 3], 'B': [4, 5]}", "map-of-vector");
    }
    printf("done\n");
    return 0;
}


#if 0
int test_stringifier(int argc, char **argv)
{
    using S = pyc::Stringifier;

    // S.UseGrouper("vector", "<>"); // use curly brace {} instead of default parentheses ()
    // S.UseDelimeter("vector", " "); // use empty space instead of comma(,) as a delimeter of vector elements.
    // S.UseQuoteForString(true)

    S s{{"delimeter", "/", "grouper", "<>"}};

    // 기본 숫자 벡터
    vector<int> v1 = {1, 2, 3, 4, 5};
    ASSERT(s(v1) == "<1/2/3/4/5>", "vector");

    vector<string> v2 = {"a", "b c", "d"};
    ASSERT(s(v2) == "<a/b c/d>", "vector-string");


    return 0;
}
#endif



int main(int argc, char **argv)
{
	printf("test of pycpp stringify\n");

    return test_tostring(argc, argv);
}
