/*
    test_numeric.cpp


*/


#include "pyc_sum.hpp"

#include "test_common.hpp"

#include <vector>
#include <map>
#include <set>



/*
    how to test?

    mkdir -p out && cd out
    cmake ..
    ninja && ./tests/test_numeric

*/

namespace pyc = com::cafrii::pyc;
using namespace std;

int test_sum(int argc, char **argv)
{
    {
        vector<int> v = {1, 2, 3};
        map<int, string> m = {{1, "a"}, {2, "b"}, {3, "c"}};
        set<int> s = {4, 5, 6};

        cout << "sum(v) = " << pyc::sum(v) << "\n";     // 6
        cout << "sum(m) = " << pyc::sum(m) << "\n";     // 6 (key만 합산)
        cout << "sum(s) = " << pyc::sum(s) << "\n";     // 15

        ASSERT(pyc::sum(v) == 6, "");
        ASSERT(pyc::sum(m) == 6, "");
        ASSERT(pyc::sum(s) == 15, "");
    }

    // compile error: unsupported type
    // struct X {}; vector<X> xvec; sum(xvec);

    {
        // overflow-proof

        vector<char> vc = { 100, 100, 100, };
        ASSERT(pyc::sum(vc, 0) == 300, "");

        vector<short> vs = { 30000, 30000, 30000, };
        ASSERT(pyc::sum(vs, 0) == 90000, "");

        vector<int> vi = { 2000000000, 2000000000, 2000000000, };
        ASSERT(pyc::sum(vi, 0L) == 6000000000, "");

        vector<int> vi2 = { 2000000000, 2000000000, 2000000000, };
        ASSERT(pyc::sum(vi2,0L) == 6000000000, "");
    }

    {   // literal
        cout << pyc::sum<vector<int>>({1, 2, 3, 4, 5}) << endl;
        cout << pyc::sum(vector<int>{1, 2, 3, 4, 5}) << endl;
    }

    {   // std::array
        array<int, 3> arr = {1, 2, 3};
        // cout << pyc::sum(arr) << endl;
        ASSERT(pyc::sum(arr) == 6, "");
    }

    {   // std::tuple
        tuple<long,long> tpl2 = {10, 100};
        ASSERT(pyc::sum(tpl2) == 110, "");

        tuple<int,int,int> tpi3 = {1, 2, 3};
        ASSERT(pyc::sum(tpi3) == 6, "");

        tuple<short,short> tps2 = {30000, 30000};
        cout << pyc::sum(tps2, 0) << endl;
        ASSERT(pyc::sum(tps2, 0) == 60000, "");

        // compile error
        // -> static_assert(are_tuple_elements_same<std::tuple<Args...>>, "tuple elements should be same type.");
        // cout << pyc::sum(tuple<short,int>{1, 100}) << endl;

        tuple<> tp0 = {};
        ASSERT(pyc::sum(tp0) == 0, "empty tuple");

        ASSERT(pyc::sum(tuple<>{}, 1U) == 1, "empty tuple with initval");

    }

    printf("done\n");
    return 0;
}

int main(int argc, char **argv)
{
	printf("test of pycpp numeric lib\n");

    return test_sum(argc, argv);
}
