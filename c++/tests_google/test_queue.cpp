#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "Queue.h"
using namespace std;

TEST(queue_tests, empty) {
    Queue q;
    EXPECT_TRUE(q.empty());
    EXPECT_TRUE(q.get().empty());
}

TEST(queue_tests, order) {
    Queue q;
    q.push("1");
    q.push("2");
    q.push("3");
    EXPECT_FALSE(q.empty());
    EXPECT_EQ(q.pop(), "1");
    EXPECT_EQ(q.pop(), "2");
    EXPECT_EQ(q.pop(), "3");
    EXPECT_TRUE(q.empty());
}

TEST(queue_tests, pop) {
    Queue q;
    EXPECT_THROW(q.pop(), std::out_of_range);
    q.push("a");
    q.pop();
    EXPECT_THROW(q.pop(), std::out_of_range);
}

TEST(queue_tests, get_returns_head) {
    Queue q;
    q.push("1");
    q.push("2");
    EXPECT_TRUE((q.get() == vector<string>{"1", "2"}));
}

TEST(queue_tests, clear) {
    Queue q;
    q.push("a");
    q.clear();
    EXPECT_TRUE(q.empty());
    q.push("b");
    EXPECT_EQ(q.pop(), "b");
}

TEST(queue_tests, copy_constructor) {
    Queue a;
    a.push("1");
    a.push("2");
    Queue b(a);
    EXPECT_TRUE((b.get() == vector<string>{"1", "2"}));
    a.pop();
    EXPECT_TRUE((b.get() == vector<string>{"1", "2"}));
    b.push("3"); 
    EXPECT_TRUE((b.get() == vector<string>{"1", "2", "3"}));
    Queue empty;
    Queue copyOfEmpty(empty);
    EXPECT_TRUE(copyOfEmpty.empty());
}

TEST(queue_tests, copy_assignment) {
    Queue a, b;
    a.push("1");
    b.push("x");
    b.push("y");
    b = a;
    EXPECT_TRUE((b.get() == vector<string>{"1"}));
    b = b;
    EXPECT_TRUE((b.get() == vector<string>{"1"}));
}

TEST(queue_tests, move_constructor_and_assignment) {
    Queue a;
    a.push("1");
    a.push("2");
    Queue b(std::move(a));
    EXPECT_TRUE((b.get() == vector<string>{"1", "2"}));
    EXPECT_TRUE(a.empty());
    Queue c;
    c.push("x");
    c = std::move(b);
    EXPECT_TRUE((c.get() == vector<string>{"1", "2"}));
    EXPECT_TRUE(b.empty());
    Queue& same = c;
    c = std::move(same);
    EXPECT_TRUE((c.get() == vector<string>{"1", "2"}));
}
