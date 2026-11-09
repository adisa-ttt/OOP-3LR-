#pragma once
#include <string>
#include <vector>
#include <map>
#include "LinkedList.h"
#include "Queue.h"
#include "Stack.h"
#include "DoubleLinkedList.h"
#include "avlTree.h"
#include "Array.h"
#include "ChainHash.h"
#include "CuckooHash.h"

enum structType {
    STACK, QUEUE, LIST, DOUBLE_LIST, TREE, ARRAY, CHAINHASH, CUCKOOHASH
};

struct Structure {
    structType type;
    void* data;
};

    extern std::map<std::string, Structure> registr;
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename);
    bool executeCmd(const std::string& query);
    bool isModified(const std::string& query);
    std::vector<std::string> parseQuery(const std::string& query);
    void clearRegistr();
