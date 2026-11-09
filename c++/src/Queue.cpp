#include <stdexcept>
#include "Queue.h"
#include <vector>
#include <fstream>
using namespace std;

Queue::QNode::QNode(const string& str, QNode* nxt) 
    : data(str), next(nxt) {}

Queue::Queue() : head(nullptr), tail(nullptr) {}

Queue::~Queue() { clear(); }

Queue::Queue(const Queue& other) : head(nullptr), tail(nullptr) {
    QNode* curr = other.head;
    while (curr!=nullptr) {
        push(curr->data);
        curr = curr->next;
    }
}

Queue& Queue::operator=(const Queue& other) {
    if (this != &other) {
        Queue temp(other);
            swap(head, temp.head);
            swap(tail, temp.tail);
        }
    return *this;
}

Queue::Queue(Queue&& other) noexcept : head(other.head), tail(other.tail) {
    other.head = nullptr;
    other.tail = nullptr;
}

Queue& Queue::operator=(Queue&& other) noexcept {
    if (this != &other) {
        clear();
        head = other.head;
        tail = other.tail;
        other.head = nullptr;
        other.tail = nullptr;
    }
    return *this;
}

bool Queue::empty() const {
    return head == nullptr;
}

void Queue::push(const string& str) {
    QNode* new_node = new QNode(str, nullptr);
    if (tail == nullptr) {
        head = new_node;
        tail = new_node; }
    else {
        tail->next = new_node;
        tail = new_node; }
}

string Queue::pop() {
    if (empty()) {
        throw out_of_range("queue is empty!"); }
    QNode* old_head = head;
    string data = old_head->data;
    head = head->next;
    if (head ==nullptr) {
        tail = nullptr; }
    delete old_head;
    return data;
}

vector<string> Queue::get() const {
    vector<string> result;
    QNode* curr = head;
    while (curr != nullptr) {
        result.push_back(curr->data);
        curr = curr->next;
    }
    return result;
}

void Queue::clear() {
    while (head != nullptr) {
        QNode* popped = head;
        head = head->next;
        delete popped;
    }
    tail = nullptr;
}


void Queue::saveToFile(const std::string& filename) const {
    ofstream out(filename);
    if (!out.is_open()) throw std::runtime_error("can not open file " + filename);

    vector<string> elems = get();
    out << elems.size() << "\n";
    for (const auto& e : elems) {
        out << e << "\n";
    }
}

void Queue::loadFromFile(const std::string& filename) {
    ifstream in(filename);
    if (!in.is_open()) throw std::runtime_error("can not open file " + filename);

    clear();
    int n;
    in >> n;
    in.ignore();
    
    for (int i = 0; i < n; ++i) {
        string line;
        getline(in, line);
        push(line);
    }
}

void Queue::saveToBinar(const std::string& filename) const {
ofstream out(filename, std::ios::binary);
    if (!out.is_open()) throw std::runtime_error("can not open file " + filename);

    vector<string> elems = get();
    int n = elems.size();
    out.write(reinterpret_cast<const char*>(&n), sizeof(int));
    for (const auto& e : elems) {
        size_t len = e.size();
        out.write(reinterpret_cast<const char*>(&len), sizeof(size_t));
        out.write(e.data(), len);
    }
}

void Queue::loadFromBinar(const std::string& filename) {
    ifstream in(filename, std::ios::binary);
    if (!in.is_open()) throw std::runtime_error("can not open file " + filename);

    clear();
    int n;
    in.read(reinterpret_cast<char*>(&n), sizeof(int));
    
    for (int i = 0; i < n; ++i) {
        size_t len;
        in.read(reinterpret_cast<char*>(&len), sizeof(size_t));
        std::string s(len, '\0');
        in.read(&s[0], len);
        push(s);
    }
}