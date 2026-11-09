#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <string>
#include "CuckooHash.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(cuckooTable_tests)

BOOST_AUTO_TEST_CASE(empty) {
    CuckooTable t;
    BOOST_CHECK(t.empty());
    BOOST_CHECK(!t.CuckooFind(1));
}

BOOST_AUTO_TEST_CASE(insert_find_getValue) {
    CuckooTable t;
    BOOST_CHECK(t.CuckooInsert(1, "one"));
    BOOST_CHECK(t.CuckooInsert(2, "two"));
    BOOST_CHECK(!t.empty());
    BOOST_CHECK(t.CuckooFind(1));
    BOOST_CHECK(!t.CuckooFind(3));
    string out;
    BOOST_CHECK(t.getValue(2, out));
    BOOST_CHECK_EQUAL(out, "two");
    BOOST_CHECK(!t.getValue(3, out));
}

BOOST_AUTO_TEST_CASE(insert_existing_key_updates_value) {
    CuckooTable t;
    BOOST_CHECK(t.CuckooInsert(7, "a"));
    BOOST_CHECK(!t.CuckooInsert(7, "b"));
    string out;
    BOOST_CHECK(t.getValue(7, out));
    BOOST_CHECK_EQUAL(out, "b");
    BOOST_CHECK(t.CuckooRemove(7));
    BOOST_CHECK(t.empty());
}

BOOST_AUTO_TEST_CASE(remove) {
    CuckooTable t;
    BOOST_CHECK(!t.CuckooRemove(1));
    t.CuckooInsert(1, "a");
    BOOST_CHECK(!t.CuckooRemove(2));
    BOOST_CHECK(t.CuckooRemove(1));
    BOOST_CHECK(!t.CuckooFind(1));
    BOOST_CHECK(t.empty());
}

BOOST_AUTO_TEST_CASE(clear) {
    CuckooTable t;
    t.CuckooInsert(1, "a");
    t.clear();
    BOOST_CHECK(t.empty());
    BOOST_CHECK(!t.CuckooFind(1));
    BOOST_CHECK(!t.CuckooRemove(1));
    string out;
    BOOST_CHECK(!t.getValue(1, out));
}

BOOST_AUTO_TEST_CASE(copy_constructor) {
    CuckooTable a;
    for (int i = 0; i < 50; i++) a.CuckooInsert(i, std::to_string(i));
    CuckooTable b(a);
    a.CuckooRemove(10);
    string out;
    BOOST_CHECK(b.getValue(10, out));
    BOOST_CHECK_EQUAL(out, "10");
    BOOST_CHECK(b.CuckooFind(49));
    
    CuckooTable empty;
    empty.clear();
    CuckooTable copyOfEmpty(empty);
    BOOST_CHECK(copyOfEmpty.empty());
}

BOOST_AUTO_TEST_CASE(copy_assignment) {
    CuckooTable a, b;
    a.CuckooInsert(1, "a");
    b.CuckooInsert(2, "b");
    b = a;
    BOOST_CHECK(b.CuckooFind(1));
    BOOST_CHECK(!b.CuckooFind(2));
    b = b;
    BOOST_CHECK(b.CuckooFind(1));
}

BOOST_AUTO_TEST_CASE(move_constructor_and_assignment) {
    CuckooTable a;
    a.CuckooInsert(1, "a");
    CuckooTable b(std::move(a));
    BOOST_CHECK(b.CuckooFind(1));
    BOOST_CHECK(a.empty());
    BOOST_CHECK(!a.CuckooFind(1));
    
    CuckooTable c;
    c.CuckooInsert(2, "c");
    c = std::move(b);
    BOOST_CHECK(c.CuckooFind(1));
    BOOST_CHECK(!c.CuckooFind(2));
    BOOST_CHECK(b.empty());
    
    CuckooTable& same = c;
    c = std::move(same);
    BOOST_CHECK(c.CuckooFind(1));
}

BOOST_AUTO_TEST_SUITE_END()