#include <catch2/catch_test_macros.hpp>
#include <string>
#include <vector>
#include "Stack.h"
using namespace std;

TEST_CASE("stack: empty") {
    Stack s;
    CHECK(s.empty());
    CHECK(s.get().empty());
}

TEST_CASE("stack: order") {
    Stack s;
    s.push("1");
    s.push("2");
    s.push("3");
    CHECK_FALSE(s.empty());
    CHECK(s.pop() == "3");
    CHECK(s.pop() == "2");
    CHECK(s.pop() == "1");
    CHECK(s.empty());
}

TEST_CASE("stack: pop") {
    Stack s;
    CHECK_THROWS_AS(s.pop(), std::out_of_range);
    s.push("a");
    s.pop();
    CHECK_THROWS_AS(s.pop(), std::out_of_range);
}

TEST_CASE("stack: get_return_top") {
    Stack s;
    s.push("1");
    s.push("2");
    CHECK((s.get() == vector<string>{"2", "1"}));
}

TEST_CASE("stack: clear") {
    Stack s;
    s.push("a");
    s.clear();
    CHECK(s.empty());
    s.push("b");
    CHECK(s.pop() == "b");
}

TEST_CASE("stack: copy_constructor") {
    Stack a;
    a.push("1");
    a.push("2");
    Stack b(a);
    CHECK((b.get() == vector<string>{"2", "1"}));
    a.pop();
    CHECK((b.get() == vector<string>{"2", "1"}));
    b.push("3"); 
    CHECK((b.get() == vector<string>{"3", "2", "1"}));
    Stack empty;
    Stack copyOfEmpty(empty);
    CHECK(copyOfEmpty.empty());
}

TEST_CASE("stack: copy_assignment") {
    Stack a, b;
    a.push("a");
    b.push("x");
    b.push("y");
    b = a;
    CHECK((b.get() == vector<string>{"a"}));
    b = b;
    CHECK((b.get() == vector<string>{"a"}));
}

TEST_CASE("stack: move_constructor_and_assignment") {
    Stack a;
    a.push("1");
    a.push("2");
    Stack b(std::move(a));
    CHECK((b.get() == vector<string>{"2", "1"}));
    CHECK(a.empty());
    Stack c;
    c.push("x");
    c = std::move(b);
    CHECK((c.get() == vector<string>{"2", "1"}));
    CHECK(b.empty());
    Stack& same = c;
    c = std::move(same);
    CHECK((c.get() == vector<string>{"2", "1"}));
}

