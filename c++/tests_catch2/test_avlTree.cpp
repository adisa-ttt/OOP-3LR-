#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <vector>
#include "avlTree.h"

using namespace std;

TEST_CASE("avlTree: empty") {
    avlTree t;
    CHECK(t.empty());
    CHECK_FALSE(t.find(1));
    CHECK(t.inorder().empty());
    CHECK(t.preorder().empty());
    CHECK(t.postorder().empty());
    CHECK(t.bfs().empty());
}

TEST_CASE("avlTree: insert_and_find") {
    avlTree t;
    t.insert(5);
    t.insert(3);
    t.insert(8);
    CHECK_FALSE(t.empty());
    CHECK(t.find(3));
    CHECK(t.find(5));
    CHECK(t.find(8));
    CHECK_FALSE(t.find(4));
}

TEST_CASE("avlTree: rotation_LL") {
    avlTree t;
    t.insert(3);
    t.insert(2);
    t.insert(1);
    CHECK((t.preorder() == vector<int>{2, 1, 3}));
}

TEST_CASE("avlTree: rotation_RR") {
    avlTree t;
    t.insert(1);
    t.insert(2);
    t.insert(3);
    CHECK((t.preorder() == vector<int>{2, 1, 3}));
}

TEST_CASE("avlTree: rotation_LR") {
    avlTree t;
    t.insert(3);
    t.insert(1);
    t.insert(2);
    CHECK((t.preorder() == vector<int>{2, 1, 3}));
}

TEST_CASE("avlTree: rotation_RL") {
    avlTree t;
    t.insert(1);
    t.insert(3);
    t.insert(2);
    CHECK((t.preorder() == vector<int>{2, 1, 3}));
}

TEST_CASE("avlTree: sequential_insert_gives_perfect_tree") {
    avlTree t;
    for (int i = 1; i <= 7; i++) t.insert(i);
    CHECK((t.preorder() == vector<int>{4, 2, 1, 3, 6, 5, 7}));
    CHECK((t.inorder() == vector<int>{1, 2, 3, 4, 5, 6, 7}));
    CHECK((t.postorder() == vector<int>{1, 3, 2, 5, 7, 6, 4}));
    CHECK((t.bfs() == vector<int>{4, 2, 6, 1, 3, 5, 7}));
}

TEST_CASE("avlTree: remove_leaf") {
    avlTree t;
    for (int i = 1; i <= 7; i++) t.insert(i);
    CHECK(t.remove(1));
    CHECK_FALSE(t.find(1));
    CHECK((t.preorder() == vector<int>{4, 2, 3, 6, 5, 7}));
}

TEST_CASE("avlTree: remove_node_with_left_child") {
    avlTree t;
    t.insert(2);
    t.insert(1);
    t.insert(3);
    t.insert(0);
    CHECK(t.remove(1));
    CHECK((t.inorder() == vector<int>{0, 2, 3}));
}

TEST_CASE("avlTree: remove_node_with_right_child") {
    avlTree t;
    t.insert(2);
    t.insert(1);
    t.insert(3);
    t.insert(4);
    CHECK(t.remove(3));
    CHECK((t.inorder() == vector<int>{1, 2, 4}));
}

TEST_CASE("avlTree: remove_node_with_two_children") {
    avlTree t;
    for (int i = 1; i <= 7; i++) t.insert(i);
    CHECK(t.remove(4));
    CHECK((t.preorder() == vector<int>{5, 2, 1, 3, 6, 7}));
    CHECK(t.remove(6));
    CHECK((t.inorder() == vector<int>{1, 2, 3, 5, 7}));
}

TEST_CASE("avlTree: remove_rebalance_RR") {
    avlTree t;
    t.insert(2);
    t.insert(1);
    t.insert(3);
    t.insert(4);
    CHECK(t.remove(1));
    CHECK((t.preorder() == vector<int>{3, 2, 4}));
}

TEST_CASE("avlTree: remove_rebalance_RL") {
    avlTree t;
    t.insert(2);
    t.insert(1);
    t.insert(4);
    t.insert(3);
    CHECK(t.remove(1));
    CHECK((t.preorder() == vector<int>{3, 2, 4}));
}

TEST_CASE("avlTree: remove_rebalance_LL") {
    avlTree t;
    t.insert(3);
    t.insert(2);
    t.insert(4);
    t.insert(1);
    CHECK(t.remove(4));
    CHECK((t.preorder() == vector<int>{2, 1, 3}));
}

TEST_CASE("avlTree: remove_rebalance_LR") {
    avlTree t;
    t.insert(3);
    t.insert(1);
    t.insert(4);
    t.insert(2);
    CHECK(t.remove(4));
    CHECK((t.preorder() == vector<int>{2, 1, 3}));
}

TEST_CASE("avlTree: clear") {
    avlTree t;
    t.insert(1);
    t.insert(2);
    t.insert(3);
    t.clear();
    CHECK(t.empty());
    t.clear();
    CHECK(t.empty());
}

TEST_CASE("avlTree: copy_constructor") {
    avlTree a;
    a.insert(5);
    a.insert(3);
    a.insert(8);
    a.insert(1);
    avlTree b(a);
    a.remove(3);
    CHECK((b.inorder() == vector<int>{1, 3, 5, 8}));
    
    avlTree empty;
    avlTree copyOfEmpty(empty);
    CHECK(copyOfEmpty.empty());
}

TEST_CASE("avlTree: copy_assignment") {
    avlTree a;
    a.insert(1);
    a.insert(2);
    avlTree b;
    b.insert(7);
    b.insert(8);
    b.insert(9);
    b = a;
    CHECK((b.inorder() == vector<int>{1, 2}));
    b = b;
    CHECK((b.inorder() == vector<int>{1, 2}));
}

TEST_CASE("avlTree: move_constructor_and_assignment") {
    avlTree a;
    a.insert(1);
    a.insert(2);
    a.insert(3);
    avlTree b(std::move(a));
    CHECK((b.inorder() == vector<int>{1, 2, 3}));
    CHECK(a.empty());
    
    avlTree c;
    c.insert(9);
    c = std::move(b);
    CHECK((c.inorder() == vector<int>{1, 2, 3}));
    CHECK(b.empty());
    
    avlTree& same = c;
    c = std::move(same);
    CHECK((c.inorder() == vector<int>{1, 2, 3}));
}

TEST_CASE("avlTree: large_sequential_insert_stays_sorted") {
    avlTree t;
    for (int i = 0; i < 1000; i++) t.insert(i);
    vector<int> in = t.inorder();
    CHECK(in.size() == 1000u);
    CHECK(std::is_sorted(in.begin(), in.end()));
    CHECK(t.bfs().front() == 511);
}