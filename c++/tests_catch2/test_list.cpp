#include <catch2/catch_test_macros.hpp>
#include <string>
#include <vector>
#include "LinkedList.h"

using namespace std;

TEST_CASE("linkList: empty") {
    LinkList l;
    CHECK(l.empty());
    CHECK(l.get().empty());
}

TEST_CASE("linkList: push_front_and_push_back") {
    LinkList l;
    l.push_back("b");
    l.push_front("a");
    l.push_back("c");
    CHECK((l.get() == vector<string>{"a", "b", "c"}));
}

TEST_CASE("linkList: pop_front_and_pop_back") {
    LinkList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    CHECK(l.pop_front() == "a");
    CHECK(l.pop_back() == "c");
    CHECK(l.pop_back() == "b");
    CHECK(l.empty());
    l.push_back("x");
    CHECK((l.get() == vector<string>{"x"}));
}

TEST_CASE("linkList: getAt") {
    LinkList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    CHECK(l.getAt(0) == "a");
    CHECK(l.getAt(2) == "c");
    CHECK_THROWS_AS(l.getAt(-1), std::out_of_range);
    CHECK_THROWS_AS(l.getAt(3), std::out_of_range);
    
    LinkList e;
    CHECK_THROWS_AS(e.getAt(0), std::out_of_range);
}

TEST_CASE("linkList: insertBeforeValue") {
    LinkList l;
    CHECK_FALSE(l.insertBeforeValue("x", "y"));
    l.push_back("b");
    l.push_back("d");
    CHECK(l.insertBeforeValue("b", "a"));
    CHECK(l.insertBeforeValue("d", "c"));
    CHECK_FALSE(l.insertBeforeValue("zzz", "q"));
    CHECK((l.get() == vector<string>{"a", "b", "c", "d"}));
}

TEST_CASE("linkList: insertAfterValue") {
    LinkList l;
    CHECK_FALSE(l.insertAfterValue("x", "y"));
    l.push_back("a");
    l.push_back("c");
    CHECK(l.insertAfterValue("a", "b"));
    CHECK(l.insertAfterValue("c", "d"));
    CHECK_FALSE(l.insertAfterValue("zzz", "q"));
    l.push_back("e");
    CHECK((l.get() == vector<string>{"a", "b", "c", "d", "e"}));
}

TEST_CASE("linkList: removeValue") {
    LinkList l;
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
    l.push_back("x");
    CHECK((l.get() == vector<string>{"b", "x"}));
}

TEST_CASE("linkList: removeBeforeValue") {
    LinkList l;
    CHECK_FALSE(l.removeBeforeValue("a"));
    l.push_back("a");
    CHECK_FALSE(l.removeBeforeValue("a"));
    l.push_back("b");
    l.push_back("c");
    l.push_back("d");
    CHECK_FALSE(l.removeBeforeValue("a"));
    CHECK_FALSE(l.removeBeforeValue("zzz"));
    CHECK(l.removeBeforeValue("b"));
    CHECK(l.removeBeforeValue("d"));
    CHECK((l.get() == vector<string>{"b", "d"}));
}

TEST_CASE("linkList: removeAfterValue") {
    LinkList l;
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
}

TEST_CASE("linkList: find") {
    LinkList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    CHECK(l.find("a") == 0);
    CHECK(l.find("c") == 2);
    CHECK(l.find("zzz") == -1);
}

TEST_CASE("linkList: getAfterValue_getBeforeValue") {
    LinkList l;
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
    
    LinkList e;
    CHECK(e.getAfterValue("a").empty());
    CHECK(e.getBeforeValue("a").empty());
}

TEST_CASE("linkList: getValue") {
    LinkList l;
    l.push_back("a");
    l.push_back("b");
    string out;
    CHECK(l.getValue("b", out));
    CHECK(out == "b");
    CHECK_FALSE(l.getValue("zzz", out));
}

TEST_CASE("linkList: clear") {
    LinkList l;
    l.push_back("a");
    l.push_back("b");
    l.clear();
    CHECK(l.empty());
    l.push_back("c");
    CHECK((l.get() == vector<string>{"c"}));
}

TEST_CASE("linkList: copy_constructor") {
    LinkList a;
    a.push_back("a");
    a.push_back("b");
    LinkList b(a);
    a.pop_front();
    CHECK((b.get() == vector<string>{"a", "b"}));
}

TEST_CASE("linkList: copy_assignment") {
    LinkList a;
    a.push_back("a");
    LinkList b;
    b.push_back("x");
    b.push_back("y");
    b = a;
    CHECK((b.get() == vector<string>{"a"}));
    b = b;
    CHECK((b.get() == vector<string>{"a"}));
}

TEST_CASE("linkList: move_constructor_and_assignment") {
    LinkList a;
    a.push_back("a");
    a.push_back("b");
    LinkList b(std::move(a));
    CHECK((b.get() == vector<string>{"a", "b"}));
    CHECK(a.empty());
    
    LinkList c;
    c.push_back("x");
    c = std::move(b);
    CHECK((c.get() == vector<string>{"a", "b"}));
    CHECK(b.empty());
    
    LinkList& same = c;
    c = std::move(same);
    CHECK((c.get() == vector<string>{"a", "b"}));
}