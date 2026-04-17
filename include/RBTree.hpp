// =====================================================
// include/RBTree.hpp
// =====================================================

#ifndef RBTREE_HPP
#define RBTREE_HPP

#include <cstddef>
#include <climits>
#include <functional>
#include <algorithm>

enum class Color { RED, BLACK };

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

        size_t subtreeSize = 1;
        long long maxEnd = 0;
        size_t subtreeBooks = 1;

        Node(const Key& k = Key{}, const Value& v = Value{})
            : key(k), value(v), color(Color::RED),
              left(nullptr), right(nullptr), parent(nullptr) {}
    };

    RBTree();
    virtual ~RBTree();

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

    Node* NIL = nullptr;
    Node* root = nullptr;
    size_t nodeCount = 0;

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

// ====================== FULL IMPLEMENTATION ======================

template <typename Key, typename Value>
RBTree<Key, Value>::RBTree() {
    NIL = new Node();
    NIL->color        = Color::BLACK;
    NIL->left         = NIL;
    NIL->right        = NIL;
    NIL->parent       = NIL;
    NIL->subtreeSize  = 0;
    NIL->maxEnd       = LLONG_MIN;
    NIL->subtreeBooks = 0;
    root              = NIL;
}

template <typename Key, typename Value>
RBTree<Key, Value>::~RBTree() {
    destroyTree(root);
    delete NIL;
}

template <typename Key, typename Value>
void RBTree<Key, Value>::destroyTree(Node* x) {
    if (x == NIL) return;
    destroyTree(x->left);
    destroyTree(x->right);
    delete x;
}

template <typename Key, typename Value>
void RBTree<Key, Value>::leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != NIL) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NIL) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
    rotationHook(x);
    rotationHook(y);
}

template <typename Key, typename Value>
void RBTree<Key, Value>::rightRotate(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (y->right != NIL) y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == NIL) root = y;
    else if (x == x->parent->right) x->parent->right = y;
    else x->parent->left = y;
    y->right = x;
    x->parent = y;
    rotationHook(x);
    rotationHook(y);
}

template <typename Key, typename Value>
void RBTree<Key, Value>::insert(const Key& key, const Value& value) {
    Node* parent = NIL;
    Node* current = root;
    while (current != NIL) {
        parent = current;
        if (key == current->key) {
            current->value = value;
            return;
        } else if (key < current->key)
            current = current->left;
        else
            current = current->right;
    }

    Node* z = new Node(key, value);
    z->left = NIL;
    z->right = NIL;
    z->color = Color::RED;
    z->parent = parent;

    if (parent == NIL)
        root = z;
    else if (key < parent->key)
        parent->left = z;
    else
        parent->right = z;

    ++nodeCount;
    initializeNode(z);
    insertFixup(z);
}

template <typename Key, typename Value>
void RBTree<Key, Value>::insertFixup(Node* z) {
    while (z->parent->color == Color::RED) {
        if (z->parent == z->parent->parent->left) {
            Node* uncle = z->parent->parent->right;
            if (uncle->color == Color::RED) {
                z->parent->color = Color::BLACK;
                uncle->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) { z = z->parent; leftRotate(z); }
                z->parent->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                rightRotate(z->parent->parent);
            }
        } else {
            Node* uncle = z->parent->parent->left;
            if (uncle->color == Color::RED) {
                z->parent->color = Color::BLACK;
                uncle->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) { z = z->parent; rightRotate(z); }
                z->parent->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                leftRotate(z->parent->parent);
            }
        }
    }
    root->color = Color::BLACK;
}

template <typename Key, typename Value>
bool RBTree<Key, Value>::remove(const Key& key) {
    // Find the node to delete
    Node* z = root;
    while (z != NIL) {
        if      (key == z->key) break;
        else if (key  < z->key) z = z->left;
        else                    z = z->right;
    }
    if (z == NIL) return false;    // key not found

    Node* y = z;                   // y = node to splice out
    Node* x;                       // x = node that moves into y's place
    Color yOriginalColor = y->color;

    if (z->left == NIL) {          // Case A: no left child
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == NIL) {  // Case B: no right child
        x = z->left;
        transplant(z, z->left);
    } else {                       // Case C: two children — use in-order successor
        y = minimum(z->right);     // leftmost node in right subtree
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;         // x may be NIL sentinel; safe to update its parent
        } else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;       // successor inherits deleted node's colour
    }

    delete z;
    --nodeCount;

    if (yOriginalColor == Color::BLACK)  // removing a BLACK node breaks black-height
        deleteFixup(x);

    return true;
}

