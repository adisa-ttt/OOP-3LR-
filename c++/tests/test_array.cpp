#include <boost/test/unit_test.hpp>
#include <string>
#include <vector>
#include "Array.h"
using namespace std;

BOOST_AUTO_TEST_SUITE(array_test);

BOOST_AUTO_TEST_CASE(empty) {
    Array a;
    BOOST_CHECK_EQUAL(a.getSize(), 0);
    BOOST_CHECK(a.get().empty());
    BOOST_CHECK_MESSAGE(a.empty(), "Массив должен быть пуст");
    BOOST_REQUIRE_MESSAGE(a.getSize() == 0, "Размер должен быть 0");
}

BOOST_AUTO_TEST_CASE(check_capacity) {
    Array a;
    for (int i = 0; i < 100; i++) {
        a.push_back(to_string(i)); }
    BOOST_CHECK_EQUAL(a.getSize(), 100);
    for (int i = 0; i < 100;i++) {
        BOOST_CHECK_EQUAL(a.getAt(i), to_string(i)); }
}

BOOST_AUTO_TEST_CASE(push_back_and_getAt) {
    Array a;
    a.push_back("adisa!");
    a.push_back("kate!");
    a.push_back("diana!");
    BOOST_CHECK(!a.empty());
    BOOST_CHECK_EQUAL(a.getSize(), 3);
    BOOST_CHECK_EQUAL(a.getAt(0), "adisa!");
    BOOST_CHECK_EQUAL(a.getAt(2), "diana!");
}

BOOST_AUTO_TEST_CASE(getAt_out_of_range) {
    Array a;
    BOOST_CHECK_THROW(a.getAt(0), out_of_range);
    a.push_back("First");
    BOOST_CHECK_THROW(a.getAt(-1), out_of_range);
    BOOST_CHECK_THROW(a.getAt(1), out_of_range);
}

BOOST_AUTO_TEST_CASE(insert_into) {
    Array a;
    a.push_back("b");
    a.push_back("d");
    BOOST_CHECK(a.insert(0, "a"));
    BOOST_CHECK(a.insert(2, "c"));
    BOOST_CHECK(a.insert(4, "e"));
    BOOST_CHECK((a.get() == vector<string>{"a", "b", "c", "d", "e"}));
}

BOOST_AUTO_TEST_CASE(insert_with_bad_index) {
    Array a;
    a.push_back("a");
    BOOST_CHECK(!a.insert(-1, "b"));
    BOOST_CHECK(a.insert(100, "c"));
    BOOST_CHECK((a.get() == vector<string>{"a", "c"}));
}

BOOST_AUTO_TEST_CASE(insert_and_resize) {
    Array a;
    BOOST_WARN(a.getSize() == 0);
    for (int i = 0; i < 4; i++) a.push_back("old");
    BOOST_CHECK(a.insert(1, "new"));
    BOOST_CHECK_EQUAL(a.getAt(1), "new");
    BOOST_CHECK_EQUAL(a.getSize(), 5);    
}

BOOST_AUTO_TEST_CASE(removeAt) {
    Array a;
    for (string s : {"a", "b", "c"}) a.push_back(s);
    BOOST_CHECK(!a.removeAt(-1));
    BOOST_CHECK(!a.removeAt(3));
    BOOST_CHECK(a.removeAt(1));
    BOOST_CHECK((a.get() == vector<string>{"a", "c"}));
    BOOST_CHECK(a.removeAt(1));
    BOOST_CHECK(a.removeAt(0));
    BOOST_CHECK(a.empty());
    BOOST_CHECK(!a.removeAt(0));
}

BOOST_AUTO_TEST_CASE(set) {
    Array a;
    a.push_back("a");
    BOOST_CHECK(a.set(0, "z"));
    BOOST_CHECK_EQUAL(a.getAt(0), "z");
    BOOST_CHECK(!a.set(1, "q"));
    BOOST_CHECK(!a.set(-1, "q"));
}

BOOST_AUTO_TEST_CASE(clear) {
    Array a;
    a.push_back("a");
    a.clear();
    BOOST_CHECK(a.empty());
    BOOST_CHECK_EQUAL(a.getSize(), 0);
    a.push_back("b");
    BOOST_CHECK_EQUAL(a.getAt(0), "b");
    a.clear();
    a.clear();
    BOOST_CHECK(a.empty());
}

BOOST_AUTO_TEST_CASE(copy_of_empty) {
    Array a;
    Array b(a);
    BOOST_CHECK(b.empty());
    b.push_back("x");
    BOOST_CHECK_EQUAL(b.getSize(), 1);
}

BOOST_AUTO_TEST_CASE(copy_constructor) {
    Array a;
    a.push_back("a");
    a.push_back("b");
    Array b(a);
    BOOST_CHECK_EQUAL(b.getAt(0), "a");
    BOOST_CHECK_EQUAL(b.getSize(), 2);
}

BOOST_AUTO_TEST_CASE(copy_assignment) {
    Array a, b;
    a.push_back("a");
    b.push_back("x");
    b.push_back("y");
    b = a;
    BOOST_CHECK((b.get() == vector<string>{"a"}));
    b = b;
    BOOST_CHECK((b.get() == vector<string>{"a"}));
}

BOOST_AUTO_TEST_CASE(move_constructor) {
    Array a;
    a.push_back("a");
    a.push_back("b");
    Array b(std::move(a));
    BOOST_CHECK_EQUAL(b.getSize(), 2);
    BOOST_CHECK(a.empty());
}

BOOST_AUTO_TEST_CASE(move_assignment) {
    Array a, b;
    a.push_back("a");
    b.push_back("x");
    b = std::move(a);
    BOOST_CHECK((b.get() == vector<string>{"a"}));
    BOOST_CHECK(a.empty());
    Array& same = b;
    b = std::move(same);  
    BOOST_CHECK((b.get() == vector<string>{"a"}));
}

BOOST_AUTO_TEST_SUITE_END()