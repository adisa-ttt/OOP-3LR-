#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <string>
#include <vector>
#include "Queue.h"
using namespace std;

BOOST_AUTO_TEST_SUITE(queue_tests)

BOOST_AUTO_TEST_CASE(empty) {
    Queue q;
    BOOST_CHECK(q.empty());
    BOOST_CHECK(q.get().empty());
}

BOOST_AUTO_TEST_CASE(order) {
    Queue q;
    q.push("1");
    q.push("2");
    q.push("3");
    BOOST_CHECK(!q.empty());
    BOOST_CHECK_EQUAL(q.pop(), "1");
    BOOST_CHECK_EQUAL(q.pop(), "2");
    BOOST_CHECK_EQUAL(q.pop(), "3");
    BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(pop) {
    Queue q;
    BOOST_CHECK_THROW(q.pop(), std::out_of_range);
    q.push("a");
    q.pop();
    BOOST_CHECK_THROW(q.pop(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(get_returns_head) {
    Queue q;
    q.push("1");
    q.push("2");
    BOOST_CHECK((q.get() == vector<string>{"1", "2"}));
}

BOOST_AUTO_TEST_CASE(clear) {
    Queue q;
    q.push("a");
    q.clear();
    BOOST_CHECK(q.empty());
    q.push("b");
    BOOST_CHECK_EQUAL(q.pop(), "b");
}

BOOST_AUTO_TEST_CASE(copy_constructor) {
    Queue a;
    a.push("1");
    a.push("2");
    Queue b(a);
    BOOST_CHECK((b.get() == vector<string>{"1", "2"}));
    a.pop();
    BOOST_CHECK((b.get() == vector<string>{"1", "2"}));
    b.push("3"); 
    BOOST_CHECK((b.get() == vector<string>{"1", "2", "3"}));
    Queue empty;
    Queue copyOfEmpty(empty);
    BOOST_CHECK(copyOfEmpty.empty());
}

BOOST_AUTO_TEST_CASE(copy_assignment) {
    Queue a, b;
    a.push("1");
    b.push("x");
    b.push("y");
    b = a;
    BOOST_CHECK((b.get() == vector<string>{"1"}));
    b = b;
    BOOST_CHECK((b.get() == vector<string>{"1"}));
}

BOOST_AUTO_TEST_CASE(move_constructor_and_assignment) {
    Queue a;
    a.push("1");
    a.push("2");
    Queue b(std::move(a));
    BOOST_CHECK((b.get() == vector<string>{"1", "2"}));
    BOOST_CHECK(a.empty());
    Queue c;
    c.push("x");
    c = std::move(b);
    BOOST_CHECK((c.get() == vector<string>{"1", "2"}));
    BOOST_CHECK(b.empty());
    Queue& same = c;
    c = std::move(same);
    BOOST_CHECK((c.get() == vector<string>{"1", "2"}));
}

BOOST_AUTO_TEST_SUITE_END()
