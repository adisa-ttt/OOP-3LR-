#include <catch2/catch_test_macros.hpp>
#include <string>
#include "ChainHash.h"

using namespace std;

TEST_CASE("chainTable: empty") {
    ChainTable t;
    CHECK(t.empty());
    CHECK_FALSE(t.chainFind(1));
}

TEST_CASE("chainTable: insert_find_getValue") {
    ChainTable t;
    CHECK(t.ChainInsert(1, "one"));
    CHECK(t.ChainInsert(-2, "-two"));
    CHECK_FALSE(t.empty());
    CHECK(t.chainFind(1));
    CHECK_FALSE(t.chainFind(3));
    string out;
    CHECK(t.getValue(-2, out));
    CHECK(out == "-two");
    CHECK_FALSE(t.getValue(3, out));
}

TEST_CASE("chainTable: insert_existing_key_updates_value") {
    ChainTable t;
    CHECK(t.ChainInsert(1, "a"));
    CHECK_FALSE(t.ChainInsert(1, "b"));
    string out;
    CHECK(t.getValue(1, out));
    CHECK(out == "b");
}

TEST_CASE("chainTable: collisions_in_one_chain") {
    ChainTable t(8);
    CHECK(t.ChainInsert(1, "a"));
    CHECK(t.ChainInsert(9, "b"));
    CHECK(t.ChainInsert(17, "c"));
    CHECK(t.chainFind(1));
    CHECK(t.chainFind(9));
    CHECK(t.chainFind(17));
    CHECK(t.chainRemove(9));
    CHECK(t.chainRemove(17));
    CHECK(t.chainRemove(1));
    CHECK(t.empty());
}

TEST_CASE("chainTable: remove") {
    ChainTable t;
    CHECK_FALSE(t.chainRemove(1));
    t.ChainInsert(1, "a");
    CHECK_FALSE(t.chainRemove(2));
    CHECK(t.chainRemove(1));
    CHECK_FALSE(t.chainRemove(1));
}

TEST_CASE("chainTable: clear") {
    ChainTable t;
    t.ChainInsert(1, "a");
    t.clear();
    CHECK(t.empty());
    CHECK_FALSE(t.chainFind(1));
    CHECK_FALSE(t.chainRemove(1));
    string out;
    CHECK_FALSE(t.getValue(1, out));
    CHECK_FALSE(t.ChainInsert(2, "b"));
}

TEST_CASE("chainTable: copy_constructor") {
    ChainTable a;
    for (int i = 0; i < 20; i++) a.ChainInsert(i, std::to_string(i));
    ChainTable b(a);
    a.chainRemove(5);
    string out;
    CHECK(b.getValue(5, out));
    CHECK(out == "5");
    CHECK(b.chainFind(19));
}

TEST_CASE("chainTable: copy_assignment") {
    ChainTable a, b;
    a.ChainInsert(1, "a");
    b.ChainInsert(2, "b");
    b = a;
    CHECK(b.chainFind(1));
    CHECK_FALSE(b.chainFind(2));
    b = b;
    CHECK(b.chainFind(1));
}

TEST_CASE("chainTable: move_constructor_and_assignment") {
    ChainTable a;
    a.ChainInsert(1, "a");
    ChainTable b(std::move(a));
    CHECK(b.chainFind(1));
    CHECK(a.empty());
    CHECK_FALSE(a.chainFind(1));
    
    ChainTable c;
    c.ChainInsert(2, "c");
    c = std::move(b);
    CHECK(c.chainFind(1));
    CHECK_FALSE(c.chainFind(2));
    CHECK(b.empty());
    
    ChainTable& same = c;
    c = std::move(same);
    CHECK(c.chainFind(1));
}