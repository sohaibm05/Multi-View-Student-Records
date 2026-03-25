#pragma once
#include <functional>
#include <vector>

// Generic Red-Black Tree
// Built collectively by all team members

template <typename Key, typename Value>
class RBTree {
public:
    enum class Color { RED, BLACK };

    struct Node {
        Key key;
        Value value;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        Node(const Key& k, const Value& v)
            : key(k), value(v), color(Color::RED),
              left(nullptr), right(nullptr), parent(nullptr) {}
    };

    RBTree();
    ~RBTree();

    void insert(const Key& key, const Value& value);
    bool remove(const Key& key);
    Node* find(const Key& key) const;

    Node* lowerBound(const Key& key) const;
    Node* upperBound(const Key& key) const;

    void inorder(std::function<void(const Key&, const Value&)> visit) const;

    int size() const { return nodeCount; }
    bool empty() const { return nodeCount == 0; }

protected:
    Node* root;
    Node* NIL;
    int nodeCount;

    void leftRotate(Node* x);
    void rightRotate(Node* x);
    void insertFixup(Node* z);
    void deleteFixup(Node* x);
    void transplant(Node* u, Node* v);
    Node* minimum(Node* x) const;
    Node* maximum(Node* x) const;
    void destroyTree(Node* x);
};
