#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <algorithm>
#include <string>
#include <vector>
#include "DoubleLinkedList.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(doubleLinkList_tests)

BOOST_AUTO_TEST_CASE(empty) {
    doubleLinkList l;
    BOOST_CHECK(l.empty());
    BOOST_CHECK(l.get().empty());
    BOOST_CHECK(l.getReverse().empty());
}

BOOST_AUTO_TEST_CASE(push_front_and_push_back) {
    doubleLinkList l;
    l.push_back("b");
    l.push_front("a");
    l.push_back("c");
    BOOST_CHECK((l.get() == vector<string>{"a", "b", "c"}));
    BOOST_CHECK((l.getReverse() == vector<string>{"c", "b", "a"}));
}

BOOST_AUTO_TEST_CASE(pop_front_and_pop_back) {
    doubleLinkList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    BOOST_CHECK_EQUAL(l.pop_front(), "a");
    BOOST_CHECK_EQUAL(l.pop_back(), "c");
    
    vector<string> fwd = l.get();
    vector<string> back = l.getReverse();
    reverse(back.begin(), back.end());
    BOOST_CHECK(fwd == back);
    
    BOOST_CHECK_EQUAL(l.pop_back(), "b");
    BOOST_CHECK(l.empty());
    l.push_front("x");
    BOOST_CHECK((l.get() == vector<string>{"x"}));
}

BOOST_AUTO_TEST_CASE(getAt) {
    doubleLinkList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    BOOST_CHECK_EQUAL(l.getAt(0), "a");
    BOOST_CHECK_EQUAL(l.getAt(2), "c");
    BOOST_CHECK_THROW(l.getAt(-1), std::out_of_range);
    BOOST_CHECK_THROW(l.getAt(3), std::out_of_range);
    
    doubleLinkList e;
    BOOST_CHECK_THROW(e.getAt(0), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(insertBeforeValue) {
    doubleLinkList l;
    BOOST_CHECK(!l.insertBeforeValue("x", "y"));
    l.push_back("b");
    l.push_back("d");
    BOOST_CHECK(l.insertBeforeValue("b", "a"));
    BOOST_CHECK(l.insertBeforeValue("d", "c"));
    BOOST_CHECK(!l.insertBeforeValue("zzz", "q"));
    BOOST_CHECK((l.get() == vector<string>{"a", "b", "c", "d"}));
    
    vector<string> fwd = l.get();
    vector<string> back = l.getReverse();
    reverse(back.begin(), back.end());
    BOOST_CHECK(fwd == back);
}

BOOST_AUTO_TEST_CASE(insertAfterValue) {
    doubleLinkList l;
    BOOST_CHECK(!l.insertAfterValue("x", "y"));
    l.push_back("a");
    l.push_back("c");
    BOOST_CHECK(l.insertAfterValue("a", "b"));
    BOOST_CHECK(l.insertAfterValue("c", "d"));
    BOOST_CHECK(!l.insertAfterValue("zzz", "q"));
    BOOST_CHECK((l.get() == vector<string>{"a", "b", "c", "d"}));
    
    vector<string> fwd = l.get();
    vector<string> back = l.getReverse();
    reverse(back.begin(), back.end());
    BOOST_CHECK(fwd == back);
}

BOOST_AUTO_TEST_CASE(removeValue) {
    doubleLinkList l;
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
    
    vector<string> fwd = l.get();
    vector<string> back = l.getReverse();
    reverse(back.begin(), back.end());
    BOOST_CHECK(fwd == back);
}

BOOST_AUTO_TEST_CASE(removeBeforeValue) {
    doubleLinkList l;
    BOOST_CHECK(!l.removeBeforeValue("a"));
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    l.push_back("d");
    BOOST_CHECK(!l.removeBeforeValue("a"));
    BOOST_CHECK(!l.removeBeforeValue("zzz"));
    BOOST_CHECK(l.removeBeforeValue("b"));
    BOOST_CHECK(l.removeBeforeValue("d"));
    BOOST_CHECK((l.get() == vector<string>{"b", "d"}));
    
    vector<string> fwd = l.get();
    vector<string> back = l.getReverse();
    reverse(back.begin(), back.end());
    BOOST_CHECK(fwd == back);
}

BOOST_AUTO_TEST_CASE(removeAfterValue) {
    doubleLinkList l;
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
    
    vector<string> fwd = l.get();
    vector<string> back = l.getReverse();
    reverse(back.begin(), back.end());
    BOOST_CHECK(fwd == back);
}

BOOST_AUTO_TEST_CASE(find_and_getValue) {
    doubleLinkList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    BOOST_CHECK_EQUAL(l.find("b"), 1);
    BOOST_CHECK_EQUAL(l.find("zzz"), -1);
    string out;
    BOOST_CHECK(l.getValue("c", out));
    BOOST_CHECK_EQUAL(out, "c");
    BOOST_CHECK(!l.getValue("zzz", out));
}

BOOST_AUTO_TEST_CASE(getAfterValue_getBeforeValue) {
    doubleLinkList l;
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
}

BOOST_AUTO_TEST_CASE(clear_copy_move) {
    doubleLinkList a;
    a.push_back("a");
    a.push_back("b");
    doubleLinkList b(a);
    a.clear();
    BOOST_CHECK(a.empty());
    BOOST_CHECK((b.get() == vector<string>{"a", "b"}));
    
    doubleLinkList c;
    c.push_back("x");
    c = b;
    BOOST_CHECK((c.get() == vector<string>{"a", "b"}));
    c = c;
    BOOST_CHECK((c.get() == vector<string>{"a", "b"}));
    
    doubleLinkList d(std::move(c));
    BOOST_CHECK((d.get() == vector<string>{"a", "b"}));
    BOOST_CHECK(c.empty());
    
    doubleLinkList e;
    e.push_back("q");
    e = std::move(d);
    BOOST_CHECK((e.get() == vector<string>{"a", "b"}));
    BOOST_CHECK(d.empty());
    
    doubleLinkList& same = e;
    e = std::move(same);
    BOOST_CHECK((e.get() == vector<string>{"a", "b"}));
    
    vector<string> fwd = e.get();
    vector<string> back = e.getReverse();
    reverse(back.begin(), back.end());
    BOOST_CHECK(fwd == back);
}

BOOST_AUTO_TEST_SUITE_END()