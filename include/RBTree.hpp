#ifndef RBTREE_HPP
#define RBTREE_HPP

#include <cstddef>      // size_t
#include <climits>      // LLONG_MIN
#include <functional>   // std::function

enum class Color {
    RED,
    BLACK
};

template <typename Key, typename Value>
class RBTree {
public:
    struct Node {
        Key key;
        Value value;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        // Augmentation fields (shared across all subsystems)
        size_t subtreeSize;   // Student Records (CGPA tree)
        long long maxEnd;     // Event Scheduler
        size_t subtreeBooks;  // Library Management

        Node(const Key& k = Key{}, const Value& v = Value{})
            : key(k), value(v), color(Color::RED),
              left(nullptr), right(nullptr), parent(nullptr),
              subtreeSize(1), maxEnd(0), subtreeBooks(1) {}
    };

    RBTree();
    ~RBTree();

    void insert(const Key& key, const Value& value);
    bool remove(const Key& key);

    Node* find(const Key& key) const;
    Node* lowerBound(const Key& key) const;
    Node* upperBound(const Key& key) const;
    Node* front() const;
    Node* back() const;

    void inorder(std::function<void(const Key&, const Value&)> visit) const;

    size_t size() const { return nodeCount; }

protected:
    virtual void initializeNode(Node* node) {}  
    virtual void rotationHook(Node* node) {}     

    Node* NIL;
    Node* root;
    size_t nodeCount;

    void leftRotate(Node* x);
    void rightRotate(Node* x);
    void insertFixup(Node* z);
    void deleteFixup(Node* x);
    void transplant(Node* u, Node* v);
    Node* minimum(Node* x) const;
    Node* maximum(Node* x) const;

private:
    void destroyTree(Node* x);
};

#endif