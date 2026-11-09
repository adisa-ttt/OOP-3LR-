#pragma  once
#include <vector>
#include <string>

class avlTree {
private:
    struct Node {
        int key;
        Node* left;
        Node* right;
        unsigned char height;
        Node(int k);
    };
    Node* root;
    
    static unsigned char getHeight(Node* node);
    static int getBalanceFactor(Node* node);
    static void fixHeight(Node* node);
    static Node* rotateRight(Node* node);//
    static Node* rotateLeft(Node* node);//
    static Node* insert(Node* node, int key);
    static Node* remove(Node* node, int key);
    static bool find(Node* node, int key);
    static Node* findMin(Node* node);
    static void clear(Node* node);

    static void inorder(Node* node, std::vector<int>& result);
    static void preorder(Node* node, std::vector<int>& result);
    static void postorder(Node* node, std::vector<int>& result);
    static void bfs(Node* root, std::vector<int>& result);
public:
    avlTree();
    ~avlTree();
    
    avlTree(const avlTree& other);//
    avlTree& operator=(const avlTree& other);//
    avlTree(avlTree&& other) noexcept;//
    avlTree& operator=(avlTree&& other) noexcept;//

    bool empty() const;//
    void clear();//
    void insert(int key);//
    bool remove(int key);//
    bool find(int key) const;//
    std::vector<int> inorder() const;//
    std::vector<int> preorder() const;//
    std::vector<int> postorder() const;//
    std::vector<int> bfs() const;//

    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    void saveToBinar(const std::string& filename) const;
    void loadFromBinar(const std::string& filename);
};
