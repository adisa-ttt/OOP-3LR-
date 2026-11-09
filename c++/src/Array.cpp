#include "Array.h"
#include <stdexcept>
#include <utility>
#include <fstream>
using namespace std;

Array::Array() : data(nullptr), size(0), capacity(0) {}

Array::~Array() { clear(); }

Array::Array(const Array& other) : data(nullptr), size(0), capacity(0) {
    if (other.size > 0) {
        capacity = other.capacity;
        size = other.size;
        data = new std::string[capacity];
        for (int i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }
}

Array& Array::operator=(const Array& other) {
    if (this != &other) {
        Array temp(other);
        swap(data, temp.data);
        swap(size, temp.size);
        swap(capacity, temp.capacity);
    }
    return *this;
}

Array::Array(Array&& other) noexcept : data(other.data)
    , size(other.size), capacity((other.capacity)) {
    other.data = nullptr;
    other.size = 0;
    other.capacity = 0;
}

Array& Array::operator=(Array&& other) noexcept {
    if (this != &other) {
        clear();
        data = other.data;
        size = other.size;
        capacity = other.capacity;
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
    }
    return *this;
}

void Array::resize() {
    int new_cap = (capacity == 0) ? START_CAPACITY : capacity*2;
    std::string* new_data = new string[new_cap];
    for (int i = 0; i < size; ++i) {
        new_data[i] = data[i];
    }
    delete[] data;
    data = new_data;
    capacity = new_cap;
}

bool Array::empty() const{
    return size == 0;
}

void Array::clear() {
    delete[] data;
    data = nullptr;
    size = 0;
    capacity = 0;
}

void Array::push_back(const std::string str){
        if (size == capacity) 
            resize();
        data[size] = str;
        ++size;
}

bool Array::insert(int index, const std::string str){
    if (index < 0) return false;
    if (index >=size) {
        push_back(str);
        return true; }
    if (size == capacity) 
        resize();
    for (int i = size; i > index; --i) {
        data[i] = data[i-1];
    }
    data[index] = str;
    ++size;
    return true;
}

bool Array::removeAt(int index){
    if (index < 0 || index >=size)
        return false; 
    for (int i = index; i < size-1; ++i) {
        data[i] = data[i+1];
    }
    --size;
    return true;
}

int Array::getSize() const {
    return size;
}

bool Array::set(int index, const std::string& str) {
    if (index < 0 || index >= size) 
        return false;
    data[index] = str;
    return true;
}

std::string Array::getAt(int index) const {
    if (index < 0 || index >= size) 
        throw out_of_range("index out of range!");
    return data[index];
}

std::vector<std::string> Array::get() const {
    vector<string> result;
    result.reserve(size);
    for (int i = 0; i < size; ++i) {
        result.push_back(data[i]);
    }
    return result;
}

void Array::saveToFile(const std::string& filename) const {
    ofstream out(filename);
    if (!out.is_open()) throw std::runtime_error("can not open file "+ filename);

    out << size << "\n";
    for (int i = 0; i < size; ++i) {
        out << data[1] << "\n";
    }
}   


void Array::loadFromFile(const std::string& filename) {
    ifstream in(filename);
    if (!in.is_open()) throw std::runtime_error("can not open file " + filename);

    clear();
    int n;
    in >> n;
    in.ignore();
    for (int i = 0; i < n; ++i) {
        string line;
        getline(in, line);
        push_back(line);
    }
}

void Array::saveToBinar(const std::string& filename) const {
    ofstream out(filename, std::ios::binary);
    if (!out.is_open()) throw std::runtime_error("can not open file " + filename);

    out.write(reinterpret_cast<const char*>(&size), sizeof(int));
    for (int i = 0; i < size; ++i) {
        size_t len = data[i].size();
        out.write(reinterpret_cast<const char*>(&len), sizeof(size_t));
        out.write(data[i].data(), len);
    }
}

void Array::loadFromBinar(const std::string& filename) {
    ifstream in(filename, std::ios::binary);
    if (!in.is_open()) throw std::runtime_error("can not open file " + filename);

    clear();
    int n;
    in.read(reinterpret_cast<char*>(&n), sizeof(int));

    for (int i = 0; i < n; ++i) {
        size_t len;
        in.read(reinterpret_cast<char*>(&len), sizeof(size_t));
        string s(len, '\0');
        in.read(&s[0], len);
        push_back(s);
    }
}