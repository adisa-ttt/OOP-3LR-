#pragma once
#include <string>
#include <vector>

class Stack {
private:
    struct SNode {
        std::string data;
        SNode* next;
        SNode(const std::string& str, SNode* nxt);
    };
    SNode* head;

public:
    Stack();
    ~Stack();

    Stack(const Stack& other);//
    Stack& operator=(const Stack& other);//
    Stack(Stack&& other) noexcept;//
    Stack& operator=(Stack&& other) noexcept;//

    bool empty() const;//
    void push(const std::string& str);///
    std::string pop();//
    std::vector<std::string> get() const;//
    void clear();//

    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    void saveToBinar(const std::string& filename) const;
    void loadFromBinar(const std::string& filename);
};