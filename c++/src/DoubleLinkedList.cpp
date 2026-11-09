#include "DoubleLinkedList.h"
#include <stdexcept>
#include <utility>
#include <fstream>
using namespace std;

doubleLinkList::LNode::LNode(const string& str, LNode* nxt, LNode* prv)
    : data(str), next(nxt), prev(prv) {}

doubleLinkList::doubleLinkList() : head(nullptr), tail(nullptr) {}

doubleLinkList::~doubleLinkList() {
    clear();
}

doubleLinkList::doubleLinkList(const doubleLinkList& other)
    : head(nullptr), tail(nullptr) {
    for (LNode* curr = other.head; curr != nullptr; curr = curr->next) {
        push_back(curr->data);
    }
}

doubleLinkList& doubleLinkList::operator=(const doubleLinkList& other) {
    if (this != &other) {
        doubleLinkList temp(other);
        swap(head, temp.head);
        swap(tail, temp.tail);
    }
    return *this;
}

doubleLinkList::doubleLinkList(doubleLinkList&& other) noexcept
    : head(other.head), tail(other.tail) {
    other.head = nullptr;
    other.tail = nullptr;
}

doubleLinkList& doubleLinkList::operator=(doubleLinkList&& other) noexcept {
    if (this != &other) {
        clear();
        head = other.head;
        tail = other.tail;
        other.head = nullptr;
        other.tail = nullptr;
    }
    return *this;
}

bool doubleLinkList::empty() const {
    return head == nullptr;
}

void doubleLinkList::clear() {
    while (head != nullptr) {
        LNode* popped = head;
        head = popped->next;
        delete popped;
    }
    tail = nullptr;
}

void doubleLinkList::push_front(const string& str) {
    LNode* new_LNode = new LNode(str, head, nullptr);
    if (head == nullptr) {
        tail = new_LNode; } 
    else {
        head->prev = new_LNode; }
    head = new_LNode;
}

void doubleLinkList::push_back(const string& str) {
    LNode* new_LNode = new LNode(str, nullptr, tail);
    if (tail == nullptr) {
        head = new_LNode;
    } else {
        tail->next = new_LNode;
    }
    tail = new_LNode;
}

string doubleLinkList::pop_front() {
    if (empty()) {
        throw out_of_range("double linked list is empty!");
    }
    LNode* popped = head;
    string data = popped->data;
    head = popped->next;
    if (head == nullptr) {
        tail = nullptr;
    } else {
        head->prev = nullptr;
    }
    delete popped;
    return data;
}

string doubleLinkList::pop_back() {
    if (empty()) {
        throw out_of_range("double linked list is empty!");
    }
    LNode* popped = tail;
    string data = popped->data;
    tail = popped->prev;
    if (tail == nullptr) {
        head = nullptr;
    } else {
        tail->next = nullptr;
    }
    delete popped;
    return data;
}

string doubleLinkList::getAt(int index) const {
    if (index < 0 || empty()) {
        throw out_of_range("index can not be smaller than 0!");
    }
    LNode* curr = head;
    int n = 0;
    while (n != index && curr != nullptr) {
        curr = curr->next;
        ++n;
    }
    if (curr == nullptr) {
        throw out_of_range("index can not be bigger than list size!");
    }
    return curr->data;
}

bool doubleLinkList::insertBeforeValue(const string& value, const string& str) {
    if (empty()) return false;
    LNode* value_LNode = head;
    while (value_LNode != nullptr && value_LNode->data != value) {
        value_LNode = value_LNode->next;
    }
    if (value_LNode == nullptr) return false;
    if (value_LNode == head) {
        push_front(str);
        return true;
    }
    LNode* left = value_LNode->prev;
    LNode* new_LNode = new LNode(str, value_LNode, left);
    left->next = new_LNode;
    value_LNode->prev = new_LNode;
    return true;
}

bool doubleLinkList::insertAfterValue(const string& value, const string& str) {
    if (empty()) return false;
    LNode* value_LNode = head;
    while (value_LNode != nullptr && value_LNode->data != value) {
        value_LNode = value_LNode->next;
    }
    if (value_LNode == nullptr) return false;
    if (value_LNode == tail) {
        push_back(str);
        return true;
    }
    LNode* right = value_LNode->next;
    LNode* new_LNode = new LNode(str, right, value_LNode);
    value_LNode->next = new_LNode;
    right->prev = new_LNode;
    return true;
}

