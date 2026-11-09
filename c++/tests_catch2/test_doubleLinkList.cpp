#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <string>
#include <vector>
#include "DoubleLinkedList.h"

using namespace std;

TEST_CASE("doubleLinkList: empty") {
    doubleLinkList l;
    CHECK(l.empty());
    CHECK(l.get().empty());
    CHECK(l.getReverse().empty());
}

TEST_CASE("doubleLinkList: push_front_and_push_back") {
    doubleLinkList l;
    l.push_back("b");
    l.push_front("a");
    l.push_back("c");
    CHECK((l.get() == vector<string>{"a", "b", "c"}));
    CHECK((l.getReverse() == vector<string>{"c", "b", "a"}));
}

TEST_CASE("doubleLinkList: pop_front_and_pop_back") {
    doubleLinkList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    CHECK(l.pop_front() == "a");
    CHECK(l.pop_back() == "c");
    
    vector<string> fwd = l.get();
    vector<string> back = l.getReverse();
    reverse(back.begin(), back.end());
    CHECK(fwd == back);
    
    CHECK(l.pop_back() == "b");
    CHECK(l.empty());
    l.push_front("x");
    CHECK((l.get() == vector<string>{"x"}));
}

TEST_CASE("doubleLinkList: getAt") {
    doubleLinkList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    CHECK(l.getAt(0) == "a");
    CHECK(l.getAt(2) == "c");
    CHECK_THROWS_AS(l.getAt(-1), std::out_of_range);
    CHECK_THROWS_AS(l.getAt(3), std::out_of_range);
    
    doubleLinkList e;
    CHECK_THROWS_AS(e.getAt(0), std::out_of_range);
}

TEST_CASE("doubleLinkList: insertBeforeValue") {
    doubleLinkList l;
    CHECK_FALSE(l.insertBeforeValue("x", "y"));
    l.push_back("b");
    l.push_back("d");
    CHECK(l.insertBeforeValue("b", "a"));
    CHECK(l.insertBeforeValue("d", "c"));
    CHECK_FALSE(l.insertBeforeValue("zzz", "q"));
    CHECK((l.get() == vector<string>{"a", "b", "c", "d"}));
    
    vector<string> fwd = l.get();
    vector<string> back = l.getReverse();
    reverse(back.begin(), back.end());
    CHECK(fwd == back);
}

TEST_CASE("doubleLinkList: insertAfterValue") {
    doubleLinkList l;
    CHECK_FALSE(l.insertAfterValue("x", "y"));
    l.push_back("a");
    l.push_back("c");
    CHECK(l.insertAfterValue("a", "b"));
    CHECK(l.insertAfterValue("c", "d"));
    CHECK_FALSE(l.insertAfterValue("zzz", "q"));
    CHECK((l.get() == vector<string>{"a", "b", "c", "d"}));
    
    vector<string> fwd = l.get();
    vector<string> back = l.getReverse();
    reverse(back.begin(), back.end());
    CHECK(fwd == back);
}

TEST_CASE("doubleLinkList: removeValue") {
    doubleLinkList l;
    CHECK_FALSE(l.removeValue("a"));
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    l.push_back("d");
    CHECK(l.removeValue("a"));
    CHECK(l.removeValue("c"));
    CHECK(l.removeValue("d"));
    CHECK_FALSE(l.removeValue("zzz"));
    CHECK((l.get() == vector<string>{"b"}));
    
    vector<string> fwd = l.get();
    vector<string> back = l.getReverse();
    reverse(back.begin(), back.end());
    CHECK(fwd == back);
}

TEST_CASE("doubleLinkList: removeBeforeValue") {
    doubleLinkList l;
    CHECK_FALSE(l.removeBeforeValue("a"));
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    l.push_back("d");
    CHECK_FALSE(l.removeBeforeValue("a"));
    CHECK_FALSE(l.removeBeforeValue("zzz"));
    CHECK(l.removeBeforeValue("b"));
    CHECK(l.removeBeforeValue("d"));
    CHECK((l.get() == vector<string>{"b", "d"}));
    
    vector<string> fwd = l.get();
    vector<string> back = l.getReverse();
    reverse(back.begin(), back.end());
    CHECK(fwd == back);
}

TEST_CASE("doubleLinkList: removeAfterValue") {
    doubleLinkList l;
    CHECK_FALSE(l.removeAfterValue("a"));
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    l.push_back("d");
    CHECK(l.removeAfterValue("a"));
    CHECK(l.removeAfterValue("c"));
    CHECK_FALSE(l.removeAfterValue("c"));
    CHECK_FALSE(l.removeAfterValue("zzz"));
    l.push_back("e");
    CHECK((l.get() == vector<string>{"a", "c", "e"}));
    
    vector<string> fwd = l.get();
    vector<string> back = l.getReverse();
    reverse(back.begin(), back.end());
    CHECK(fwd == back);
}

TEST_CASE("doubleLinkList: find_and_getValue") {
    doubleLinkList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    CHECK(l.find("b") == 1);
    CHECK(l.find("zzz") == -1);
    string out;
    CHECK(l.getValue("c", out));
    CHECK(out == "c");
    CHECK_FALSE(l.getValue("zzz", out));
}

TEST_CASE("doubleLinkList: getAfterValue_getBeforeValue") {
    doubleLinkList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    l.push_back("d");
    CHECK((l.getAfterValue("b") == vector<string>{"c", "d"}));
    CHECK(l.getAfterValue("d").empty());
    CHECK(l.getAfterValue("zzz").empty());
    CHECK((l.getBeforeValue("c") == vector<string>{"a", "b"}));
    CHECK(l.getBeforeValue("a").empty());
    CHECK(l.getBeforeValue("zzz").empty());
}

TEST_CASE("doubleLinkList: clear_copy_move") {
    doubleLinkList a;
    a.push_back("a");
    a.push_back("b");
    doubleLinkList b(a);
    a.clear();
    CHECK(a.empty());
    CHECK((b.get() == vector<string>{"a", "b"}));
    
    doubleLinkList c;
    c.push_back("x");
    c = b;
    CHECK((c.get() == vector<string>{"a", "b"}));
    c = c;
    CHECK((c.get() == vector<string>{"a", "b"}));
    
    doubleLinkList d(std::move(c));
    CHECK((d.get() == vector<string>{"a", "b"}));
    CHECK(c.empty());
    
    doubleLinkList e;
    e.push_back("q");
    e = std::move(d);
    CHECK((e.get() == vector<string>{"a", "b"}));
    CHECK(d.empty());
    
    doubleLinkList& same = e;
    e = std::move(same);
    CHECK((e.get() == vector<string>{"a", "b"}));
    
    vector<string> fwd = e.get();
    vector<string> back = e.getReverse();
    reverse(back.begin(), back.end());
    CHECK(fwd == back);
}