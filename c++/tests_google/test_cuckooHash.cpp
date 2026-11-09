#include <gtest/gtest.h>
#include <string>
#include "CuckooHash.h"

using namespace std;

TEST(cuckooTable_tests, empty) {
    CuckooTable t;
    EXPECT_TRUE(t.empty());
    EXPECT_FALSE(t.CuckooFind(1));
}

TEST(cuckooTable_tests, insert_find_getValue) {
    CuckooTable t;
    EXPECT_TRUE(t.CuckooInsert(1, "one"));
    EXPECT_TRUE(t.CuckooInsert(2, "two"));
    EXPECT_FALSE(t.empty());
    EXPECT_TRUE(t.CuckooFind(1));
    EXPECT_FALSE(t.CuckooFind(3));
    string out;
    EXPECT_TRUE(t.getValue(2, out));
    EXPECT_EQ(out, "two");
    EXPECT_FALSE(t.getValue(3, out));
}

TEST(cuckooTable_tests, insert_existing_key_updates_value) {
    CuckooTable t;
    EXPECT_TRUE(t.CuckooInsert(7, "a"));
    EXPECT_FALSE(t.CuckooInsert(7, "b"));
    string out;
    EXPECT_TRUE(t.getValue(7, out));
    EXPECT_EQ(out, "b");
    EXPECT_TRUE(t.CuckooRemove(7));
    EXPECT_TRUE(t.empty());
}

TEST(cuckooTable_tests, remove) {
    CuckooTable t;
    EXPECT_FALSE(t.CuckooRemove(1));
    t.CuckooInsert(1, "a");
    EXPECT_FALSE(t.CuckooRemove(2));
    EXPECT_TRUE(t.CuckooRemove(1));
    EXPECT_FALSE(t.CuckooFind(1));
    EXPECT_TRUE(t.empty());
}

TEST(cuckooTable_tests, clear) {
    CuckooTable t;
    t.CuckooInsert(1, "a");
    t.clear();
    EXPECT_TRUE(t.empty());
    EXPECT_FALSE(t.CuckooFind(1));
    EXPECT_FALSE(t.CuckooRemove(1));
    string out;
    EXPECT_FALSE(t.getValue(1, out));
}

TEST(cuckooTable_tests, copy_constructor) {
    CuckooTable a;
    for (int i = 0; i < 50; i++) a.CuckooInsert(i, std::to_string(i));
    CuckooTable b(a);
    a.CuckooRemove(10);
    string out;
    EXPECT_TRUE(b.getValue(10, out));
    EXPECT_EQ(out, "10");
    EXPECT_TRUE(b.CuckooFind(49));
    
    CuckooTable empty;
    empty.clear();
    CuckooTable copyOfEmpty(empty);
    EXPECT_TRUE(copyOfEmpty.empty());
}

TEST(cuckooTable_tests, copy_assignment) {
    CuckooTable a, b;
    a.CuckooInsert(1, "a");
    b.CuckooInsert(2, "b");
    b = a;
    EXPECT_TRUE(b.CuckooFind(1));
    EXPECT_FALSE(b.CuckooFind(2));
    b = b;
    EXPECT_TRUE(b.CuckooFind(1));
}

TEST(cuckooTable_tests, move_constructor_and_assignment) {
    CuckooTable a;
    a.CuckooInsert(1, "a");
    CuckooTable b(std::move(a));
    EXPECT_TRUE(b.CuckooFind(1));
    EXPECT_TRUE(a.empty());
    EXPECT_FALSE(a.CuckooFind(1));
    
    CuckooTable c;
    c.CuckooInsert(2, "c");
    c = std::move(b);
    EXPECT_TRUE(c.CuckooFind(1));
    EXPECT_FALSE(c.CuckooFind(2));
    EXPECT_TRUE(b.empty());
    
    CuckooTable& same = c;
    c = std::move(same);
    EXPECT_TRUE(c.CuckooFind(1));
}
