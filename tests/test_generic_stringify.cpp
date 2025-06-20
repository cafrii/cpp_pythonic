/*
    test_generic_stringify.cpp


*/

#include <string_view>
#include <cstdio>
#include <cstring>
#include <vector>
#include <iostream>

#include "pyc_stringify.hpp"
// 정확한 파일 이름은 미정.

/*
    how to test?

    mkdir -p out && cd out
    cmake ..
    make && ./tests/test_generic_stringify

*/

namespace pyc = com::cafrii::pyc;
using namespace pyc;

#define ASSERT2(cond, action, msg, ...) \
	do { if (!(cond)) { printf("!! err, line %d: " msg "\n", __LINE__, ##__VA_ARGS__); \
	fflush(stdout); action; } } while(0)
#define ASSERT(cond, msg, ...) ASSERT2(cond, exit(1), msg, ##__VA_ARGS__)


using namespace std;
namespace pyc = com::cafrii::pyc;


int test_stringify(int argc, char **argv)
{



    // 기본 숫자 벡터
    vector<int> v1 = {1, 2, 3, 4, 5};
    string s = pyc::to_string(v1);
    ASSERT(s == "[1, 2, 3, 4, 5]", "vector");
    // ASSERT(pyc::to_string(v, "{}") == "{1, 2, 3, 4, 5}", "vector{}");

    vector<string> v2 = {"a", "b", "c"};
    ASSERT(pyc::to_string(v2) == "[a, b, c]", "vector-string");

    // tuple / pair
    pair<int,int> p1 = {1, 2};
    ASSERT(pyc::to_string(p1) == "(1, 2)", "pair");
    tuple<int,int,int> t1 = {1, 2, 3};
    ASSERT(pyc::to_string(t1) == "(1, 2, 3)", "tuple");

    // Pair를 담은 벡터
    vector<pair<int, string>> vp = {{1, "one"}, {2, "two"}};
    ASSERT(pyc::to_string(vp) == "[(1, one), (2, two)]", "vector-of-pair");

    // 중첩된 튜플을 담은 벡터
    using MyTuple = tuple<int, int, string>;
    vector<MyTuple> vt = {{1, 2, "apple"}, {3, 4, "banana"}};
    ASSERT(pyc::to_string(vt) == "[(1, 2, apple), (3, 4, banana)]", "vector-of-tuple");

    // 맵
    map<string, int> m = {{"a", 1}, {"b", 2}, {"c", 3}};
    ASSERT(pyc::to_string(m) == "{a: 1, b: 2, c: 3}", "map");

    // 셋
    set<int> s = {5, 2, 8, 1};
    ASSERT(pyc::to_string(s) == "{1, 2, 5, 8}", "set");

    // 맵의 값으로 벡터가 들어간 복잡한 구조
    map<string, vector<int>> mv = {
        {"A", {1, 2, 3}},
        {"B", {4, 5}}
    };
    ASSERT(pyc::to_string(mv) == "{A: [1, 2, 3], B: [4, 5]}", "map-of-vector");


    return 0;
}

int main(int argc, char **argv)
{
	printf("test of pycpp stringify\n");

    return test_stringify(argc, argv);
}