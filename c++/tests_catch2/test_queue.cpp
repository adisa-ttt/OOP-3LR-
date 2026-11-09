#include <catch2/catch_test_macros.hpp>
#include <string>
#include <vector>
#include "Queue.h"
using namespace std;

TEST_CASE("queue: empty") {
    Queue q;
    CHECK(q.empty());
    CHECK(q.get().empty());
}

TEST_CASE("queue: order") {
    Queue q;
    q.push("1");
    q.push("2");
    q.push("3");
    CHECK_FALSE(q.empty());
    CHECK(q.pop() == "1");
    CHECK(q.pop() == "2");
    CHECK(q.pop() == "3");
    CHECK(q.empty());
}

TEST_CASE("queue: pop") {
    Queue q;
    CHECK_THROWS_AS(q.pop(), std::out_of_range);
    q.push("a");
    q.pop();
    CHECK_THROWS_AS(q.pop(), std::out_of_range);
}

TEST_CASE("queue: get_returns_head") {
    Queue q;
    q.push("1");
    q.push("2");
    CHECK((q.get() == vector<string>{"1", "2"}));
}

TEST_CASE("queue: clear") {
    Queue q;
    q.push("a");
    q.clear();
    CHECK(q.empty());
    q.push("b");
    CHECK(q.pop() == "b");
}

TEST_CASE("queue: copy_constructor") {
    Queue a;
    a.push("1");
    a.push("2");
    Queue b(a);
    CHECK((b.get() == vector<string>{"1", "2"}));
    a.pop();
    CHECK((b.get() == vector<string>{"1", "2"}));
    b.push("3"); 
    CHECK((b.get() == vector<string>{"1", "2", "3"}));
    Queue empty;
    Queue copyOfEmpty(empty);
    CHECK(copyOfEmpty.empty());
}

TEST_CASE("queue: copy_assignment") {
    Queue a, b;
    a.push("1");
    b.push("x");
    b.push("y");
    b = a;
    CHECK((b.get() == vector<string>{"1"}));
    b = b;
    CHECK((b.get() == vector<string>{"1"}));
}

TEST_CASE("queue: move_constructor_and_assignment") {
    Queue a;
    a.push("1");
    a.push("2");
    Queue b(std::move(a));
    CHECK((b.get() == vector<string>{"1", "2"}));
    CHECK(a.empty());
    Queue c;
    c.push("x");
    c = std::move(b);
    CHECK((c.get() == vector<string>{"1", "2"}));
    CHECK(b.empty());
    Queue& same = c;
    c = std::move(same);
    CHECK((c.get() == vector<string>{"1", "2"}));
}