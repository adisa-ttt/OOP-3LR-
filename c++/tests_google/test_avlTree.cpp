#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include "avlTree.h"

using namespace std;

TEST(avlTree_tests, empty) {
    avlTree t;
    /* Убран дублирующийся макрос BOOST_CHECK(t.empty()) / EXPECT_TRUE(t.empty()) */
    EXPECT_TRUE(t.empty());
    EXPECT_FALSE(t.find(1));
    EXPECT_TRUE(t.inorder().empty());
    EXPECT_TRUE(t.preorder().empty());
    EXPECT_TRUE(t.postorder().empty());
    EXPECT_TRUE(t.bfs().empty());
}

TEST(avlTree_tests, insert_and_find) {
    avlTree t;
    t.insert(5);
    t.insert(3);
    t.insert(8);
    EXPECT_FALSE(t.empty());
    EXPECT_TRUE(t.find(3));
    EXPECT_TRUE(t.find(5));
    EXPECT_TRUE(t.find(8));
    EXPECT_FALSE(t.find(4));
}

TEST(avlTree_tests, rotation_LL) {
    avlTree t;
    t.insert(3);
    t.insert(2);
    t.insert(1);
    EXPECT_TRUE((t.inorder() == vector<int>{1, 2, 3}));
}

TEST(avlTree_tests, rotation_RR) {
    avlTree t;
    t.insert(1);
    t.insert(2);
    t.insert(3);
    EXPECT_TRUE((t.inorder() == vector<int>{1, 2, 3}));
}

TEST(avlTree_tests, rotation_LR) {
    avlTree t;
    t.insert(3);
    t.insert(1);
    t.insert(2);
    EXPECT_TRUE((t.inorder() == vector<int>{1, 2, 3}));
}

TEST(avlTree_tests, rotation_RL) {
    avlTree t;
    t.insert(1);
    t.insert(3);
    t.insert(2);
    EXPECT_TRUE((t.inorder() == vector<int>{1, 2, 3}));
}

TEST(avlTree_tests, sequential_insert_gives_perfect_tree) {
    avlTree t;
    for (int i = 1; i <= 7; i++) t.insert(i);
    EXPECT_TRUE((t.preorder() == vector<int>{4, 2, 1, 3, 6, 5, 7}));
    EXPECT_TRUE((t.inorder() == vector<int>{1, 2, 3, 4, 5, 6, 7}));
    EXPECT_TRUE((t.postorder() == vector<int>{1, 3, 2, 5, 7, 6, 4}));
    EXPECT_TRUE((t.bfs() == vector<int>{4, 2, 6, 1, 3, 5, 7}));
}

TEST(avlTree_tests, remove_leaf) {
    avlTree t;
    for (int i = 1; i <= 7; i++) t.insert(i);
    EXPECT_TRUE(t.remove(1));
    EXPECT_FALSE(t.find(1));
    EXPECT_TRUE((t.preorder() == vector<int>{4, 2, 3, 6, 5, 7}));
}

TEST(avlTree_tests, remove_node_with_left_child) {
    avlTree t;
    t.insert(2);
    t.insert(1);
    t.insert(3);
    t.insert(0);
    EXPECT_TRUE(t.remove(1));
    EXPECT_TRUE((t.inorder() == vector<int>{0, 2, 3}));
}

TEST(avlTree_tests, remove_node_with_right_child) {
    avlTree t;
    t.insert(2);
    t.insert(1);
    t.insert(3);
    t.insert(4);
    EXPECT_TRUE(t.remove(3));
    EXPECT_TRUE((t.inorder() == vector<int>{1, 2, 4}));
}

TEST(avlTree_tests, remove_node_with_two_children) {
    avlTree t;
    for (int i = 1; i <= 7; i++) t.insert(i);
    EXPECT_TRUE(t.remove(4));
    EXPECT_TRUE((t.preorder() == vector<int>{5, 2, 1, 3, 6, 7}));
    EXPECT_TRUE(t.remove(6));
    EXPECT_TRUE((t.inorder() == vector<int>{1, 2, 3, 5, 7}));
}

TEST(avlTree_tests, remove_rebalance_RR) {
    avlTree t;
    t.insert(2);
    t.insert(1);
    t.insert(3);
    t.insert(4);
    EXPECT_TRUE(t.remove(1));
    EXPECT_TRUE((t.preorder() == vector<int>{3, 2, 4}));
}

TEST(avlTree_tests, remove_rebalance_RL) {
    avlTree t;
    t.insert(2);
    t.insert(1);
    t.insert(4);
    t.insert(3);
    EXPECT_TRUE(t.remove(1));
    EXPECT_TRUE((t.preorder() == vector<int>{3, 2, 4}));
}

TEST(avlTree_tests, remove_rebalance_LL) {
    avlTree t;
    t.insert(3);
    t.insert(2);
    t.insert(4);
    t.insert(1);
    EXPECT_TRUE(t.remove(4));
    EXPECT_TRUE((t.inorder() == vector<int>{1, 2, 3}));
}

TEST(avlTree_tests, remove_rebalance_LR) {
    avlTree t;
    t.insert(3);
    t.insert(1);
    t.insert(4);
    t.insert(2);
    EXPECT_TRUE(t.remove(4));
    EXPECT_TRUE((t.inorder() == vector<int>{1, 2, 3}));
}

TEST(avlTree_tests, clear) {
    avlTree t;
    t.insert(1);
    t.insert(2);
    t.insert(3);
    t.clear();
    EXPECT_TRUE(t.empty());
    t.clear();
    EXPECT_TRUE(t.empty());
}

TEST(avlTree_tests, copy_constructor) {
    avlTree a;
    a.insert(5);
    a.insert(3);
    a.insert(8);
    a.insert(1);
    avlTree b(a);
    a.remove(3);
    EXPECT_TRUE((b.inorder() == vector<int>{1, 3, 5, 8}));
    
    avlTree empty;
    avlTree copyOfEmpty(empty);
    EXPECT_TRUE(copyOfEmpty.empty());
}

TEST(avlTree_tests, copy_assignment) {
    avlTree a;
    a.insert(1);
    a.insert(2);
    avlTree b;
    b.insert(7);
    b.insert(8);
    b.insert(9);
    b = a;
    EXPECT_TRUE((b.inorder() == vector<int>{1, 2}));
    b = b;
    EXPECT_TRUE((b.inorder() == vector<int>{1, 2}));
}

TEST(avlTree_tests, move_constructor_and_assignment) {
    avlTree a;
    a.insert(1);
    a.insert(2);
    a.insert(3);
    avlTree b(std::move(a));
    EXPECT_TRUE((b.inorder() == vector<int>{1, 2, 3}));
    EXPECT_TRUE(a.empty());
    
    avlTree c;
    c.insert(9);
    c = std::move(b);
    EXPECT_TRUE((c.inorder() == vector<int>{1, 2, 3}));
    EXPECT_TRUE(b.empty());
    
    avlTree& same = c;
    c = std::move(same);
    EXPECT_TRUE((c.inorder() == vector<int>{1, 2, 3}));
}

TEST(avlTree_tests, large_sequential_insert_stays_sorted) {
    avlTree t;
    for (int i = 0; i < 1000; i++) t.insert(i);
    vector<int> in = t.inorder();
    EXPECT_EQ(in.size(), 1000u);
    EXPECT_TRUE(std::is_sorted(in.begin(), in.end()));
    EXPECT_EQ(t.bfs().front(), 511);
}
