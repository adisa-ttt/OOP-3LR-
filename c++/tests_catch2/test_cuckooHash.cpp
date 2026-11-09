#include <catch2/catch_test_macros.hpp>
#include <string>
#include "CuckooHash.h"

using namespace std;

TEST_CASE("cuckooTable: empty") {
    CuckooTable t;
    CHECK(t.empty());
    CHECK_FALSE(t.CuckooFind(1));
}

TEST_CASE("cuckooTable: insert_find_getValue") {
    CuckooTable t;
    CHECK(t.CuckooInsert(1, "one"));
    CHECK(t.CuckooInsert(2, "two"));
    CHECK_FALSE(t.empty());
    CHECK(t.CuckooFind(1));
    CHECK_FALSE(t.CuckooFind(3));
    string out;
    CHECK(t.getValue(2, out));
    CHECK(out == "two");
    CHECK_FALSE(t.getValue(3, out));
}

TEST_CASE("cuckooTable: insert_existing_key_updates_value") {
    CuckooTable t;
    CHECK(t.CuckooInsert(7, "a"));
    CHECK_FALSE(t.CuckooInsert(7, "b"));
    string out;
    CHECK(t.getValue(7, out));
    CHECK(out == "b");
    CHECK(t.CuckooRemove(7));
    CHECK(t.empty());
}

TEST_CASE("cuckooTable: remove") {
    CuckooTable t;
    CHECK_FALSE(t.CuckooRemove(1));
    t.CuckooInsert(1, "a");
    CHECK_FALSE(t.CuckooRemove(2));
    CHECK(t.CuckooRemove(1));
    CHECK_FALSE(t.CuckooFind(1));
    CHECK(t.empty());
}

TEST_CASE("cuckooTable: clear") {
    CuckooTable t;
    t.CuckooInsert(1, "a");
    t.clear();
    CHECK(t.empty());
    CHECK_FALSE(t.CuckooFind(1));
    CHECK_FALSE(t.CuckooRemove(1));
    string out;
    CHECK_FALSE(t.getValue(1, out));
}

TEST_CASE("cuckooTable: copy_constructor") {
    CuckooTable a;
    for (int i = 0; i < 50; i++) a.CuckooInsert(i, std::to_string(i));
    CuckooTable b(a);
    a.CuckooRemove(10);
    string out;
    CHECK(b.getValue(10, out));
    CHECK(out == "10");
    CHECK(b.CuckooFind(49));
    
    CuckooTable empty;
    empty.clear();
    CuckooTable copyOfEmpty(empty);
    CHECK(copyOfEmpty.empty());
}

TEST_CASE("cuckooTable: copy_assignment") {
    CuckooTable a, b;
    a.CuckooInsert(1, "a");
    b.CuckooInsert(2, "b");
    b = a;
    CHECK(b.CuckooFind(1));
    CHECK_FALSE(b.CuckooFind(2));
    b = b;
    CHECK(b.CuckooFind(1));
}

TEST_CASE("cuckooTable: move_constructor_and_assignment") {
    CuckooTable a;
    a.CuckooInsert(1, "a");
    CuckooTable b(std::move(a));
    CHECK(b.CuckooFind(1));
    CHECK(a.empty());
    CHECK_FALSE(a.CuckooFind(1));
    
    CuckooTable c;
    c.CuckooInsert(2, "c");
    c = std::move(b);
    CHECK(c.CuckooFind(1));
    CHECK_FALSE(c.CuckooFind(2));
    CHECK(b.empty());
    
    CuckooTable& same = c;
    c = std::move(same);
    CHECK(c.CuckooFind(1));
}