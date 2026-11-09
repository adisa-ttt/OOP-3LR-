#include <stdexcept>
#include "Stack.h"
#include <fstream>
#include <algorithm>
using namespace std;

Stack::Stack() : head(nullptr) {}
Stack::SNode::SNode(const string& str, SNode* nxt) : data(str), next(nxt) {}

Stack::~Stack() {
    clear();
}

Stack::Stack(const Stack& other) : head(nullptr) {
    SNode* cur = other.head;
    SNode* tail = nullptr;
    while (cur != nullptr) {
        SNode* new_node = new SNode(cur->data, nullptr);
        if (head == nullptr) {
            head = new_node;
            tail = new_node; }
        else {
            tail->next = new_node;
            tail = new_node; }
        cur = cur->next;
    }
}

Stack& Stack::operator=(const Stack& other) {
    if (this != &other) {
        Stack temp(other);
        std::swap(head, temp.head); }
    return *this;
}

Stack::Stack(Stack&& other) noexcept : head(other.head) {
    other.head = nullptr;
}

Stack& Stack::operator=(Stack&& other) noexcept {
    if (this != &other) {
        clear();
        head = other.head;
        other.head = nullptr;
    }
    return *this;
}

bool Stack::empty() const {
    return head == nullptr;
}

void Stack::push(const string& str) {
    head = new SNode(str, head);
}

string Stack::pop() {
    if (empty()) {
        throw out_of_range("stack is empty!"); 
    }
    SNode* old_head = head;
    string data = old_head->data;
    head = old_head->next;
    delete old_head;
    return data;
}

vector<string> Stack::get() const {
    vector<string> result;
    SNode* curr = head;
    while (curr!= nullptr) {
        result.push_back(curr->data);
        curr = curr->next;
    }
    return result;
}

void Stack::clear() {
    while (head != nullptr) {
        SNode* popped = head;
        head = head->next;
        delete popped;
    }
}

void Stack::saveToFile(const std::string& filename) const {
    ofstream out(filename);
    if (!out.is_open()) throw std::runtime_error("can not open file " + filename);

    vector<std::string> elems = get(); 
    reverse(elems.begin(), elems.end()); 
    out << elems.size() << "\n";
    for (const auto& e : elems) {
        out << e << "\n";
    }
}

void Stack::loadFromFile(const std::string& filename) {
    ifstream in(filename);
    if (!in.is_open()) throw std::runtime_error("can not open file " + filename);

    clear();
    int n;
    in >> n;
    in.ignore();
    vector<string> elems;
    for (int i = 0; i < n; ++i) {
        string line;
        getline(in, line);
        elems.push_back(line);
    }
    for (auto it = elems.rbegin(); it != elems.rend(); ++it) {
        push(*it);
    }
}

void Stack::saveToBinar(const std::string& filename) const {
    ofstream out(filename, std::ios::binary);
    if (!out.is_open()) throw std::runtime_error("can not open file " + filename);

    vector<string> elems = get();
    reverse(elems.begin(), elems.end());
    
    int n = elems.size();
    out.write(reinterpret_cast<const char*>(&n), sizeof(int));
    for (const auto& e : elems) {
        size_t len = e.size();
        out.write(reinterpret_cast<const char*>(&len), sizeof(size_t));
        out.write(e.data(), len);
    }
}

void Stack::loadFromBinar(const std::string& filename) {
    ifstream in(filename, std::ios::binary);
    if (!in.is_open()) throw std::runtime_error("can not open file " + filename);

    clear();
    int n;
    in.read(reinterpret_cast<char*>(&n), sizeof(int));
    
    vector<string> elems;
    for (int i = 0; i < n; ++i) {
        size_t len;
        in.read(reinterpret_cast<char*>(&len), sizeof(size_t));
        string s(len, '\0');
        in.read(&s[0], len);
        elems.push_back(s);
    }
    for (auto it = elems.rbegin(); it != elems.rend(); ++it) {
        push(*it);
    }
}