#include <gtest/gtest.h>
#include <string>
#include "ChainHash.h"

using namespace std;

TEST(chainTable_tests, empty) {
    ChainTable t;
    EXPECT_TRUE(t.empty());
    EXPECT_FALSE(t.chainFind(1));
}

TEST(chainTable_tests, insert_find_getValue) {
    ChainTable t;
    EXPECT_TRUE(t.ChainInsert(1, "one"));
    EXPECT_TRUE(t.ChainInsert(-2, "-two"));
    EXPECT_FALSE(t.empty());
    EXPECT_TRUE(t.chainFind(1));
    EXPECT_FALSE(t.chainFind(3));
    string out;
    EXPECT_TRUE(t.getValue(-2, out));
    EXPECT_EQ(out, "-two");
    EXPECT_FALSE(t.getValue(3, out));
}

TEST(chainTable_tests, insert_existing_key_updates_value) {
    ChainTable t;
    EXPECT_TRUE(t.ChainInsert(1, "a"));
    EXPECT_FALSE(t.ChainInsert(1, "b"));
    string out;
    EXPECT_TRUE(t.getValue(1, out));
    EXPECT_EQ(out, "b");
}

TEST(chainTable_tests, collisions_in_one_chain) {
    ChainTable t(8);
    EXPECT_TRUE(t.ChainInsert(1, "a"));
    EXPECT_TRUE(t.ChainInsert(9, "b"));
    EXPECT_TRUE(t.ChainInsert(17, "c"));
    EXPECT_TRUE(t.chainFind(1));
    EXPECT_TRUE(t.chainFind(9));
    EXPECT_TRUE(t.chainFind(17));
    EXPECT_TRUE(t.chainRemove(9));
    EXPECT_TRUE(t.chainRemove(17));
    EXPECT_TRUE(t.chainRemove(1));
    EXPECT_TRUE(t.empty());
}

TEST(chainTable_tests, remove) {
    ChainTable t;
    EXPECT_FALSE(t.chainRemove(1));
    t.ChainInsert(1, "a");
    EXPECT_FALSE(t.chainRemove(2));
    EXPECT_TRUE(t.chainRemove(1));
    EXPECT_FALSE(t.chainRemove(1));
}

TEST(chainTable_tests, clear) {
    ChainTable t;
    t.ChainInsert(1, "a");
    t.clear();
    EXPECT_TRUE(t.empty());
    EXPECT_FALSE(t.chainFind(1));
    EXPECT_FALSE(t.chainRemove(1));
    string out;
    EXPECT_FALSE(t.getValue(1, out));
    EXPECT_FALSE(t.ChainInsert(2, "b"));
}

TEST(chainTable_tests, copy_constructor) {
    ChainTable a;
    for (int i = 0; i < 20; i++) a.ChainInsert(i, std::to_string(i));
    ChainTable b(a);
    a.chainRemove(5);
    string out;
    EXPECT_TRUE(b.getValue(5, out));
    EXPECT_EQ(out, "5");
    EXPECT_TRUE(b.chainFind(19));
}

TEST(chainTable_tests, copy_assignment) {
    ChainTable a, b;
    a.ChainInsert(1, "a");
    b.ChainInsert(2, "b");
    b = a;
    EXPECT_TRUE(b.chainFind(1));
    EXPECT_FALSE(b.chainFind(2));
    b = b;
    EXPECT_TRUE(b.chainFind(1));
}

TEST(chainTable_tests, move_constructor_and_assignment) {
    ChainTable a;
    a.ChainInsert(1, "a");
    ChainTable b(std::move(a));
    EXPECT_TRUE(b.chainFind(1));
    EXPECT_TRUE(a.empty());
    EXPECT_FALSE(a.chainFind(1));
    
    ChainTable c;
    c.ChainInsert(2, "c");
    c = std::move(b);
    EXPECT_TRUE(c.chainFind(1));
    EXPECT_FALSE(c.chainFind(2));
    EXPECT_TRUE(b.empty());
    
    ChainTable& same = c;
    c = std::move(same);
    EXPECT_TRUE(c.chainFind(1));
}
