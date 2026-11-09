#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <string>
#include <vector>
#include "LinkedList.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(linkList_tests)

BOOST_AUTO_TEST_CASE(empty) {
    LinkList l;
    BOOST_CHECK(l.empty());
    BOOST_CHECK(l.get().empty());
}

BOOST_AUTO_TEST_CASE(push_front_and_push_back) {
    LinkList l;
    l.push_back("b");
    l.push_front("a");
    l.push_back("c");
    BOOST_CHECK((l.get() == vector<string>{"a", "b", "c"}));
}

BOOST_AUTO_TEST_CASE(pop_front_and_pop_back) {
    LinkList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    BOOST_CHECK_EQUAL(l.pop_front(), "a");
    BOOST_CHECK_EQUAL(l.pop_back(), "c");
    BOOST_CHECK_EQUAL(l.pop_back(), "b");
    BOOST_CHECK(l.empty());
    l.push_back("x");
    BOOST_CHECK((l.get() == vector<string>{"x"}));
}

BOOST_AUTO_TEST_CASE(getAt) {
    LinkList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    BOOST_CHECK_EQUAL(l.getAt(0), "a");
    BOOST_CHECK_EQUAL(l.getAt(2), "c");
    BOOST_CHECK_THROW(l.getAt(-1), std::out_of_range);
    BOOST_CHECK_THROW(l.getAt(3), std::out_of_range);
    
    LinkList e;
    BOOST_CHECK_THROW(e.getAt(0), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(insertBeforeValue) {
    LinkList l;
    BOOST_CHECK(!l.insertBeforeValue("x", "y"));
    l.push_back("b");
    l.push_back("d");
    BOOST_CHECK(l.insertBeforeValue("b", "a"));
    BOOST_CHECK(l.insertBeforeValue("d", "c"));
    BOOST_CHECK(!l.insertBeforeValue("zzz", "q"));
    BOOST_CHECK((l.get() == vector<string>{"a", "b", "c", "d"}));
}

BOOST_AUTO_TEST_CASE(insertAfterValue) {
    LinkList l;
    BOOST_CHECK(!l.insertAfterValue("x", "y"));
    l.push_back("a");
    l.push_back("c");
    BOOST_CHECK(l.insertAfterValue("a", "b"));
    BOOST_CHECK(l.insertAfterValue("c", "d"));
    BOOST_CHECK(!l.insertAfterValue("zzz", "q"));
    l.push_back("e");
    BOOST_CHECK((l.get() == vector<string>{"a", "b", "c", "d", "e"}));
}

BOOST_AUTO_TEST_CASE(removeValue) {
    LinkList l;
    BOOST_CHECK(!l.removeValue("a"));
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    l.push_back("d");
    BOOST_CHECK(l.removeValue("a"));
    BOOST_CHECK(l.removeValue("c"));
    BOOST_CHECK(l.removeValue("d"));
    BOOST_CHECK(!l.removeValue("zzz"));
    BOOST_CHECK((l.get() == vector<string>{"b"}));
    l.push_back("x");
    BOOST_CHECK((l.get() == vector<string>{"b", "x"}));
}

BOOST_AUTO_TEST_CASE(removeBeforeValue) {
    LinkList l;
    BOOST_CHECK(!l.removeBeforeValue("a"));
    l.push_back("a");
    BOOST_CHECK(!l.removeBeforeValue("a"));
    l.push_back("b");
    l.push_back("c");
    l.push_back("d");
    BOOST_CHECK(!l.removeBeforeValue("a"));
    BOOST_CHECK(!l.removeBeforeValue("zzz"));
    BOOST_CHECK(l.removeBeforeValue("b"));
    BOOST_CHECK(l.removeBeforeValue("d"));
    BOOST_CHECK((l.get() == vector<string>{"b", "d"}));
}

BOOST_AUTO_TEST_CASE(removeAfterValue) {
    LinkList l;
    BOOST_CHECK(!l.removeAfterValue("a"));
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    l.push_back("d");
    BOOST_CHECK(l.removeAfterValue("a"));
    BOOST_CHECK(l.removeAfterValue("c"));
    BOOST_CHECK(!l.removeAfterValue("c"));
    BOOST_CHECK(!l.removeAfterValue("zzz"));
    l.push_back("e");
    BOOST_CHECK((l.get() == vector<string>{"a", "c", "e"}));
}

BOOST_AUTO_TEST_CASE(find) {
    LinkList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    BOOST_CHECK_EQUAL(l.find("a"), 0);
    BOOST_CHECK_EQUAL(l.find("c"), 2);
    BOOST_CHECK_EQUAL(l.find("zzz"), -1);
}

BOOST_AUTO_TEST_CASE(getAfterValue_getBeforeValue) {
    LinkList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    l.push_back("d");
    BOOST_CHECK((l.getAfterValue("b") == vector<string>{"c", "d"}));
    BOOST_CHECK(l.getAfterValue("d").empty());
    BOOST_CHECK(l.getAfterValue("zzz").empty());
    BOOST_CHECK((l.getBeforeValue("c") == vector<string>{"a", "b"}));
    BOOST_CHECK(l.getBeforeValue("a").empty());
    BOOST_CHECK(l.getBeforeValue("zzz").empty());
    
    LinkList e;
    BOOST_CHECK(e.getAfterValue("a").empty());
    BOOST_CHECK(e.getBeforeValue("a").empty());
}

BOOST_AUTO_TEST_CASE(getValue) {
    LinkList l;
    l.push_back("a");
    l.push_back("b");
    string out;
    BOOST_CHECK(l.getValue("b", out));
    BOOST_CHECK_EQUAL(out, "b");
    BOOST_CHECK(!l.getValue("zzz", out));
}

BOOST_AUTO_TEST_CASE(clear) {
    LinkList l;
    l.push_back("a");
    l.push_back("b");
    l.clear();
    BOOST_CHECK(l.empty());
    l.push_back("c");
    BOOST_CHECK((l.get() == vector<string>{"c"}));
}

BOOST_AUTO_TEST_CASE(copy_constructor) {
    LinkList a;
    a.push_back("a");
    a.push_back("b");
    LinkList b(a);
    a.pop_front();
    BOOST_CHECK((b.get() == vector<string>{"a", "b"}));
}

BOOST_AUTO_TEST_CASE(copy_assignment) {
    LinkList a;
    a.push_back("a");
    LinkList b;
    b.push_back("x");
    b.push_back("y");
    b = a;
    BOOST_CHECK((b.get() == vector<string>{"a"}));
    b = b;
    BOOST_CHECK((b.get() == vector<string>{"a"}));
}

BOOST_AUTO_TEST_CASE(move_constructor_and_assignment) {
    LinkList a;
    a.push_back("a");
    a.push_back("b");
    LinkList b(std::move(a));
    BOOST_CHECK((b.get() == vector<string>{"a", "b"}));
    BOOST_CHECK(a.empty());
    
    LinkList c;
    c.push_back("x");
    c = std::move(b);
    BOOST_CHECK((c.get() == vector<string>{"a", "b"}));
    BOOST_CHECK(b.empty());
    
    LinkList& same = c;
    c = std::move(same);
    BOOST_CHECK((c.get() == vector<string>{"a", "b"}));
}

BOOST_AUTO_TEST_SUITE_END()