bool doubleLinkList::removeValue(const string& value) {
    if (empty()) return false;
    LNode* value_LNode = head;
    while (value_LNode != nullptr && value_LNode->data != value) {
        value_LNode = value_LNode->next;
    }
    if (value_LNode == nullptr) return false;
    if (value_LNode == head) {
        pop_front();
        return true;
    }
    if (value_LNode == tail) {
        pop_back();
        return true;
    }
    LNode* left = value_LNode->prev;
    LNode* right = value_LNode->next;
    left->next = right;
    right->prev = left;
    delete value_LNode;
    return true;
}

bool doubleLinkList::removeBeforeValue(const string& value) {
    if (empty() || head->data == value) return false;
    LNode* value_LNode = head;
    while (value_LNode != nullptr && value_LNode->data != value) {
        value_LNode = value_LNode->next;
    }
    if (value_LNode == nullptr) return false;
    LNode* to_remove = value_LNode->prev;
    if (to_remove == head) {
        head = to_remove->next;
        if (head == nullptr) {
            tail = nullptr;
        } else {
            head->prev = nullptr;
        }
        delete to_remove;
        return true;
    }
    LNode* left = to_remove->prev;
    LNode* right = to_remove->next;
    left->next = right;
    right->prev = left;
    delete to_remove;
    return true;
}

bool doubleLinkList::removeAfterValue(const string& value) {
    if (empty()) return false;
    LNode* value_LNode = head;
    while (value_LNode != nullptr && value_LNode->data != value) {
        value_LNode = value_LNode->next;
    }
    if (value_LNode == nullptr || value_LNode->next == nullptr) return false;
    LNode* to_remove = value_LNode->next;
    if (to_remove == tail) {
        tail = to_remove->prev;
        if (tail == nullptr) {
            head = nullptr;
        } else {
            tail->next = nullptr;
        }
        delete to_remove;
        return true;
    }
    LNode* left = to_remove->prev;
    LNode* right = to_remove->next;
    left->next = right;
    right->prev = left;
    delete to_remove;
    return true;
}

int doubleLinkList::find(const string& value) const {
    int n = 0;
    for (LNode* c = head; c != nullptr; c = c->next, ++n) {
        if (c->data == value) return n;
    }
    return -1;
}

vector<string> doubleLinkList::get() const {
    vector<string> result;
    for (LNode* curr = head; curr != nullptr; curr = curr->next) {
        result.push_back(curr->data);
    }
    return result;
}

vector<string> doubleLinkList::getReverse() const {
    vector<string> result;
    for (LNode* curr = tail; curr != nullptr; curr = curr->prev) {
        result.push_back(curr->data);
    }
    return result;
}

vector<string> doubleLinkList::getAfterValue(const string& value) const {
    vector<string> result;
    LNode* value_LNode = head;
    while (value_LNode != nullptr && value_LNode->data != value) {
        value_LNode = value_LNode->next;
    }
    if (value_LNode == nullptr) return result;
    for (LNode* c = value_LNode->next; c != nullptr; c = c->next) {
        result.push_back(c->data);
    }
    return result;
}

vector<string> doubleLinkList::getBeforeValue(const string& value) const {
    vector<string> result;
    LNode* curr = head;
    while (curr != nullptr && curr->data != value) {
        result.push_back(curr->data);
        curr = curr->next;
    }
    if (curr == nullptr) return {};
    return result;
}

bool doubleLinkList::getValue(const string& value, string& out) const {
    LNode* value_LNode = head;
    while (value_LNode != nullptr && value_LNode->data != value) {
        value_LNode = value_LNode->next;
    }
    if (value_LNode == nullptr) return false;
    out = value_LNode->data;
    return true;
}

void doubleLinkList::saveToFile(const string& filename) const {
    ofstream out(filename);
    if (!out.is_open()) throw runtime_error("can not open file " + filename);

    vector<string> elems = get();
    out << elems.size() << "\n";
    for (const auto& e : elems) {
        out << e << "\n";
    }
}

void doubleLinkList::loadFromFile(const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) throw runtime_error("can not open file " + filename);

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

void doubleLinkList::saveToBinar(const string& filename) const {
    ofstream out(filename, ios::binary);
    if (!out.is_open()) throw runtime_error("can not open file " + filename);

    vector<string> elems = get();
    int n = elems.size();
    out.write(reinterpret_cast<const char*>(&n), sizeof(int));
    for (const auto& e : elems) {
        size_t len = e.size();
        out.write(reinterpret_cast<const char*>(&len), sizeof(size_t));
        out.write(e.data(), len);
    }
}

void doubleLinkList::loadFromBinar(const string& filename) {
    ifstream in(filename, ios::binary);
    if (!in.is_open()) throw runtime_error("can not open file " + filename);
    
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