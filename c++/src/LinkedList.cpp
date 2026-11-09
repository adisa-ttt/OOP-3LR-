#include <stdexcept>
#include "LinkedList.h"
#include <fstream>
using namespace std;

LinkList::FNode::FNode(const string& str, FNode* nxt) 
    : data(str), next(nxt) {}

LinkList::LinkList() : head(nullptr), tail(nullptr) {}

LinkList::~LinkList() { clear(); }

LinkList::LinkList(const LinkList& other) : head(nullptr), tail(nullptr) {
    for (FNode* curr = other.head; curr != nullptr; curr = curr->next) {
        push_back(curr->data);
    }
}

LinkList& LinkList::operator=(const LinkList& other) {
    if (this != &other) {
        LinkList temp(other);
        swap(head, temp.head);
        swap(tail, temp.tail);
    }
    return *this;
}

LinkList::LinkList(LinkList&& other) noexcept 
    : head(other.head), tail(other.tail) {
        other.head = nullptr;
        other.tail = nullptr;
}

LinkList& LinkList::operator=(LinkList&& other) noexcept {
    if(this != &other) {
        clear();
        head = other.head;
        tail = other.tail;
        other.head = nullptr;
        other.tail = nullptr;
    }
    return *this;
}

bool LinkList::empty() const {
    return head == nullptr;
}

void LinkList::clear() {
    while (head != nullptr) {
        FNode* popped = head;
        head = head->next;
        delete popped;
    }
    tail = nullptr;
}

void LinkList::push_front(const string& str) {
    FNode* new_node = new FNode(str, head);
    if (head == nullptr) {
        tail = new_node; }
    head = new_node;
}

void LinkList::push_back(const string& str) {
    FNode* new_node = new FNode(str, nullptr);
    if (head == nullptr) {
        head = new_node;
        tail = new_node; }
    else {
        tail->next = new_node;
        tail = new_node; }
}

string LinkList::pop_front() {
    if (empty()) {
        throw out_of_range("list is empty!"); }
    FNode* popped = head;
    string data = popped->data;
    head = head->next;
    if (head == nullptr) 
        tail = nullptr;
    delete popped;
    return data;
}

string LinkList::pop_back() {
    if (empty()) {
        throw out_of_range("list is empty!"); }
    if (head == tail) {
        string data = head->data;
        delete head;
        head = nullptr;
        tail = nullptr;
        return data; }
    FNode* prev_tail = head;
    while (prev_tail->next != tail) {
        prev_tail = prev_tail->next; }
    string data = tail->data;
    delete tail;
    tail = prev_tail;
    tail->next = nullptr;
    return data;
}

string LinkList::getAt(int index) const {
    if (index < 0 ||empty()) {
        throw out_of_range("index can not be smaller than 0!"); }
    FNode* curr = head;
    int n = 0;
    while (n != index && curr != nullptr) {
        curr = curr->next;
        ++n; }
    if (curr == nullptr) {
        throw out_of_range("index can not be bigger than list size!"); }
    return curr->data;
}

bool LinkList::insertBeforeValue(const string& value, const string& str) {
    if (empty()) return false;
    if (head->data == value) {
        push_front(str);
        return true; }
    FNode* prev = head;
    FNode* curr = head->next;
    while (curr != nullptr && curr->data != value) {
        prev = curr;
        curr = curr->next; }
    if (curr ==  nullptr) return false;
    prev->next = new FNode(str, curr);
    return true;
}

bool LinkList::insertAfterValue(const string& value, const string& str) {
    if(empty()) return false;
    FNode* curr = head;
    while (curr != nullptr && curr->data != value) {
        curr = curr->next; }
    if (curr == nullptr) return false;
    FNode* new_node = new FNode(str, curr->next);
    curr->next = new_node;
    if (new_node->next ==nullptr) 
        tail = new_node;
    return true;
}

bool LinkList::removeValue(const string& value) {
    if (empty()) return false;
    if (head->data == value) {
        pop_front();
        return true; }
    FNode* prev = head;
    FNode* curr = head->next;
    while (curr != nullptr) {
        if (curr->data == value) {
            if (curr == tail) {
                pop_back();
                return true; }
            prev->next = curr->next;
            delete curr;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

bool LinkList::removeBeforeValue(const string& value) {
    if (empty() || head->data == value)return false;
    FNode* prev_prev = nullptr;
    FNode* prev = head;
    FNode* curr = head->next;
    while (curr != nullptr && curr->data != value) {
        prev_prev = prev;
        prev = curr;
        curr = curr->next;
    }
    if (curr == nullptr) return false;
    if (prev_prev == nullptr) {
        head = prev->next;
        delete prev;
        return true;
    }
    prev_prev->next = prev->next;
    delete prev;
    return true;
}

bool LinkList::removeAfterValue(const string& value) {
    if (empty()) return false;
    FNode* curr = head;
    while (curr != nullptr && curr->data != value) {
        curr = curr->next;
    }
    if (curr == nullptr || curr == tail) return false;
    FNode* to_delete = curr->next;
    if (to_delete == tail) {
        tail = curr;
        tail->next = nullptr;
        delete to_delete;
        return true;
    }
    curr->next = to_delete->next;
    delete to_delete;
    return true;
}

int LinkList::find(const std::string& value) const {
    int n = 0;
    for (FNode* c = head; c != nullptr; c = c->next, ++n) {
        if (c->data == value) return n;
    }
    return -1;
}

std::vector<std::string> LinkList::get() const {
    std::vector<std::string> result;
    for (FNode* curr = head; curr != nullptr; curr = curr->next) {
        result.push_back(curr->data);
    }
    return result;
}

std::vector<std::string> LinkList::getAfterValue(const std::string& value) const {
    std::vector<std::string> result;
    if (empty()) return result;
    FNode* curr = head;
    while (curr != nullptr && curr->data != value) {
        curr = curr->next;
    }
    if (curr == nullptr) return result;
    for (FNode* c = curr->next; c != nullptr; c = c->next) {
        result.push_back(c->data);
    }
    return result;
}

std::vector<std::string> LinkList::getBeforeValue(const std::string& value) const {
    std::vector<std::string> result;
    if (empty()) return result;
    FNode* curr = head;
    while (curr != nullptr && curr->data != value) {
        result.push_back(curr->data);
        curr = curr->next;
    }
    if (curr == nullptr) return {};
    return result;
}

bool LinkList::getValue(const std::string& value, std::string& out) const {
    FNode* curr = head;
    while (curr != nullptr && curr->data != value) {
        curr = curr->next;
    }
    if (curr == nullptr) return false;
    out = curr->data;
    return true;
}


void LinkList::saveToFile(const std::string& filename) const {
    ofstream out(filename);
    if (!out.is_open()) throw std::runtime_error("can not open file " + filename);

    vector<string> elems = get();
    out << elems.size() << "\n";
    for (const auto& e : elems) {
        out << e << "\n";
    }
}

void LinkList::loadFromFile(const std::string& filename) {
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

void LinkList::saveToBinar(const std::string& filename) const {
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

void LinkList::loadFromBinar(const std::string& filename) {
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
        push_back(s);
    }
}

