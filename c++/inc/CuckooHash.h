#pragma once
#include <string>
#include <vector>

class CuckooTable {
private:
    struct cell {
        int key;
        std::string value;
        bool used;
        cell();
    };
    cell* table1;
    cell* table2;
    int size;
    int count;
    unsigned int seed1;
    unsigned int seed2;
    int kicks;

    static const int MAX_KICKS = 30;

    int hash1(int key) const;
    int hash2(int key) const;
    unsigned int nextSeed(unsigned int s) const;
    void allocTables(int newSize);
    bool placeInto(int key, const std::string& value);
    void rehash();//

public:
    CuckooTable(int startSize = 16);
    ~CuckooTable();//

    CuckooTable(const CuckooTable& other);//
    CuckooTable& operator=(const CuckooTable& other);//
    CuckooTable(CuckooTable&& other) noexcept;//
    CuckooTable& operator=(CuckooTable&& other) noexcept;//

    bool empty();//
    void clear();//
    bool CuckooInsert(int key, const std::string& value);//
    bool CuckooFind(int key);//
    bool getValue(int key, std::string& out);//
    bool CuckooRemove(int key);//

    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    void saveToBinar(const std::string& filename) const;
    void loadFromBinar(const std::string& filename);
std::vector<std::pair<int, std::string>> items() const;
};