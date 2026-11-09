#pragma once
#include <string>
#include<vector>

class LinkList {
private:
    struct FNode {
        std::string data;
        FNode* next;
        FNode(const std::string& str, FNode* nxt);
    };
    FNode* head;
    FNode* tail;
public:
    LinkList();
    ~LinkList();

    LinkList(const LinkList& other);
    LinkList& operator=(const LinkList& other);
    LinkList(LinkList&& other) noexcept;
    LinkList& operator=(LinkList&& other) noexcept;

    bool empty() const;//
    void clear();//
    void push_front(const std::string& str);//
    void push_back(const std::string& str);//
    std::string pop_front();//
    std::string pop_back();//

    std::string getAt(int index) const;//

    bool insertBeforeValue(const std::string& value, const std::string& str);//
    bool insertAfterValue(const std::string& value, const std::string& str);//
    bool removeBeforeValue(const std::string& value);//
    bool removeAfterValue(const std::string& value);//
    bool removeValue(const std::string& value);//

    int find(const std::string& value) const;//

    std::vector<std::string>get() const;//
    std::vector<std::string>getAfterValue(const std::string& value) const;//
    std::vector<std::string>getBeforeValue(const std::string& value) const;//
    bool getValue(const std::string& value, std::string& out) const;//

    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    void saveToBinar(const std::string& filename) const;
    void loadFromBinar(const std::string& filename);
};