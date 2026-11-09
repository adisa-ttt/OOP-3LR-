#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <string>
#include <vector>
#include "Array.h"
#include "Stack.h"
#include "Queue.h"
#include "LinkedList.h"
#include "DoubleLinkedList.h"
#include "avlTree.h"
#include "ChainHash.h"
#include "CuckooHash.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(serialization_tests)

//  ARRAY  ----------------------------------------------------------


BOOST_AUTO_TEST_CASE(array_text_empty) {
    Array a;
    a.saveToFile("/tmp/test_array_empty.txt");

    Array b;
    b.loadFromFile("/tmp/test_array_empty.txt");
    BOOST_CHECK(b.empty());
    BOOST_CHECK_EQUAL(b.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(array_binary_empty) {
    Array a;
    a.saveToBinar("/tmp/test_array_empty.bin");

    Array b;
    b.loadFromBinar("/tmp/test_array_empty.bin");
    BOOST_CHECK(b.empty());
}

BOOST_AUTO_TEST_CASE(array_load_nonexistent_text) {
    Array a;
    BOOST_CHECK_THROW(a.loadFromFile("/tmp/nonexistent_array.txt"), runtime_error);
}

BOOST_AUTO_TEST_CASE(array_load_nonexistent_binary) {
    Array a;
    BOOST_CHECK_THROW(a.loadFromBinar("/tmp/nonexistent_array.bin"), runtime_error);
}

//  STACK-----------------------------------------------------------------


BOOST_AUTO_TEST_CASE(stack_text_empty) {
    Stack s;
    s.saveToFile("/tmp/test_stack_empty.txt");

    Stack t;
    t.loadFromFile("/tmp/test_stack_empty.txt");
    BOOST_CHECK(t.empty());
}

BOOST_AUTO_TEST_CASE(stack_binary_empty) {
    Stack s;
    s.saveToBinar("/tmp/test_stack_empty.bin");

    Stack t;
    t.loadFromBinar("/tmp/test_stack_empty.bin");
    BOOST_CHECK(t.empty());
}

BOOST_AUTO_TEST_CASE(stack_load_nonexistent) {
    Stack s;
    BOOST_CHECK_THROW(s.loadFromFile("/tmp/nonexistent_stack.txt"), runtime_error);
    BOOST_CHECK_THROW(s.loadFromBinar("/tmp/nonexistent_stack.bin"), runtime_error);
}

//  QUEUE------------------------------------------------------------------------------------


BOOST_AUTO_TEST_CASE(queue_binary_roundtrip) {
    Queue q;
    q.push("x");
    q.push("y");
    q.saveToBinar("/tmp/test_queue.bin");

    Queue r;
    r.loadFromBinar("/tmp/test_queue.bin");
    BOOST_CHECK_EQUAL(q.get().size(), r.get().size());
    for (size_t i = 0; i < q.get().size(); ++i) {
        BOOST_CHECK_EQUAL(q.get()[i], r.get()[i]);
    }
}

BOOST_AUTO_TEST_CASE(queue_text_empty) {
    Queue q;
    q.saveToFile("/tmp/test_queue_empty.txt");

    Queue r;
    r.loadFromFile("/tmp/test_queue_empty.txt");
    BOOST_CHECK(r.empty());
}

BOOST_AUTO_TEST_CASE(queue_binary_empty) {
    Queue q;
    q.saveToBinar("/tmp/test_queue_empty.bin");

    Queue r;
    r.loadFromBinar("/tmp/test_queue_empty.bin");
    BOOST_CHECK(r.empty());
}

BOOST_AUTO_TEST_CASE(queue_load_nonexistent) {
    Queue q;
    BOOST_CHECK_THROW(q.loadFromFile("/tmp/nonexistent_queue.txt"), runtime_error);
    BOOST_CHECK_THROW(q.loadFromBinar("/tmp/nonexistent_queue.bin"), runtime_error);
}

//  LINKED LIST----------------------------------------------------------------------------


BOOST_AUTO_TEST_CASE(linklist_text_empty) {
    LinkList l;
    l.saveToFile("/tmp/test_list_empty.txt");

    LinkList m;
    m.loadFromFile("/tmp/test_list_empty.txt");
    BOOST_CHECK(m.empty());
}

BOOST_AUTO_TEST_CASE(linklist_binary_empty) {
    LinkList l;
    l.saveToBinar("/tmp/test_list_empty.bin");

    LinkList m;
    m.loadFromBinar("/tmp/test_list_empty.bin");
    BOOST_CHECK(m.empty());
}

BOOST_AUTO_TEST_CASE(linklist_load_nonexistent) {
    LinkList l;
    BOOST_CHECK_THROW(l.loadFromFile("/tmp/nonexistent_list.txt"), runtime_error);
    BOOST_CHECK_THROW(l.loadFromBinar("/tmp/nonexistent_list.bin"), runtime_error);
}

//  DOUBLE LINKED LIST---------------------------------------------------------------


BOOST_AUTO_TEST_CASE(doublelist_text_empty) {
    doubleLinkList l;
    l.saveToFile("/tmp/test_dll_empty.txt");

    doubleLinkList m;
    m.loadFromFile("/tmp/test_dll_empty.txt");
    BOOST_CHECK(m.empty());
}

BOOST_AUTO_TEST_CASE(doublelist_binary_empty) {
    doubleLinkList l;
    l.saveToBinar("/tmp/test_dll_empty.bin");

    doubleLinkList m;
    m.loadFromBinar("/tmp/test_dll_empty.bin");
    BOOST_CHECK(m.empty());
}

BOOST_AUTO_TEST_CASE(doublelist_load_nonexistent) {
    doubleLinkList l;
    BOOST_CHECK_THROW(l.loadFromFile("/tmp/nonexistent_dll.txt"), runtime_error);
    BOOST_CHECK_THROW(l.loadFromBinar("/tmp/nonexistent_dll.bin"), runtime_error);
}

//  AVL TREE-----------------------------------------------------------------


BOOST_AUTO_TEST_CASE(avltree_text_empty) {
    avlTree t;
    t.saveToFile("/tmp/test_avl_empty.txt");

    avlTree u;
    u.loadFromFile("/tmp/test_avl_empty.txt");
    BOOST_CHECK(u.empty());
}

BOOST_AUTO_TEST_CASE(avltree_binary_empty) {
    avlTree t;
    t.saveToBinar("/tmp/test_avl_empty.bin");

    avlTree u;
    u.loadFromBinar("/tmp/test_avl_empty.bin");
    BOOST_CHECK(u.empty());
}

BOOST_AUTO_TEST_CASE(avltree_load_nonexistent) {
    avlTree t;
    BOOST_CHECK_THROW(t.loadFromFile("/tmp/nonexistent_avl.txt"), runtime_error);
    BOOST_CHECK_THROW(t.loadFromBinar("/tmp/nonexistent_avl.bin"), runtime_error);
}

//  CHAIN HASH —--------------------------------------------------------

BOOST_AUTO_TEST_CASE(chainhash_text_empty) {
    ChainTable t(128);
    t.saveToFile("/tmp/test_chain_empty.txt");

    ChainTable u(128);
    u.loadFromFile("/tmp/test_chain_empty.txt");
    BOOST_CHECK(u.empty());
}

BOOST_AUTO_TEST_CASE(chainhash_binary_empty) {
    ChainTable t(128);
    t.saveToBinar("/tmp/test_chain_empty.bin");

    ChainTable u(128);
    u.loadFromBinar("/tmp/test_chain_empty.bin");
    BOOST_CHECK(u.empty());
}

BOOST_AUTO_TEST_CASE(chainhash_load_nonexistent) {
    ChainTable t(128);
    BOOST_CHECK_THROW(t.loadFromFile("/tmp/nonexistent_chain.txt"), runtime_error);
    BOOST_CHECK_THROW(t.loadFromBinar("/tmp/nonexistent_chain.bin"), runtime_error);
}

//  CUCKOO HASH —----------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(cuckoohash_text_empty) {
    CuckooTable t(128);
    t.saveToFile("/tmp/test_cuckoo_empty.txt");

    CuckooTable u(128);
    u.loadFromFile("/tmp/test_cuckoo_empty.txt");
    BOOST_CHECK(u.empty());
}

BOOST_AUTO_TEST_CASE(cuckoohash_binary_empty) {
    CuckooTable t(128);
    t.saveToBinar("/tmp/test_cuckoo_empty.bin");

    CuckooTable u(128);
    u.loadFromBinar("/tmp/test_cuckoo_empty.bin");
    BOOST_CHECK(u.empty());
}

BOOST_AUTO_TEST_CASE(cuckoohash_load_nonexistent) {
    CuckooTable t(128);
    BOOST_CHECK_THROW(t.loadFromFile("/tmp/nonexistent_cuckoo.txt"), runtime_error);
    BOOST_CHECK_THROW(t.loadFromBinar("/tmp/nonexistent_cuckoo.bin"), runtime_error);
}

BOOST_AUTO_TEST_CASE(cuckoo_binary_roundtrip) {
    CuckooTable ct;
    ct.CuckooInsert(10, "a");
    ct.CuckooInsert(15, "b");
    ct.CuckooInsert(17, "c");
    ct.CuckooInsert(78, "d");
    ct.CuckooInsert(12, "e");

    ct.saveToBinar("/tmp/test_cuckoo.bin");

    CuckooTable ctt;
    ctt.loadFromBinar("/tmp/test_cuckoo.bin");
    BOOST_CHECK_EQUAL(ct.items().size(), ctt.items().size());

}

BOOST_AUTO_TEST_CASE(cuckoo_test_roadtrip) {
    CuckooTable ct;
    ct.CuckooInsert(10, "a");
    ct.CuckooInsert(15, "b");
    ct.CuckooInsert(17, "c");
    ct.CuckooInsert(78, "d");
    ct.CuckooInsert(12, "e");

    ct.saveToFile("/tmp/test_cuckoo.txt");

    CuckooTable ctt;
    ctt.loadFromFile("/tmp/test_cuckoo.txt");
    BOOST_CHECK_EQUAL(ct.items().size(), ctt.items().size());
    BOOST_CHECK_EQUAL(ct.CuckooFind(10), ctt.CuckooFind(10));
    }


BOOST_AUTO_TEST_SUITE_END()

/*
cd ..
cd ~/PROGA/3LR/c++
cmake -B build && cmake --build build
./build/boost_tests
./build/gtest_tests
./build/catch2_tests  

cd ~/PROGA/3LR/c++/build
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON ..
make -j$(nproc)

./boost_tests --run_test=cuckoo_test_roadtrip
*/