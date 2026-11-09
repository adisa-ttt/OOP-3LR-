#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <algorithm>
#include <vector>
#include "avlTree.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(avlTree_tests)

BOOST_AUTO_TEST_CASE(empty) {
    avlTree t;
    BOOST_CHECK(t.empty());
    BOOST_CHECK(!t.find(1));
    BOOST_CHECK(t.inorder().empty());
    BOOST_CHECK(t.bfs().empty());
    BOOST_CHECK(t.postorder().empty());
    BOOST_CHECK(t.bfs().empty());
}

BOOST_AUTO_TEST_CASE(insert_and_find) {
    avlTree t;
    t.insert(5);
    t.insert(3);
    t.insert(8);
    BOOST_CHECK(!t.empty());
    BOOST_CHECK(t.find(3));
    BOOST_CHECK(t.find(5));
    BOOST_CHECK(t.find(8));
    BOOST_CHECK(!t.find(4));
}

BOOST_AUTO_TEST_CASE(rotation_LL) {
    avlTree t;
    t.insert(3);
    t.insert(2);
    t.insert(1);
    BOOST_CHECK((t.bfs() == vector<int>{2, 1, 3}));
}

BOOST_AUTO_TEST_CASE(rotation_RR) {
    avlTree t;
    t.insert(1);
    t.insert(2);
    t.insert(3);
    BOOST_CHECK((t.bfs() == vector<int>{2, 1, 3}));
}

BOOST_AUTO_TEST_CASE(rotation_LR) {
    avlTree t;
    t.insert(3);
    t.insert(1);
    t.insert(2);
    BOOST_CHECK((t.bfs() == vector<int>{2, 1, 3}));
}

BOOST_AUTO_TEST_CASE(rotation_RL) {
    avlTree t;
    t.insert(1);
    t.insert(3);
    t.insert(2);
    BOOST_CHECK((t.bfs() == vector<int>{2, 1, 3}));
}

BOOST_AUTO_TEST_CASE(sequential_insert_gives_perfect_tree) {
    avlTree t;
    for (int i = 1; i <= 7; i++) t.insert(i);
    BOOST_CHECK((t.preorder() == vector<int>{4, 2, 1, 3, 6, 5, 7}));
    BOOST_CHECK((t.inorder() == vector<int>{1, 2, 3, 4, 5, 6, 7}));
    BOOST_CHECK((t.postorder() == vector<int>{1, 3, 2, 5, 7, 6, 4}));
    BOOST_CHECK((t.bfs() == vector<int>{4, 2, 6, 1, 3, 5, 7}));
}

BOOST_AUTO_TEST_CASE(remove_leaf) {
    avlTree t;
    for (int i = 1; i <= 7; i++) t.insert(i);
    BOOST_CHECK(t.remove(1));
    BOOST_CHECK(!t.find(1));
    BOOST_CHECK((t.preorder() == vector<int>{4, 2, 3, 6, 5, 7}));
}

BOOST_AUTO_TEST_CASE(remove_node_with_left_child) {
    avlTree t;
    t.insert(2);
    t.insert(1);
    t.insert(3);
    t.insert(0);
    BOOST_CHECK(t.remove(1));
    BOOST_CHECK((t.inorder() == vector<int>{0, 2, 3}));
}

BOOST_AUTO_TEST_CASE(remove_node_with_right_child) {
    avlTree t;
    t.insert(2);
    t.insert(1);
    t.insert(3);
    t.insert(4);
    BOOST_CHECK(t.remove(3));
    BOOST_CHECK((t.inorder() == vector<int>{1, 2, 4}));
}

BOOST_AUTO_TEST_CASE(remove_node_with_two_children) {
    avlTree t;
    for (int i = 1; i <= 7; i++) t.insert(i);
    BOOST_CHECK(t.remove(4));
    BOOST_CHECK((t.preorder() == vector<int>{5, 2, 1, 3, 6, 7}));
    BOOST_CHECK(t.remove(6));
    BOOST_CHECK((t.inorder() == vector<int>{1, 2, 3, 5, 7}));
}

BOOST_AUTO_TEST_CASE(remove_rebalance_RR) {
    avlTree t;
    t.insert(2);
    t.insert(1);
    t.insert(3);
    t.insert(4);
    BOOST_CHECK(t.remove(1));
    BOOST_CHECK((t.bfs() == vector<int>{3, 2, 4}));
}

BOOST_AUTO_TEST_CASE(remove_rebalance_RL) {
    avlTree t;
    t.insert(2);
    t.insert(1);
    t.insert(4);
    t.insert(3);
    BOOST_CHECK(t.remove(1));
    BOOST_CHECK((t.bfs() == vector<int>{3, 2, 4}));
}

BOOST_AUTO_TEST_CASE(remove_rebalance_LL) {
    avlTree t;
    t.insert(3);
    t.insert(2);
    t.insert(4);
    t.insert(1);
    BOOST_CHECK(t.remove(4));
    BOOST_CHECK((t.bfs() == vector<int>{2, 1, 3}));
}

BOOST_AUTO_TEST_CASE(remove_rebalance_LR) {
    avlTree t;
    t.insert(3);
    t.insert(1);
    t.insert(4);
    t.insert(2);
    BOOST_CHECK(t.remove(4));
    BOOST_CHECK((t.bfs() == vector<int>{2, 1, 3}));
}

BOOST_AUTO_TEST_CASE(clear) {
    avlTree t;
    t.insert(1);
    t.insert(2);
    t.insert(3);
    t.clear();
    BOOST_CHECK(t.empty());
    t.clear();
    BOOST_CHECK(t.empty());
}

BOOST_AUTO_TEST_CASE(copy_constructor) {
    avlTree a;
    a.insert(5);
    a.insert(3);
    a.insert(8);
    a.insert(1);
    avlTree b(a);
    a.remove(3);
    BOOST_CHECK((b.inorder() == vector<int>{1, 3, 5, 8}));
    
    avlTree empty;
    avlTree copyOfEmpty(empty);
    BOOST_CHECK(copyOfEmpty.empty());
}

BOOST_AUTO_TEST_CASE(copy_assignment) {
    avlTree a;
    a.insert(1);
    a.insert(2);
    avlTree b;
    b.insert(7);
    b.insert(8);
    b.insert(9);
    b = a;
    BOOST_CHECK((b.inorder() == vector<int>{1, 2}));
    b = b;
    BOOST_CHECK((b.inorder() == vector<int>{1, 2}));
}

BOOST_AUTO_TEST_CASE(move_constructor_and_assignment) {
    avlTree a;
    a.insert(1);
    a.insert(2);
    a.insert(3);
    avlTree b(std::move(a));
    BOOST_CHECK((b.inorder() == vector<int>{1, 2, 3}));
    BOOST_CHECK(a.empty());
    
    avlTree c;
    c.insert(9);
    c = std::move(b);
    BOOST_CHECK((c.inorder() == vector<int>{1, 2, 3}));
    BOOST_CHECK(b.empty());
    
    avlTree& same = c;
    c = std::move(same);
    BOOST_CHECK((c.inorder() == vector<int>{1, 2, 3}));
}

BOOST_AUTO_TEST_CASE(large_sequential_insert_stays_sorted) {
    avlTree t;
    for (int i = 0; i < 1000; i++) t.insert(i);
    vector<int> in = t.inorder();
    BOOST_CHECK_EQUAL(in.size(), 1000u);
    BOOST_CHECK(std::is_sorted(in.begin(), in.end()));
    BOOST_CHECK_EQUAL(t.bfs().front(), 511);
}

BOOST_AUTO_TEST_SUITE_END()