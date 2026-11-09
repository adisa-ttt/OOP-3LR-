#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "Stack.h"
using namespace std;

TEST(stack_test, empty) {
    Stack s;
    EXPECT_TRUE(s.empty());
    EXPECT_TRUE(s.get().empty());
}

TEST(stack_test, order) {
    Stack s;
    s.push("1");
    s.push("2");
    s.push("3");
    EXPECT_FALSE(s.empty());
    EXPECT_EQ(s.pop(), "3");
    EXPECT_EQ(s.pop(), "2");
    EXPECT_EQ(s.pop(), "1");
    EXPECT_TRUE(s.empty());
}

TEST(stack_test, pop) {
    Stack s;
    EXPECT_THROW(s.pop(), std::out_of_range);
    s.push("a");
    s.pop();
    EXPECT_THROW(s.pop(), std::out_of_range);
}

TEST(stack_test, get_return_top) {
    Stack s;
    s.push("1");
    s.push("2");
    EXPECT_TRUE((s.get() == vector<string>{"2", "1"}));
}

TEST(stack_test, clear) {
    Stack s;
    s.push("a");
    s.clear();
    EXPECT_TRUE(s.empty());
    s.push("b");
    EXPECT_EQ(s.pop(), "b");
}

TEST(stack_test, copy_constructor) {
    Stack a;
    a.push("1");
    a.push("2");
    Stack b(a);
    EXPECT_TRUE((b.get() == vector<string>{"2", "1"}));
    a.pop();
    EXPECT_TRUE((b.get() == vector<string>{"2", "1"}));
    b.push("3"); 
    EXPECT_TRUE((b.get() == vector<string>{"3", "2", "1"}));
    Stack empty;
    Stack copyOfEmpty(empty);
    EXPECT_TRUE(copyOfEmpty.empty());
}

TEST(stack_test, copy_assignment) {
    Stack a, b;
    a.push("a");
    b.push("x");
    b.push("y");
    b = a;
    EXPECT_TRUE((b.get() == vector<string>{"a"}));
    b = b;
    EXPECT_TRUE((b.get() == vector<string>{"a"}));
}

TEST(stack_test, move_constructor_and_assignment) {
    Stack a;
    a.push("1");
    a.push("2");
    Stack b(std::move(a));
    EXPECT_TRUE((b.get() == vector<string>{"2", "1"}));
    EXPECT_TRUE(a.empty());
    Stack c;
    c.push("x");
    c = std::move(b);
    EXPECT_TRUE((c.get() == vector<string>{"2", "1"}));
    EXPECT_TRUE(b.empty());
    Stack& same = c;
    c = std::move(same);
    EXPECT_TRUE((c.get() == vector<string>{"2", "1"}));
}

