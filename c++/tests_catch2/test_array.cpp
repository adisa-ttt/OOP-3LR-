#include <catch2/catch_test_macros.hpp>
#include <string>
#include <vector>
#include "Array.h"
using namespace std;

TEST_CASE("array: empty") {
    Array a;
    CHECK(a.empty());
    CHECK(a.getSize() == 0);
    CHECK(a.get().empty());
}

TEST_CASE("array: check_capacity") {
    Array a;
    for (int i = 0; i < 100; i++) {
        a.push_back(to_string(i)); }
    CHECK(a.getSize() == 100);
    for (int i = 0; i < 100;i++) {
        CHECK(a.getAt(i) == to_string(i)); }
}

TEST_CASE("array: push_back_and_getAt") {
    Array a;
    a.push_back("a");
    a.push_back("b");
    a.push_back("c");
    CHECK_FALSE(a.empty());
    CHECK(a.getSize() == 3);
    CHECK(a.getAt(0) == "a");
    CHECK(a.getAt(2) == "c");
}

TEST_CASE("array: getAt_out_of_range") {
    Array a;
    CHECK_THROWS_AS(a.getAt(0), out_of_range);
    a.push_back("First");
    CHECK_THROWS_AS(a.getAt(-1), out_of_range);
    CHECK_THROWS_AS(a.getAt(1), out_of_range);
}

TEST_CASE("array: insert_into") {
    Array a;
    a.push_back("b");
    a.push_back("d");
    CHECK(a.insert(0, "a"));
    CHECK(a.insert(2, "c"));
    CHECK(a.insert(4, "e"));
    CHECK((a.get() == vector<string>{"a", "b", "c", "d", "e"}));
}

TEST_CASE("array: insert_with_bad_index") {
    Array a;
    a.push_back("a");
    CHECK_FALSE(a.insert(-1, "b"));
    CHECK(a.insert(100, "c"));
    CHECK((a.get() == vector<string>{"a", "c"}));
}

TEST_CASE("array: insert_and_resize") {
    Array a;
    for (int i = 0; i < 4; i++) a.push_back("old");
    CHECK(a.insert(1, "new"));
    CHECK(a.getAt(1) == "new");
    CHECK(a.getSize() == 5);    
}

TEST_CASE("array: removeAt") {
    Array a;
    for (string s : {"a", "b", "c"}) a.push_back(s);
    CHECK_FALSE(a.removeAt(-1));
    CHECK_FALSE(a.removeAt(3));
    CHECK(a.removeAt(1));
    CHECK((a.get() == vector<string>{"a", "c"}));
    CHECK(a.removeAt(1));
    CHECK(a.removeAt(0));
    CHECK(a.empty());
    CHECK_FALSE(a.removeAt(0));
}

TEST_CASE("array: set") {
    Array a;
    a.push_back("a");
    CHECK(a.set(0, "z"));
    CHECK(a.getAt(0) == "z");
    CHECK_FALSE(a.set(1, "q"));
    CHECK_FALSE(a.set(-1, "q"));
}

TEST_CASE("array: clear") {
    Array a;
    a.push_back("a");
    a.clear();
    CHECK(a.empty());
    CHECK(a.getSize() == 0);
    a.push_back("b");
    CHECK(a.getAt(0) == "b");
    a.clear();
    a.clear();
    CHECK(a.empty());
}

TEST_CASE("array: copy_of_empty") {
    Array a;
    Array b(a);
    CHECK(b.empty());
    b.push_back("x");
    CHECK(b.getSize() == 1);
}

TEST_CASE("array: copy_constructor") {
    Array a;
    a.push_back("a");
    a.push_back("b");
    Array b(a);
    CHECK(b.getAt(0) == "a");
    CHECK(b.getSize() == 2);
}

TEST_CASE("array: copy_assignment") {
    Array a, b;
    a.push_back("a");
    b.push_back("x");
    b.push_back("y");
    b = a;
    CHECK((b.get() == vector<string>{"a"}));
    b = b;
    CHECK((b.get() == vector<string>{"a"}));
}

TEST_CASE("array: move_constructor") {
    Array a;
    a.push_back("a");
    a.push_back("b");
    Array b(std::move(a));
    CHECK(b.getSize() == 2);
    CHECK(a.empty());
}

TEST_CASE("array: move_assignment") {
    Array a, b;
    a.push_back("a");
    b.push_back("x");
    b = std::move(a);
    CHECK((b.get() == vector<string>{"a"}));
    CHECK(a.empty());
    Array& same = b;
    b = std::move(same);  
    CHECK((b.get() == vector<string>{"a"}));
}