template <typename Key, typename Value>
void RBTree<Key, Value>::deleteFixup(Node* x) {
    while (x != root && x->color == Color::BLACK) {
        if (x == x->parent->left) {          // x is a left child
            Node* w = x->parent->right;      // w is x's sibling

            if (w->color == Color::RED) {
                // Case 1: sibling RED
                w->color           = Color::BLACK;
                x->parent->color   = Color::RED;
                leftRotate(x->parent);
                w = x->parent->right;        // new sibling after rotation
            }

            if (w->left->color == Color::BLACK && w->right->color == Color::BLACK) {
                // Case 2: sibling BLACK, both nephews BLACK
                w->color = Color::RED;
                x = x->parent;
            } else {
                if (w->right->color == Color::BLACK) {
                    // Case 3: sibling BLACK, far nephew BLACK, near nephew RED
                    w->left->color = Color::BLACK;
                    w->color       = Color::RED;
                    rightRotate(w);
                    w = x->parent->right;
                }
                // Case 4: sibling BLACK, far nephew RED
                w->color           = x->parent->color;
                x->parent->color   = Color::BLACK;
                w->right->color    = Color::BLACK;
                leftRotate(x->parent);
                x = root;                    // done
            }
        } else {                             // x is a right child (mirror)
            Node* w = x->parent->left;

            if (w->color == Color::RED) {
                // Case 1 (mirror)
                w->color           = Color::BLACK;
                x->parent->color   = Color::RED;
                rightRotate(x->parent);
                w = x->parent->left;
            }

            if (w->right->color == Color::BLACK && w->left->color == Color::BLACK) {
                // Case 2 (mirror)
                w->color = Color::RED;
                x = x->parent;
            } else {
                if (w->left->color == Color::BLACK) {
                    // Case 3 (mirror)
                    w->right->color = Color::BLACK;
                    w->color        = Color::RED;
                    leftRotate(w);
                    w = x->parent->left;
                }
                // Case 4 (mirror)
                w->color           = x->parent->color;
                x->parent->color   = Color::BLACK;
                w->left->color     = Color::BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = Color::BLACK;    // absorb any remaining double-black
}

template <typename Key, typename Value>
void RBTree<Key, Value>::transplant(Node* u, Node* v) {
    if (u->parent == NIL)          // u was root → v becomes new root
        root = v;
    else if (u == u->parent->left) // u was left child
        u->parent->left = v;
    else                           // u was right child
        u->parent->right = v;
    v->parent = u->parent;         // always update v's parent (safe even if v == NIL)
}

template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::minimum(Node* x) const {
    if (x == NIL) return nullptr;
    while (x->left != NIL)
        x = x->left;
    return x;
}

template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::maximum(Node* x) const {
    if (x == NIL) return nullptr;
    while (x->right != NIL)
        x = x->right;
    return x;
}

template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::find(const Key& key) const {
    Node* current = root;
    while (current != NIL) {
        if (key == current->key)
            return current;
        else if (key < current->key)
            current = current->left;
        else
            current = current->right;
    }
    return nullptr;
}

template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::lowerBound(const Key& key) const {
    Node* result  = nullptr;
    Node* current = root;
    while (current != NIL) {
        if (current->key >= key) {
            result  = current;   // current is a candidate (key >= target)
            current = current->left;  // try to find an earlier one
        } else {
            current = current->right; // too small, go right
        }
    }
    return result;
}

template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::upperBound(const Key& key) const {
    Node* result  = nullptr;
    Node* current = root;
    while (current != NIL) {
        if (current->key > key) {
            result  = current;   // candidate (key strictly greater)
            current = current->left;  // try to find an earlier one
        } else {
            current = current->right; // not strictly greater, go right
        }
    }
    return result;
}

template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::front() const {
    return minimum(root);
}

template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::back() const {
    return maximum(root);
}

template <typename Key, typename Value>
void RBTree<Key, Value>::inorder(std::function<void(const Key&, const Value&)> visit) const {
    std::function<void(Node*)> helper = [&](Node* x) {
        if (x == NIL) return;
        helper(x->left);
        visit(x->key, x->value);
        helper(x->right);
    };
    helper(root);
}

#endif