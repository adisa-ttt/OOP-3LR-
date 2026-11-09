#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "LinkedList.h"

using namespace std;

TEST(linkList_tests, empty) {
    LinkList l;
    EXPECT_TRUE(l.empty());
    EXPECT_TRUE(l.get().empty());
}

TEST(linkList_tests, push_front_and_push_back) {
    LinkList l;
    l.push_back("b");
    l.push_front("a");
    l.push_back("c");
    EXPECT_TRUE((l.get() == vector<string>{"a", "b", "c"}));
}

TEST(linkList_tests, pop_front_and_pop_back) {
    LinkList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    EXPECT_EQ(l.pop_front(), "a");
    EXPECT_EQ(l.pop_back(), "c");
    EXPECT_EQ(l.pop_back(), "b");
    EXPECT_TRUE(l.empty());
    l.push_back("x");
    EXPECT_TRUE((l.get() == vector<string>{"x"}));
}

TEST(linkList_tests, getAt) {
    LinkList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    EXPECT_EQ(l.getAt(0), "a");
    EXPECT_EQ(l.getAt(2), "c");
    EXPECT_THROW(l.getAt(-1), std::out_of_range);
    EXPECT_THROW(l.getAt(3), std::out_of_range);
    
    LinkList e;
    EXPECT_THROW(e.getAt(0), std::out_of_range);
}

TEST(linkList_tests, insertBeforeValue) {
    LinkList l;
    EXPECT_FALSE(l.insertBeforeValue("x", "y"));
    l.push_back("b");
    l.push_back("d");
    EXPECT_TRUE(l.insertBeforeValue("b", "a"));
    EXPECT_TRUE(l.insertBeforeValue("d", "c"));
    EXPECT_FALSE(l.insertBeforeValue("zzz", "q"));
    EXPECT_TRUE((l.get() == vector<string>{"a", "b", "c", "d"}));
}

TEST(linkList_tests, insertAfterValue) {
    LinkList l;
    EXPECT_FALSE(l.insertAfterValue("x", "y"));
    l.push_back("a");
    l.push_back("c");
    EXPECT_TRUE(l.insertAfterValue("a", "b"));
    EXPECT_TRUE(l.insertAfterValue("c", "d"));
    EXPECT_FALSE(l.insertAfterValue("zzz", "q"));
    l.push_back("e");
    EXPECT_TRUE((l.get() == vector<string>{"a", "b", "c", "d", "e"}));
}

TEST(linkList_tests, removeValue) {
    LinkList l;
    EXPECT_FALSE(l.removeValue("a"));
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    l.push_back("d");
    EXPECT_TRUE(l.removeValue("a"));
    EXPECT_TRUE(l.removeValue("c"));
    EXPECT_TRUE(l.removeValue("d"));
    EXPECT_FALSE(l.removeValue("zzz"));
    EXPECT_TRUE((l.get() == vector<string>{"b"}));
    l.push_back("x");
    EXPECT_TRUE((l.get() == vector<string>{"b", "x"}));
}

TEST(linkList_tests, removeBeforeValue) {
    LinkList l;
    EXPECT_FALSE(l.removeBeforeValue("a"));
    l.push_back("a");
    EXPECT_FALSE(l.removeBeforeValue("a"));
    l.push_back("b");
    l.push_back("c");
    l.push_back("d");
    EXPECT_FALSE(l.removeBeforeValue("a"));
    EXPECT_FALSE(l.removeBeforeValue("zzz"));
    EXPECT_TRUE(l.removeBeforeValue("b"));
    EXPECT_TRUE(l.removeBeforeValue("d"));
    EXPECT_TRUE((l.get() == vector<string>{"b", "d"}));
}

TEST(linkList_tests, removeAfterValue) {
    LinkList l;
    EXPECT_FALSE(l.removeAfterValue("a"));
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    l.push_back("d");
    EXPECT_TRUE(l.removeAfterValue("a"));
    EXPECT_TRUE(l.removeAfterValue("c"));
    EXPECT_FALSE(l.removeAfterValue("c"));
    EXPECT_FALSE(l.removeAfterValue("zzz"));
    l.push_back("e");
    EXPECT_TRUE((l.get() == vector<string>{"a", "c", "e"}));
}

TEST(linkList_tests, find) {
    LinkList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    EXPECT_EQ(l.find("a"), 0);
    EXPECT_EQ(l.find("c"), 2);
    EXPECT_EQ(l.find("zzz"), -1);
}

TEST(linkList_tests, getAfterValue_getBeforeValue) {
    LinkList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    l.push_back("d");
    EXPECT_TRUE((l.getAfterValue("b") == vector<string>{"c", "d"}));
    EXPECT_TRUE(l.getAfterValue("d").empty());
    EXPECT_TRUE(l.getAfterValue("zzz").empty());
    EXPECT_TRUE((l.getBeforeValue("c") == vector<string>{"a", "b"}));
    EXPECT_TRUE(l.getBeforeValue("a").empty());
    EXPECT_TRUE(l.getBeforeValue("zzz").empty());
    
    LinkList e;
    EXPECT_TRUE(e.getAfterValue("a").empty());
    EXPECT_TRUE(e.getBeforeValue("a").empty());
}

TEST(linkList_tests, getValue) {
    LinkList l;
    l.push_back("a");
    l.push_back("b");
    string out;
    EXPECT_TRUE(l.getValue("b", out));
    EXPECT_EQ(out, "b");
    EXPECT_FALSE(l.getValue("zzz", out));
}

TEST(linkList_tests, clear) {
    LinkList l;
    l.push_back("a");
    l.push_back("b");
    l.clear();
    EXPECT_TRUE(l.empty());
    l.push_back("c");
    EXPECT_TRUE((l.get() == vector<string>{"c"}));
}

TEST(linkList_tests, copy_constructor) {
    LinkList a;
    a.push_back("a");
    a.push_back("b");
    LinkList b(a);
    a.pop_front();
    EXPECT_TRUE((b.get() == vector<string>{"a", "b"}));
}

TEST(linkList_tests, copy_assignment) {
    LinkList a;
    a.push_back("a");
    LinkList b;
    b.push_back("x");
    b.push_back("y");
    b = a;
    EXPECT_TRUE((b.get() == vector<string>{"a"}));
    b = b;
    EXPECT_TRUE((b.get() == vector<string>{"a"}));
}

TEST(linkList_tests, move_constructor_and_assignment) {
    LinkList a;
    a.push_back("a");
    a.push_back("b");
    LinkList b(std::move(a));
    EXPECT_TRUE((b.get() == vector<string>{"a", "b"}));
    EXPECT_TRUE(a.empty());
    
    LinkList c;
    c.push_back("x");
    c = std::move(b);
    EXPECT_TRUE((c.get() == vector<string>{"a", "b"}));
    EXPECT_TRUE(b.empty());
    
    LinkList& same = c;
    c = std::move(same);
    EXPECT_TRUE((c.get() == vector<string>{"a", "b"}));
}
