#include "CuckooHash.h"
#include "murmur3.h"
#include <utility>
#include <stdexcept>
#include <fstream>
#include <vector>
using namespace std;

extern unsigned int murmur3_32(unsigned int key, unsigned int seed);

CuckooTable::cell::cell() : key(0), value(""), used(false) {}

CuckooTable::CuckooTable(int startSize) 
    : table1(nullptr), table2(nullptr), size(0), count(0)
        , seed1(0x9E3779B9U), seed2(0x85EBCA6BU), kicks(0) {
    allocTables(startSize);
}

CuckooTable::~CuckooTable() { clear(); }

CuckooTable::CuckooTable(const CuckooTable& other) 
    : table1(nullptr), table2(nullptr), size(0), count(0)
        , seed1(other.seed1), seed2(other.seed2), kicks(0) {
    if (other.size > 0) {
        allocTables(other.size);
        for (int i = 0; i < other.size; ++i) {
            if (other.table1[i].used) {
                CuckooInsert(other.table1[i].key, other.table1[i].value); }
            if (other.table2[i].used) {
                CuckooInsert(other.table2[i].key, other.table2[i].value); }
        }
    }
} 

CuckooTable& CuckooTable::operator=(const CuckooTable& other) {
    if (this != &other) {
        CuckooTable temp(other);
        swap(table1, temp.table1);
        swap(table2, temp.table2);
        swap(size, temp.size);
        swap(count, temp.count);
        swap(seed1, temp.seed1);
        swap(seed2, temp.seed2);
        swap(kicks, temp.kicks);
    }
    return *this;
}

CuckooTable::CuckooTable(CuckooTable&& other) noexcept 
    : table1(other.table1), table2(other.table2), size(other.size),
        count(other.count), seed1(other.seed1), seed2(other.seed2)
            , kicks(other.kicks) {
    other.count = 0;
    other.table1 = nullptr;
    other.table2 = nullptr;
    other.size = 0;
}

CuckooTable& CuckooTable::operator=(CuckooTable&& other) noexcept {
    if (this != &other) {
        clear();
        table1 = other.table1;
        table2 = other.table2;
        size = other.size;
        count = other.count;
        seed1 = other.seed1;
        seed2 = other.seed2;
        kicks = other.kicks;
        other.table1 = nullptr;
        other.table2 = nullptr;
        other.size = 0;
        other.count = 0;
    }
    return *this;
}

int CuckooTable::hash1(int key) const {
    return static_cast<int>(murmur3_32(static_cast<unsigned int>(key), seed1) % static_cast<unsigned int>(size));
}

int CuckooTable::hash2(int key) const {
    return static_cast<int>(murmur3_32(static_cast<unsigned int>(key), seed2) % static_cast<unsigned int>(size));

}

unsigned int CuckooTable::nextSeed(unsigned int s) const {
    return s*1664525U + 1013904223U;
}

void CuckooTable::allocTables(int newSize) {
    table1 = new cell[newSize]();
    table2 = new cell[newSize]();
    size = newSize;
}

bool CuckooTable::placeInto(int key, const std::string& value) {
    std::string currentValue = value;
    for (int i = 0; i < MAX_KICKS; ++i) {
        cell& c1 = table1[hash1(key)];
        if (!c1.used) {
            c1.key = key;
            c1.value = std::move(currentValue);
            c1.used = true;
            return true;
        }
        swap(key, c1.key);
        swap(currentValue, c1.value);
        ++kicks;

        cell& c2 = table2[hash2(key)];
        if (!c2.used) {
            c2.key = key;
            c2.value = std::move(currentValue);
            c2.used = true;
            return true;
        }
        swap(key, c2.key);
        swap(currentValue, c2.value);
        kicks++;
    }
    return false;
}

void CuckooTable::rehash() {
    int oldSize = size;
    int newSize = oldSize*2;

    cell* old_t1 = table1;
    cell* old_t2 = table2;
    seed1 = nextSeed(seed1);
    seed2 = nextSeed(seed2);

    allocTables(newSize);
    count = 0;

    for (int i = 0; i < oldSize; ++i) {
        if (old_t1[i].used) {
            placeInto(old_t1[i].key, old_t1[i].value);
            ++count; }
        if (old_t2[i].used) {
            placeInto(old_t2[i].key, old_t2[i].value);
            ++count; }
    }
    delete[] old_t1;
    delete[] old_t2;
}

bool CuckooTable::empty() {
    return count == 0;
}

