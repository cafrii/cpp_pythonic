/*

*/

#include <type_traits>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <list>
#include <deque>


#include "test_common.hpp"
#include "pyc_tostring.hpp"

using namespace std;

namespace pyc = com::cafrii::pyc;



int test_meta()
{
    constexpr bool is_scalar = std::is_arithmetic_v<bool>;
    static_assert(is_scalar, "bool is arithmetic type");

    static_assert(pyc::is_map_like_v<std::map<int, int>>, "std::map is map-like");
    static_assert(pyc::is_map_like_v<std::unordered_map<std::string, int>>, "unordered_map is map-like");
    static_assert(!pyc::is_map_like_v<std::vector<int>>, "vector is NOT map-like");


    static_assert(pyc::is_map_like_v<std::map<int, int>>, "map");
    static_assert(!pyc::is_map_like_v<std::vector<std::pair<int, int>>>, "vector<pair>");

    static_assert(pyc::is_set_like_v<std::set<int>>, "set");
    static_assert(pyc::is_set_like_v<std::unordered_multiset<std::string>>, "unordered_multiset");

    static_assert(pyc::is_sequence_like_v<std::vector<int>>, "vector");
    static_assert(pyc::is_sequence_like_v<std::array<double, 3>>, "array");
    static_assert(!pyc::is_sequence_like_v<std::map<int, int>>, "map is not sequence");
    static_assert(!pyc::is_sequence_like_v<std::set<int>>, "set is not sequence");



    return 0;
}

int test_type_cast()
{
    auto s1 = to_string(123);
    // auto s2 = to_string("abc");

    return 0;
}

int test_type_inference(int argc, char **argv)
{
    // template instanciation
    using IntVector = vector<int>;

    // some sub-classes
    struct MyVector: public IntVector {
        int max_count = 100;
        // ...
    };

    ASSERT( (is_same_v<IntVector,vector<int>>), "same vector");

    // these two class is NOT SAME! it is inheritance relation!
    // ASSERT( (is_same_v<IntVector,MyVector>), "same vector");

    printf("%s ok\n", __func__);
    return 0;
}

int test_callable()
{
    struct MyCallable {
        MyCallable(int value = 0) : data_(value) {}

        int operator()(int newval) {
            data_ = newval;
            return data_;
        }
        void operator()(const std::string& msg) {
            cout << msg << endl;
        }
        int operator()() {
            cout << "get" << endl;
            return data_;
        }
        int data_;
    };

    MyCallable c;
    ASSERT(c(123) == 123, "callable");

    ASSERT(MyCallable(456)() == 456, "callable");
    ASSERT(MyCallable{789}() == 789, "callable");


    printf("%s ok\n", __func__);
    return 0;
}

int main(int argc, char **argv)
{
    test_type_inference(argc, argv);

    test_callable();

    return 0;
}
