#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <string>
#include <vector>
#include "Stack.h"
using namespace std;

BOOST_AUTO_TEST_SUITE(stack_test)

BOOST_AUTO_TEST_CASE(empty) {
    Stack s;
    BOOST_CHECK(s.empty());
    BOOST_CHECK(s.get().empty());
}

BOOST_AUTO_TEST_CASE(order) {
    Stack s;
    s.push("1");
    s.push("2");
    s.push("3");
    BOOST_CHECK(!s.empty());
    BOOST_CHECK_EQUAL(s.pop(), "3");
    BOOST_CHECK_EQUAL(s.pop(), "2");
    BOOST_CHECK_EQUAL(s.pop(), "1");
    BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_CASE(pop) {
    Stack s;
    BOOST_CHECK_THROW(s.pop(), std::out_of_range);
    s.push("a");
    s.pop();
    BOOST_CHECK_THROW(s.pop(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(get_return_top) {
    Stack s;
    s.push("1");
    s.push("2");
    BOOST_CHECK((s.get() == vector<string>{"2", "1"}));
}

BOOST_AUTO_TEST_CASE(clear) {
    Stack s;
    s.push("a");
    s.clear();
    BOOST_CHECK(s.empty());
    s.push("b");
    BOOST_CHECK_EQUAL(s.pop(), "b");
}

BOOST_AUTO_TEST_CASE(copy_constructor) {
    Stack a;
    a.push("1");
    a.push("2");
    Stack b(a);
    BOOST_CHECK((b.get() == vector<string>{"2", "1"}));
    a.pop();
    BOOST_CHECK((b.get() == vector<string>{"2", "1"}));
    b.push("3"); 
    BOOST_CHECK((b.get() == vector<string>{"3", "2", "1"}));
    Stack empty;
    Stack copyOfEmpty(empty);
    BOOST_CHECK(copyOfEmpty.empty());
}

BOOST_AUTO_TEST_CASE(copy_assignment) {
    Stack a, b;
    a.push("a");
    b.push("x");
    b.push("y");
    b = a;
    BOOST_CHECK((b.get() == vector<string>{"a"}));
    b = b;
    BOOST_CHECK((b.get() == vector<string>{"a"}));
}

BOOST_AUTO_TEST_CASE(move_constructor_and_assignment) {
    Stack a;
    a.push("1");
    a.push("2");
    Stack b(std::move(a));
    BOOST_CHECK((b.get() == vector<string>{"2", "1"}));
    BOOST_CHECK(a.empty());
    Stack c;
    c.push("x");
    c = std::move(b);
    BOOST_CHECK((c.get() == vector<string>{"2", "1"}));
    BOOST_CHECK(b.empty());
    Stack& same = c;
    c = std::move(same);
    BOOST_CHECK((c.get() == vector<string>{"2", "1"}));
}

BOOST_AUTO_TEST_SUITE_END()
