#include <gtest/gtest.h>
#include <algorithm>
#include <string>
#include <vector>
#include "DoubleLinkedList.h"

using namespace std;

TEST(doubleLinkList_tests, empty) {
    doubleLinkList l;
    EXPECT_TRUE(l.empty());
    EXPECT_TRUE(l.get().empty());
    EXPECT_TRUE(l.getReverse().empty());
}

TEST(doubleLinkList_tests, push_front_and_push_back) {
    doubleLinkList l;
    l.push_back("b");
    l.push_front("a");
    l.push_back("c");
    EXPECT_TRUE((l.get() == vector<string>{"a", "b", "c"}));
    EXPECT_TRUE((l.getReverse() == vector<string>{"c", "b", "a"}));
}

TEST(doubleLinkList_tests, pop_front_and_pop_back) {
    doubleLinkList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    EXPECT_EQ(l.pop_front(), "a");
    EXPECT_EQ(l.pop_back(), "c");
    
    vector<string> fwd = l.get();
    vector<string> back = l.getReverse();
    reverse(back.begin(), back.end());
    EXPECT_TRUE(fwd == back);
    
    EXPECT_EQ(l.pop_back(), "b");
    EXPECT_TRUE(l.empty());
    l.push_front("x");
    EXPECT_TRUE((l.get() == vector<string>{"x"}));
}

TEST(doubleLinkList_tests, getAt) {
    doubleLinkList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    EXPECT_EQ(l.getAt(0), "a");
    EXPECT_EQ(l.getAt(2), "c");
    EXPECT_THROW(l.getAt(-1), std::out_of_range);
    EXPECT_THROW(l.getAt(3), std::out_of_range);
    
    doubleLinkList e;
    EXPECT_THROW(e.getAt(0), std::out_of_range);
}

TEST(doubleLinkList_tests, insertBeforeValue) {
    doubleLinkList l;
    EXPECT_FALSE(l.insertBeforeValue("x", "y"));
    l.push_back("b");
    l.push_back("d");
    EXPECT_TRUE(l.insertBeforeValue("b", "a"));
    EXPECT_TRUE(l.insertBeforeValue("d", "c"));
    EXPECT_FALSE(l.insertBeforeValue("zzz", "q"));
    EXPECT_TRUE((l.get() == vector<string>{"a", "b", "c", "d"}));
    
    vector<string> fwd = l.get();
    vector<string> back = l.getReverse();
    reverse(back.begin(), back.end());
    EXPECT_TRUE(fwd == back);
}

TEST(doubleLinkList_tests, insertAfterValue) {
    doubleLinkList l;
    EXPECT_FALSE(l.insertAfterValue("x", "y"));
    l.push_back("a");
    l.push_back("c");
    EXPECT_TRUE(l.insertAfterValue("a", "b"));
    EXPECT_TRUE(l.insertAfterValue("c", "d"));
    EXPECT_FALSE(l.insertAfterValue("zzz", "q"));
    EXPECT_TRUE((l.get() == vector<string>{"a", "b", "c", "d"}));
    
    vector<string> fwd = l.get();
    vector<string> back = l.getReverse();
    reverse(back.begin(), back.end());
    EXPECT_TRUE(fwd == back);
}

TEST(doubleLinkList_tests, removeValue) {
    doubleLinkList l;
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
    
    vector<string> fwd = l.get();
    vector<string> back = l.getReverse();
    reverse(back.begin(), back.end());
    EXPECT_TRUE(fwd == back);
}

TEST(doubleLinkList_tests, removeBeforeValue) {
    doubleLinkList l;
    EXPECT_FALSE(l.removeBeforeValue("a"));
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    l.push_back("d");
    EXPECT_FALSE(l.removeBeforeValue("a"));
    EXPECT_FALSE(l.removeBeforeValue("zzz"));
    EXPECT_TRUE(l.removeBeforeValue("b"));
    EXPECT_TRUE(l.removeBeforeValue("d"));
    EXPECT_TRUE((l.get() == vector<string>{"b", "d"}));
    
    vector<string> fwd = l.get();
    vector<string> back = l.getReverse();
    reverse(back.begin(), back.end());
    EXPECT_TRUE(fwd == back);
}

TEST(doubleLinkList_tests, removeAfterValue) {
    doubleLinkList l;
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
    
    vector<string> fwd = l.get();
    vector<string> back = l.getReverse();
    reverse(back.begin(), back.end());
    EXPECT_TRUE(fwd == back);
}

TEST(doubleLinkList_tests, find_and_getValue) {
    doubleLinkList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    EXPECT_EQ(l.find("b"), 1);
    EXPECT_EQ(l.find("zzz"), -1);
    string out;
    EXPECT_TRUE(l.getValue("c", out));
    EXPECT_EQ(out, "c");
    EXPECT_FALSE(l.getValue("zzz", out));
}

TEST(doubleLinkList_tests, getAfterValue_getBeforeValue) {
    doubleLinkList l;
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
}

TEST(doubleLinkList_tests, clear_copy_move) {
    doubleLinkList a;
    a.push_back("a");
    a.push_back("b");
    doubleLinkList b(a);
    a.clear();
    EXPECT_TRUE(a.empty());
    EXPECT_TRUE((b.get() == vector<string>{"a", "b"}));
    
    doubleLinkList c;
    c.push_back("x");
    c = b;
    EXPECT_TRUE((c.get() == vector<string>{"a", "b"}));
    c = c;
    EXPECT_TRUE((c.get() == vector<string>{"a", "b"}));
    
    doubleLinkList d(std::move(c));
    EXPECT_TRUE((d.get() == vector<string>{"a", "b"}));
    EXPECT_TRUE(c.empty());
    
    doubleLinkList e;
    e.push_back("q");
    e = std::move(d);
    EXPECT_TRUE((e.get() == vector<string>{"a", "b"}));
    EXPECT_TRUE(d.empty());
    
    doubleLinkList& same = e;
    e = std::move(same);
    EXPECT_TRUE((e.get() == vector<string>{"a", "b"}));
    
    vector<string> fwd = e.get();
    vector<string> back = e.getReverse();
    reverse(back.begin(), back.end());
    EXPECT_TRUE(fwd == back);
}
