#include "avlTree.h"
#include <fstream>
#include <utility>
#include <vector>
#include <queue>
using  namespace std;

avlTree::Node::Node(int k) : key(k), left(nullptr)
    , right(nullptr), height(1) {}

avlTree::avlTree() : root(nullptr) {}

avlTree::~avlTree() { clear(); }

avlTree::avlTree(const avlTree& other) : root(nullptr) {
    if (other.root != nullptr) {
        vector<int> values = other.preorder();
        for (int val : values) {
            insert(val);
        }
    }
}

avlTree& avlTree::operator=(const avlTree& other) {
    if (this != &other) {
        avlTree temp(other);
        swap(root, temp.root);
    }
    return *this;
}

avlTree::avlTree(avlTree&& other) noexcept : root(other.root) {
    other.root = nullptr;
}

avlTree& avlTree::operator=(avlTree&& other) noexcept {
    if (this != &other) {
        clear();
        root = other.root;
        other.root = nullptr;
    }
    return *this;
}

unsigned char avlTree::getHeight(Node* node) {
    return (node == nullptr) ? 0 : node->height;
}

int avlTree::getBalanceFactor(Node* node){
        //if (node == nullptr)  return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

void avlTree::fixHeight(Node* node) {
        unsigned char hL = getHeight(node->left);
        unsigned char hR = getHeight((node->right));
        node->height = (hL > hR ? hL : hR) + 1;
    }
    
avlTree::Node* avlTree::rotateRight(Node* node) {
        Node* temp = node->left;
        node->left = temp->right;
        temp->right = node;
        fixHeight(node);
        fixHeight(temp);
        return temp;
    }
    
avlTree::Node* avlTree::rotateLeft(Node* node) {
    Node* temp = node->right;
    node->right = temp->left;
    temp->left = node;
    fixHeight(node);
    fixHeight(temp);
    return temp;
}
    
avlTree::Node* avlTree::insert(Node* node, int key) {
    if (node == nullptr) return new Node(key);
    if (key < node->key) 
        node->left = insert(node->left, key);
    else if (key > node->key) 
        node->right = insert(node->right, key);
    else 
        return node;
    fixHeight(node);
    int bf = getBalanceFactor(node);
    if (bf == 2) {
        if (getBalanceFactor(node->left) >= 0) {
            return rotateRight(node); }
        else {
            node->left = rotateLeft(node->left);
            return rotateRight(node); }        
    }
    if (bf == -2) {
        if (getBalanceFactor(node->right) <= 0) {
            return rotateLeft(node); }
        else {
            node->right = rotateRight(node->right);
            return rotateLeft(node); }
    }
    return node;
}
    
avlTree::Node* avlTree::remove(Node* node, int key) {
    //if (node == nullptr) return nullptr;
    if (key < node->key) {
        node->left = remove(node->left, key); }
    else if (key > node->key) {
        node->right = remove(node->right, key); }
    else {
        if (node->left == nullptr) {
            Node* tmp = node->right;
            delete node;
            return tmp; }
        else if (node->right == nullptr) {
            Node* tmp = node->left;
            delete node;
            return tmp; }
        Node* min_node = findMin(node->right);
        node->key = min_node->key;
        node->right = remove(node->right, min_node->key);
    }
    fixHeight(node);
    int bf = getBalanceFactor(node);
    if (bf == 2) {
        if (getBalanceFactor(node->left) >= 0) {
            return rotateRight(node); }
        else {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
    }
    if (bf == -2) {
        if (getBalanceFactor(node->right) <= 0) {
            return rotateLeft(node); }
        else {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
    }
    return node;
}

bool avlTree::find(Node* node, int key) {
    if (node==nullptr) return false;
    if (key == node->key) return true;
    if (key < node->key) return find(node->left, key);
    return find(node->right, key);
}

avlTree::Node* avlTree::findMin(Node* node) {
    //if (node == nullptr) return nullptr;
    if (node->left == nullptr) return node;
    return findMin(node->left);
}

void avlTree::clear(Node* node) {
    if (node == nullptr) return;
    clear(node->left);
    clear(node->right);
    delete node;
}

void avlTree::inorder(Node* node, std::vector<int>& result) {
    if (node == nullptr) return;
    inorder(node->left, result);
    result.push_back(node->key);
    inorder(node->right, result);
}

void avlTree::preorder(Node* node, std::vector<int>& result) {
    if (node == nullptr) return;
    result.push_back(node->key);
    preorder(node->left, result);
    preorder(node->right, result);
}

void avlTree::postorder(Node* node, std::vector<int>& result) {
    if (node == nullptr) return;
    postorder(node->left, result);
    postorder(node->right, result);
    result.push_back(node->key);
}

void avlTree::bfs(Node* root, std::vector<int>& result) {
    if (root == nullptr) return;
    std::queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node* curr = q.front();
        q.pop();
        result.push_back(curr->key);
        if (curr->left != nullptr) q.push(curr->left);
        if (curr->right != nullptr) q.push(curr->right);
    }
}

bool avlTree::empty() const {
    return root == nullptr;
}

void avlTree::clear() {
    clear(root);
    root = nullptr;
}

void avlTree::insert(int key) {
    root = insert(root, key);
}

bool avlTree::remove(int key) {
    if (!find(root, key)) return false;
    root = remove(root, key);
    return true;
}

bool avlTree::find(int key) const {
    return find(root, key);
}

std::vector<int> avlTree::inorder() const {
    vector<int> result;
    inorder(root, result);
    return result;
}

std::vector<int> avlTree::preorder() const {
    vector<int> result;
    preorder(root, result);
    return result;
}

std::vector<int> avlTree::postorder() const {
    vector<int> result;
    postorder(root, result);
    return result;
}

std::vector<int> avlTree::bfs() const {
    vector<int> result;
    bfs(root, result);
    return result;
}

void avlTree::saveToFile(const std::string& filename) const {
    ofstream out(filename);
    if (!out.is_open()) throw std::runtime_error("can not open file " + filename);

    vector<int> elements = preorder();
    out << elements.size() << "\n";
    for (auto i : elements) {
        out << i << "\n";
    }
}

void avlTree::loadFromFile(const std::string& filename) {
    ifstream in(filename);
    if (!in.is_open()) throw std::runtime_error("can not open file " + filename);

    clear();
    int n;
    in >> n;
    for(int i = 0; i < n; ++i) {
        int el;
        in >> el;
        insert(el);
    }
}

void avlTree::saveToBinar(const std::string& filename) const {
    ofstream out(filename, std::ios::binary);
    if (!out.is_open()) throw std::runtime_error("can not open file " + filename);

    vector<int> els = preorder();
    int n = els.size();
    out.write(reinterpret_cast<const char*>(&n), sizeof(int));
    out.write(reinterpret_cast<const char*>(els.data()), n*sizeof(int));

}

void avlTree::loadFromBinar(const std::string& filename) {
    ifstream in(filename, std::ios::binary);
    if (!in.is_open()) throw std::runtime_error("can not open file " + filename);

    clear();
    int n;
    in.read(reinterpret_cast<char*>(&n), sizeof(int));
    vector<int> els(n);
    in.read(reinterpret_cast<char*>(els.data()), n*sizeof(int));
    for (auto i : els) insert(i);
}