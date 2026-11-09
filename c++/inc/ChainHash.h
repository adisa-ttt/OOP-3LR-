#pragma once
#include <string>
#include <vector>

class ChainTable {
private:
    struct Node {
        int key; 
        std::string value;
        Node* prev;
        Node* next;
        Node(int k, const std::string& value, Node* p, Node* n);
    };
    Node** table;
    int size;
    int count;

    int hashKey(int key) const;
    void rehash();//
public:
    ChainTable(int startSize = 8);
    ~ChainTable();  

    ChainTable(const ChainTable& other);//
    ChainTable& operator=(const ChainTable& other);//
    ChainTable(ChainTable&& other) noexcept;//
    ChainTable& operator=(ChainTable&& other) noexcept;//

    bool empty();//
    void clear();//
    bool ChainInsert (int key, const std::string& value);//
    bool chainFind (int key);//
    bool chainRemove (int key);///
    bool getValue (int key, std::string& out);//

    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    void saveToBinar(const std::string& filename) const;
    void loadFromBinar(const std::string& filename);
std::vector<std::pair<int, std::string>> items() const;
};