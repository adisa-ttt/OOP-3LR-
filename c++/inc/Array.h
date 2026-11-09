#pragma once 
#include <string>
#include <vector>

class Array {
private:
    std::string* data;
    int size;
    int capacity;
    static const int START_CAPACITY = 4;
    void resize();
public:
    Array();
    ~Array();

    Array(const Array& other);
    Array& operator=(const Array& other);
    Array(Array&& other) noexcept;
    Array& operator=(Array&& other) noexcept;

    bool empty() const;//
    void clear();//
    void push_back(const std::string str);//
    bool insert(int index, const std::string str);//
    bool removeAt(int index);//
    int getSize() const;//
    bool set(int index, const std::string& str);//
    std::string getAt(int index) const;//
    std::vector<std::string> get() const;//

    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    void saveToBinar(const std::string& filename) const;
    void loadFromBinar(const std::string& filename);
};