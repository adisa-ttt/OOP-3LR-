#pragma once
#include <vector>
#include <string>

class Queue{
private:
    struct QNode {
        std::string data;
        QNode* next;
        QNode(const std::string& str, QNode* nxt);
    };
    QNode* head;
    QNode* tail;
public:
    Queue();
    ~Queue();

    Queue(const Queue& other);//
    Queue& operator=(const Queue& other);//
    Queue(Queue&& other) noexcept;//
    Queue& operator=(Queue&& other) noexcept;//

    bool empty() const;//
    void push(const std::string& str);//
    std::string pop();//
    std::vector<std::string> get() const;//
    void clear();//

    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    void saveToBinar(const std::string& filename) const;
    void loadFromBinar(const std::string& filename);
};
