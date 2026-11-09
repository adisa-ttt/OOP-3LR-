#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "Array.h"
using namespace std;

TEST(array_test, empty) {
    Array a;
    EXPECT_TRUE(a.empty());
    EXPECT_EQ(a.getSize(), 0);
    EXPECT_TRUE(a.get().empty());
}

TEST(array_test, check_capacity) {
    Array a;
    for (int i = 0; i < 100; i++) {
        a.push_back(to_string(i)); }
    EXPECT_EQ(a.getSize(), 100);
    for (int i = 0; i < 100;i++) {
        EXPECT_EQ(a.getAt(i), to_string(i)); }
}

TEST(array_test, push_back_and_getAt) {
    Array a;
    a.push_back("adisa!");
    a.push_back("kate!");
    a.push_back("diana!");
    EXPECT_FALSE(a.empty());
    EXPECT_EQ(a.getSize(), 3);
    EXPECT_EQ(a.getAt(0), "adisa!");
    EXPECT_EQ(a.getAt(2), "diana!");
}

TEST(array_test, getAt_out_of_range) {
    Array a;
    EXPECT_THROW(a.getAt(0), out_of_range);
    a.push_back("First");
    EXPECT_THROW(a.getAt(-1), out_of_range);
    EXPECT_THROW(a.getAt(1), out_of_range);
}

TEST(array_test, insert_into) {
    Array a;
    a.push_back("b");
    a.push_back("d");
    EXPECT_TRUE(a.insert(0, "a"));
    EXPECT_TRUE(a.insert(2, "c"));
    EXPECT_TRUE(a.insert(4, "e"));
    EXPECT_TRUE((a.get() == vector<string>{"a", "b", "c", "d", "e"}));
}

TEST(array_test, insert_with_bad_index) {
    Array a;
    a.push_back("a");
    EXPECT_FALSE(a.insert(-1, "b"));
    EXPECT_TRUE(a.insert(100, "c"));
    EXPECT_TRUE((a.get() == vector<string>{"a", "c"}));
}

TEST(array_test, insert_and_resize) {
    Array a;
    for (int i = 0; i < 4; i++) a.push_back("old");
    EXPECT_TRUE(a.insert(1, "new"));
    EXPECT_EQ(a.getAt(1), "new");
    EXPECT_EQ(a.getSize(), 5);    
}

TEST(array_test, removeAt) {
    Array a;
    for (string s : {"a", "b", "c"}) a.push_back(s);
    EXPECT_FALSE(a.removeAt(-1));
    EXPECT_FALSE(a.removeAt(3));
    EXPECT_TRUE(a.removeAt(1));
    EXPECT_TRUE((a.get() == vector<string>{"a", "c"}));
    EXPECT_TRUE(a.removeAt(1));
    EXPECT_TRUE(a.removeAt(0));
    EXPECT_TRUE(a.empty());
    EXPECT_FALSE(a.removeAt(0));
}

TEST(array_test, set) {
    Array a;
    a.push_back("a");
    EXPECT_TRUE(a.set(0, "z"));
    EXPECT_EQ(a.getAt(0), "z");
    EXPECT_FALSE(a.set(1, "q"));
    EXPECT_FALSE(a.set(-1, "q"));
}

TEST(array_test, clear) {
    Array a;
    a.push_back("a");
    a.clear();
    EXPECT_TRUE(a.empty());
    EXPECT_EQ(a.getSize(), 0);
    a.push_back("b");
    EXPECT_EQ(a.getAt(0), "b");
    a.clear();
    a.clear();
    EXPECT_TRUE(a.empty());
}

TEST(array_test, copy_of_empty) {
    Array a;
    Array b(a);
    EXPECT_TRUE(b.empty());
    b.push_back("x");
    EXPECT_EQ(b.getSize(), 1);
}

TEST(array_test, copy_constructor) {
    Array a;
    a.push_back("a");
    a.push_back("b");
    Array b(a);
    EXPECT_EQ(b.getAt(0), "a");
    EXPECT_EQ(b.getSize(), 2);
}

TEST(array_test, copy_assignment) {
    Array a, b;
    a.push_back("a");
    b.push_back("x");
    b.push_back("y");
    b = a;
    EXPECT_TRUE((b.get() == vector<string>{"a"}));
    b = b;
    EXPECT_TRUE((b.get() == vector<string>{"a"}));
}

TEST(array_test, move_constructor) {
    Array a;
    a.push_back("a");
    a.push_back("b");
    Array b(std::move(a));
    EXPECT_EQ(b.getSize(), 2);
    EXPECT_TRUE(a.empty());
}

TEST(array_test, move_assignment) {
    Array a, b;
    a.push_back("a");
    b.push_back("x");
    b = std::move(a);
    EXPECT_TRUE((b.get() == vector<string>{"a"}));
    EXPECT_TRUE(a.empty());
    Array& same = b;
    b = std::move(same);  
    EXPECT_TRUE((b.get() == vector<string>{"a"}));
}