bool CuckooTable::CuckooInsert(int key, const std::string& value) {
    if (table1 == nullptr) return false;
    if (CuckooFind(key)) {
        int idx1 = hash1(key);
        if (table1[idx1].used && table1[idx1].key == key) {
            table1[idx1].value = value;
            return false; }
        int idx2 = hash2(key);
        if (table2[idx2].used && table2[idx2].key == key) {
            table2[idx2].value = value;
            return false; }
    }
    if (count*2 >= size) rehash();
    if (!placeInto(key, value)) {
        rehash();
        if (!placeInto(key, value)) { 
            return false; 
        }
    }
    ++count;
    return true;
}

bool CuckooTable::CuckooFind(int key) {
    if (table1 == nullptr) return false;
    int idx1 = hash1(key);
    if (table1[idx1].used && table1[idx1].key == key) {
        return true;
    }
    int idx2 = hash2(key);
    if (table2[idx2].used && table2[idx2].key == key) {
        return true;
    }
    return false;
}

bool CuckooTable::getValue(int key, std::string& out) {
    if (table1 == nullptr) return false;
    int idx1 = hash1(key);
    if (table1[idx1].used && table1[idx1].key == key) {
        out = table1[idx1].value;
        return true;
    }
    int idx2 = hash2(key);
    if (table2[idx2].used && table2[idx2].key == key) {
        out = table2[idx2].value;
        return true;
    }
    return false;
}

bool CuckooTable::CuckooRemove(int key) {
    if (table1 == nullptr) return false;
    int idx1 = hash1(key);
    if (table1[idx1].used && table1[idx1].key == key) {
        table1[idx1].used = false;
        table1[idx1].value.clear();
        --count;
        return true; }
    int idx2 = hash2(key);
    if (table2[idx2].used && table2[idx2].key == key) {
        table2[idx2].used = false;
        table2[idx2].value.clear();
        --count;
        return true; }
    return false;
}

void CuckooTable::clear() {
    if (table1 != nullptr) {
        delete[] table1;
        table1 = nullptr;
    }
    if (table2 != nullptr) {
        delete[] table2;
        table2 = nullptr;
    }
    size = 0;
    count = 0;
}


void CuckooTable::saveToFile(const std::string& filename) const {
    ofstream out(filename);
    if (!out.is_open()) throw std::runtime_error("can not open file " + filename);

    out << count << "\n";
    for (int i = 0; i < size; ++i) {
        if (table1[i].used) {
            out << table1[i].key << " " << table1[i].value << "\n"; }
        if (table2[i].used) {
            out << table2[i].key << " " << table2[i].value << "\n"; }
    }
}

void CuckooTable::loadFromFile(const std::string& filename) {
    ifstream in(filename);
    if (!in.is_open()) throw std::runtime_error("can not open file " + filename);
    
    clear();
    int n;
    in >> n;
    allocTables(n > 0 ? n * 2 : 10); 
    for (int i = 0; i < n; ++i) {
        int key;
        string value;
        in >> key >> value;
        CuckooInsert(key, value);
    }
}

void CuckooTable::saveToBinar(const std::string& filename) const {
    ofstream out(filename, std::ios::binary);
    if (!out.is_open()) throw std::runtime_error("can not open file " + filename);

    out.write(reinterpret_cast<const char*>(&count), sizeof(int));
    for (int i = 0; i < size; ++i) {
        if (table1[i].used) {
            out.write(reinterpret_cast<char*>(&table1[i].key), sizeof(int));
            size_t len = table1[i].value.size();
            out.write(reinterpret_cast<char*>(&len), sizeof(size_t));
            out.write(table1[i].value.data(), len); }
        if (table2[i].used) {
            out.write(reinterpret_cast<char*>(&table2[i].key), sizeof(int));
            size_t len = table2[i].value.size();
            out.write(reinterpret_cast<char*>(&len), sizeof(size_t));
            out.write(table2[i].value.data(), len); }
    }
}

void CuckooTable::loadFromBinar(const std::string& filename) {
    ifstream in(filename, std::ios::binary);
    if (!in.is_open()) throw std::runtime_error("can not open file " + filename);

    clear();
    int n;
    in.read(reinterpret_cast<char*>(&n), sizeof(int));
    allocTables(n > 0 ? n * 2 : 10); 
    for (int i = 0; i < n; ++i) {
        int key;
        in.read(reinterpret_cast<char*>(&key), sizeof(int));
        size_t len;
        in.read(reinterpret_cast<char*>(&len), sizeof(size_t));
        string value(len, '\0');
        in.read(&value[0], len);
        CuckooInsert(key, value);
    }
}

std::vector<std::pair<int, std::string>> CuckooTable::items() const {
    std::vector<std::pair<int, std::string>> res;
    for (int i = 0; i < size; ++i) {
        if (table1[i].used) res.push_back({table1[i].key, table1[i].value});
        if (table2[i].used) res.push_back({table2[i].key, table2[i].value});
    }
    return res;
}