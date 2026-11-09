#include "ChainHash.h"
#include <utility>
#include <fstream>
#include <stdexcept>
#include <vector>

using namespace std;

ChainTable::Node::Node(int key, const string& value, Node* p, Node* n) 
    : key(key), value(value), prev(p), next(n) {}

ChainTable::ChainTable(int startSize) : table(nullptr)
    , size(0), count(0) {
        size = startSize;
        table = new Node*[size];
        for (int i = 0; i < size; ++i) {
            table[i] = nullptr;
        }
}

ChainTable::~ChainTable() { clear(); }

ChainTable::ChainTable(const ChainTable& other) 
    : table(nullptr), size(0), count(0) {
    if (other.size > 0) {
        size = other.size;
        table = new Node*[size];
        for (int i = 0; i < size; ++i) {
            table[i] = nullptr; }
        for (int i = 0; i < other.size; ++i) {
            Node* curr = other.table[i];
            while (curr != nullptr) {
                ChainInsert(curr->key, curr->value);
                curr = curr->next;
            }
        }
    }

}

ChainTable& ChainTable::operator=(const ChainTable& other){ 
    if (this != &other) {
        ChainTable temp(other);
        swap(table, temp.table);
        swap(size, temp.size);
        swap(count, temp.count);
    }
    return *this;
}

ChainTable::ChainTable(ChainTable&& other) noexcept 
    : table(other.table), size(other.size), count(other.count) {
    other.table = nullptr;
    other.count = 0;
    other.size = 0;
}

ChainTable& ChainTable::operator=(ChainTable&& other) noexcept {
    if (this != &other) {
        clear();
        table = other.table;
        size = other.size;
        count = other.count;
        other.table = nullptr;
        other.count = 0;
        other.size = 0;
    }
    return *this;
}

int ChainTable::hashKey(int key) const {
    return ((key%size)+size)%size;
}

void ChainTable::rehash() {
    int oldSize  = size;
    int newSize = oldSize*2;
    Node** newTable = new Node*[newSize];
    for (int i = 0; i < newSize; ++i) {
        newTable[i] = nullptr;
    }
    Node** oldTable = table;
    table = newTable;
    size = newSize;
    for (int i = 0; i < oldSize; ++i) {
        Node* curr = oldTable[i];
        while (curr != nullptr) {
            Node* next = curr->next;
            int idx = hashKey(curr->key);
            curr->prev = nullptr;
            curr->next = table[idx];
            if (table[idx] != nullptr) {
                table[idx]->prev = curr; }
            table[idx] = curr;
            curr = next;
        }
    }
    delete [] oldTable;
}

bool ChainTable::empty() {
    return count == 0;
}

void ChainTable::clear() {
    if (table != nullptr) {
        for (int i = 0; i < size; ++i) {
            Node* curr = table[i];
            while (curr != nullptr) {
                Node* next = curr->next;
                delete curr;
                curr = next;
            }
        }
        delete[] table;
    }
    table = nullptr;
    size = 0;
    count = 0;
}

bool ChainTable::ChainInsert (int key, const std::string& value) {
    if (table == nullptr) return false;
    int idx = hashKey(key);
    for (Node* curr = table[idx]; curr != nullptr; curr = curr->next) {
        if (curr->key == key) {
            curr->value = value;
            return false; }
    }
    Node* newNode = new Node(key, value, nullptr, table[idx]);
    if (table[idx] != nullptr) {
        table[idx]->prev = newNode; }
    table[idx] = newNode;
    ++count;
    if (count > size) rehash();
    return true;
}

bool ChainTable::chainFind (int key) {
    if (table == nullptr) return false;
    int idx = hashKey(key);
    for (Node* curr = table[idx]; curr != nullptr; curr = curr->next) {
        if (curr->key == key) return true;
    }
    return false;
}

bool ChainTable::chainRemove (int key) {
    if (table == nullptr) return false;
    int idx = hashKey(key);
    for (Node* curr = table[idx]; curr != nullptr; curr = curr->next) {
        if (curr->key == key) {
            if (curr->prev != nullptr) {
                curr->prev->next = curr->next; }
            else {
                table[idx] = curr->next; }
            if (curr->next != nullptr) {
                curr->next->prev = curr->prev; }
            delete curr;
            --count;
            return true;
        }
    }
    return false;
}

bool ChainTable::getValue (int key, std::string& out) {
    if (table == nullptr) return false;
    int idx = hashKey(key);
    for (Node* curr = table[idx]; curr != nullptr; curr = curr->next) {
        if (curr->key == key) {
            out = curr->value;
            return true;}
    }
    return false;
}

void ChainTable::saveToFile(const std::string& filename) const {
    ofstream out(filename);
    if (!out.is_open()) throw std::runtime_error("can not open file " + filename);

    out << count << "\n";
    for (int i = 0; i < size; ++i) {
        Node* curr = table[i];
        while (curr != nullptr) {
            out << curr->key << " " << curr->value << "\n";
            curr = curr->next;
        }
    }
}

void ChainTable::loadFromFile(const std::string& filename) {
    ifstream in(filename);
    if (!in.is_open()) throw std::runtime_error("can not open file " + filename);

    clear();
    int n;
    in >> n;
    for (int i = 0; i < n; ++i) {
        int key;
        string value;
        in >> key >> value;
        ChainInsert(key, value);
    }
}

void ChainTable::saveToBinar(const std::string& filename) const {
    ofstream out(filename, std::ios::binary);
    if (!out.is_open()) throw std::runtime_error("can not open file " + filename);

    out.write(reinterpret_cast<const char*>(&count), sizeof(int));
    for (int i = 0; i < size; ++i) {
        Node* curr = table[i];
        while (curr != nullptr) {
            out.write(reinterpret_cast<const char*>(&curr->key), sizeof(int));
            size_t len = curr->value.size();
            out.write(reinterpret_cast<const char*>(&len), sizeof(size_t));
            out.write(curr->value.data(), len);
            curr = curr->next;
        }
    }
}

void ChainTable::loadFromBinar(const std::string& filename) {
    ifstream in(filename, std::ios::binary);
    if (!in.is_open()) throw std::runtime_error("can not open file " + filename);

    clear();
    int n;
    in.read(reinterpret_cast<char*>(&n), sizeof(int));
    for (int i = 0; i < n; ++i) {
        int key;
        in.read(reinterpret_cast<char*>(&key), sizeof(int));
        size_t len;
        in.read(reinterpret_cast<char*>(&len), sizeof(int));
        string value(len, '\0');
        in.read(&value[0], len);
        ChainInsert(key, value);
    }
}

// ChainHash.cpp
std::vector<std::pair<int, std::string>> ChainTable::items() const {
    std::vector<std::pair<int, std::string>> res;
    for (int i = 0; i < size; ++i)
        for (Node* c = table[i]; c; c = c->next)
            res.push_back({c->key, c->value});
    return res;
}
