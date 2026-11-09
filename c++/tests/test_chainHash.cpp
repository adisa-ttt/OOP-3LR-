#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <string>
#include "ChainHash.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(chainTable_tests)

BOOST_AUTO_TEST_CASE(empty) {
    ChainTable t;
    BOOST_CHECK(t.empty());
    BOOST_CHECK(!t.chainFind(1));
}

BOOST_AUTO_TEST_CASE(insert_find_getValue) {
    ChainTable t;
    BOOST_CHECK(t.ChainInsert(1, "one"));
    BOOST_CHECK(t.ChainInsert(-2, "-two"));
    BOOST_CHECK(!t.empty());
    BOOST_CHECK(t.chainFind(1));
    BOOST_CHECK(!t.chainFind(3));
    string out;
    BOOST_CHECK(t.getValue(-2, out));
    BOOST_CHECK_EQUAL(out, "-two");
    BOOST_CHECK(!t.getValue(3, out));
}

BOOST_AUTO_TEST_CASE(insert_existing_key_updates_value) {
    ChainTable t;
    BOOST_CHECK(t.ChainInsert(1, "a"));
    BOOST_CHECK(!t.ChainInsert(1, "b"));
    string out;
    BOOST_CHECK(t.getValue(1, out));
    BOOST_CHECK_EQUAL(out, "b");
}

BOOST_AUTO_TEST_CASE(collisions_in_one_chain) {
    ChainTable t(8);
    BOOST_CHECK(t.ChainInsert(1, "a"));
    BOOST_CHECK(t.ChainInsert(9, "b"));
    BOOST_CHECK(t.ChainInsert(17, "c"));
    BOOST_CHECK(t.chainFind(1));
    BOOST_CHECK(t.chainFind(9));
    BOOST_CHECK(t.chainFind(17));
    BOOST_CHECK(t.chainRemove(9));
    BOOST_CHECK(t.chainRemove(17));
    BOOST_CHECK(t.chainRemove(1));
    BOOST_CHECK(t.empty());
}

BOOST_AUTO_TEST_CASE(remove) {
    ChainTable t;
    BOOST_CHECK(!t.chainRemove(1));
    t.ChainInsert(1, "a");
    BOOST_CHECK(!t.chainRemove(2));
    BOOST_CHECK(t.chainRemove(1));
    BOOST_CHECK(!t.chainRemove(1));
}

BOOST_AUTO_TEST_CASE(clear) {
    ChainTable t;
    t.ChainInsert(1, "a");
    t.clear();
    BOOST_CHECK(t.empty());
    BOOST_CHECK(!t.chainFind(1));
    BOOST_CHECK(!t.chainRemove(1));
    string out;
    BOOST_CHECK(!t.getValue(1, out));
    BOOST_CHECK(!t.ChainInsert(2, "b"));
}

BOOST_AUTO_TEST_CASE(copy_constructor) {
    ChainTable a;
    for (int i = 0; i < 20; i++) a.ChainInsert(i, std::to_string(i));
    ChainTable b(a);
    a.chainRemove(5);
    string out;
    BOOST_CHECK(b.getValue(5, out));
    BOOST_CHECK_EQUAL(out, "5");
    BOOST_CHECK(b.chainFind(19));
}

BOOST_AUTO_TEST_CASE(copy_assignment) {
    ChainTable a, b;
    a.ChainInsert(1, "a");
    b.ChainInsert(2, "b");
    b = a;
    BOOST_CHECK(b.chainFind(1));
    BOOST_CHECK(!b.chainFind(2));
    b = b;
    BOOST_CHECK(b.chainFind(1));
}

BOOST_AUTO_TEST_CASE(move_constructor_and_assignment) {
    ChainTable a;
    a.ChainInsert(1, "a");
    ChainTable b(std::move(a));
    BOOST_CHECK(b.chainFind(1));
    BOOST_CHECK(a.empty());
    BOOST_CHECK(!a.chainFind(1));
    
    ChainTable c;
    c.ChainInsert(2, "c");
    c = std::move(b);
    BOOST_CHECK(c.chainFind(1));
    BOOST_CHECK(!c.chainFind(2));
    BOOST_CHECK(b.empty());
    
    ChainTable& same = c;
    c = std::move(same);
    BOOST_CHECK(c.chainFind(1));
}

BOOST_AUTO_TEST_SUITE_END()