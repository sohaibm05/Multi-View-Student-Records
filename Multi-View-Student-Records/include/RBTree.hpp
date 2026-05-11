#ifndef UNICORE_RBTREE_HPP
#define UNICORE_RBTREE_HPP

#include <functional>
#include <vector>
#include <stdexcept>
#include <utility>

enum class Color { RED, BLACK };

template <typename Key, typename Value>
struct RBNode {
    Key key;
    Value value;
    Color color;
    RBNode* left;
    RBNode* right;
    RBNode* parent;

    // Generic augmentation fields used by different modules.
    int subtreeSize;
    long long maxEnd;
    int subtreeBooks;

    RBNode() : color(Color::BLACK), left(nullptr), right(nullptr), parent(nullptr),
               subtreeSize(0), maxEnd(0), subtreeBooks(0) {}

    RBNode(const Key& k, const Value& v)
        : key(k), value(v), color(Color::RED), left(nullptr), right(nullptr), parent(nullptr),
          subtreeSize(1), maxEnd(0), subtreeBooks(0) {}
};

template <typename Key, typename Value>
struct DefaultAugment {
    void update(RBNode<Key, Value>* node, RBNode<Key, Value>* nil) const {
        if (node == nil) return;
        node->subtreeSize = node->left->subtreeSize + node->right->subtreeSize + 1;
        node->maxEnd = 0;
        node->subtreeBooks = 0;
    }
};

template <typename Key, typename Value,
          typename Compare = std::less<Key>,
          typename Augment = DefaultAugment<Key, Value> >
class RBTree {
public:
    using Node = RBNode<Key, Value>;

    RBTree() : comp(Compare()), augment(Augment()) {
        nil = new Node();
        nil->color = Color::BLACK;
        nil->left = nil;
        nil->right = nil;
        nil->parent = nil;
        root = nil;
    }

    ~RBTree() {
        clear(root);
        delete nil;
    }

    RBTree(const RBTree&) = delete;
    RBTree& operator=(const RBTree&) = delete;

    Node* getNil() const { return nil; }
    Node* getRoot() const { return root; }
    bool empty() const { return root == nil; }
    int size() const { return root->subtreeSize; }

    bool insert(const Key& key, const Value& value) {
        Node* parent = nil;
        Node* cur = root;

        while (cur != nil) {
            parent = cur;
            if (equalKey(key, cur->key)) return false;
            if (comp(key, cur->key)) cur = cur->left;
            else cur = cur->right;
        }

        Node* node = new Node(key, value);
        node->left = nil;
        node->right = nil;
        node->parent = parent;
        augment.update(node, nil);

        if (parent == nil) root = node;
        else if (comp(key, parent->key)) parent->left = node;
        else parent->right = node;

        updateUpwards(parent);
        insertFixup(node);
        updateUpwards(node);
        return true;
    }

    bool erase(const Key& key) {
        Node* z = findNode(key);
        if (z == nil) return false;
        eraseNode(z);
        return true;
    }

    Node* findNode(const Key& key) const {
        Node* cur = root;
        while (cur != nil) {
            if (equalKey(key, cur->key)) return cur;
            if (comp(key, cur->key)) cur = cur->left;
            else cur = cur->right;
        }
        return nil;
    }

    Value* find(const Key& key) {
        Node* node = findNode(key);
        if (node == nil) return nullptr;
        return &node->value;
    }

    const Value* find(const Key& key) const {
        Node* node = findNode(key);
        if (node == nil) return nullptr;
        return &node->value;
    }

    bool contains(const Key& key) const { return findNode(key) != nil; }

    Node* minimum(Node* node) const {
        if (node == nil) return nil;
        while (node->left != nil) node = node->left;
        return node;
    }

    Node* maximum(Node* node) const {
        if (node == nil) return nil;
        while (node->right != nil) node = node->right;
        return node;
    }

    Node* lowerBoundNode(const Key& key) const {
        Node* cur = root;
        Node* ans = nil;
        while (cur != nil) {
            if (!comp(cur->key, key)) {
                ans = cur;
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        }
        return ans;
    }

    Node* upperBoundNode(const Key& key) const {
        Node* cur = root;
        Node* ans = nil;
        while (cur != nil) {
            if (comp(key, cur->key)) {
                ans = cur;
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        }
        return ans;
    }

    std::vector<Value> inorderValues() const {
        std::vector<Value> result;
        inorderValues(root, result);
        return result;
    }

    template <typename Func>
    void inorder(Func fn) const { inorder(root, fn); }

    void updateNode(Node* node) { augment.update(node, nil); }
    void updateUpwards(Node* node) {
        while (node != nil) {
            augment.update(node, nil);
            node = node->parent;
        }
    }

    Value popMinimum() {
        if (root == nil) throw std::runtime_error("Cannot pop from empty RBTree");
        Node* node = minimum(root);
        Value val = node->value;
        eraseNode(node);
        return val;
    }

    bool validateRBTree() const {
        if (root == nil) return true;
        if (root->color != Color::BLACK) return false;
        int blackHeight = -1;
        return validateNode(root, 0, blackHeight);
    }

private:
    Node* root;
    Node* nil;
    Compare comp;
    Augment augment;

    bool equalKey(const Key& a, const Key& b) const {
        return !comp(a, b) && !comp(b, a);
    }

    void clear(Node* node) {
        if (node == nil) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != nil) y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == nil) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;

        augment.update(x, nil);
        augment.update(y, nil);
        updateUpwards(y->parent);
    }

    void rightRotate(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        if (x->right != nil) x->right->parent = y;
        x->parent = y->parent;
        if (y->parent == nil) root = x;
        else if (y == y->parent->right) y->parent->right = x;
        else y->parent->left = x;
        x->right = y;
        y->parent = x;

        augment.update(y, nil);
        augment.update(x, nil);
        updateUpwards(x->parent);
    }

    void insertFixup(Node* z) {
        while (z->parent->color == Color::RED) {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;
                if (y->color == Color::RED) {
                    z->parent->color = Color::BLACK;
                    y->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    rightRotate(z->parent->parent);
                }
            } else {
                Node* y = z->parent->parent->left;
                if (y->color == Color::RED) {
                    z->parent->color = Color::BLACK;
                    y->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = Color::BLACK;
        updateUpwards(z);
    }

    void transplant(Node* u, Node* v) {
        if (u->parent == nil) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        v->parent = u->parent;
    }

    void eraseNode(Node* z) {
        Node* y = z;
        Node* x;
        Color yOriginalColor = y->color;
        Node* updateStart = z->parent;

        if (z->left == nil) {
            x = z->right;
            updateStart = z->parent;
            transplant(z, z->right);
        } else if (z->right == nil) {
            x = z->left;
            updateStart = z->parent;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z) {
                x->parent = y;
                updateStart = y;
            } else {
                updateStart = y->parent;
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
            augment.update(y, nil);
        }

        delete z;
        updateUpwards(updateStart);
        if (yOriginalColor == Color::BLACK) deleteFixup(x);
        if (root != nil) root->color = Color::BLACK;
        updateUpwards(x);
    }

    void deleteFixup(Node* x) {
        while (x != root && x->color == Color::BLACK) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;
                if (w->color == Color::RED) {
                    w->color = Color::BLACK;
                    x->parent->color = Color::RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == Color::BLACK && w->right->color == Color::BLACK) {
                    w->color = Color::RED;
                    x = x->parent;
                } else {
                    if (w->right->color == Color::BLACK) {
                        w->left->color = Color::BLACK;
                        w->color = Color::RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = Color::BLACK;
                    w->right->color = Color::BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                Node* w = x->parent->left;
                if (w->color == Color::RED) {
                    w->color = Color::BLACK;
                    x->parent->color = Color::RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == Color::BLACK && w->left->color == Color::BLACK) {
                    w->color = Color::RED;
                    x = x->parent;
                } else {
                    if (w->left->color == Color::BLACK) {
                        w->right->color = Color::BLACK;
                        w->color = Color::RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = Color::BLACK;
                    w->left->color = Color::BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = Color::BLACK;
    }

    void inorderValues(Node* node, std::vector<Value>& result) const {
        if (node == nil) return;
        inorderValues(node->left, result);
        result.push_back(node->value);
        inorderValues(node->right, result);
    }

    template <typename Func>
    void inorder(Node* node, Func fn) const {
        if (node == nil) return;
        inorder(node->left, fn);
        fn(node);
        inorder(node->right, fn);
    }

    bool validateNode(Node* node, int blackCount, int& expectedBlackHeight) const {
        if (node == nil) {
            if (expectedBlackHeight == -1) expectedBlackHeight = blackCount;
            return expectedBlackHeight == blackCount;
        }
        if (node->color == Color::BLACK) blackCount++;
        if (node->color == Color::RED) {
            if (node->left->color == Color::RED || node->right->color == Color::RED) return false;
        }
        if (node->left != nil && comp(node->key, node->left->key)) return false;
        if (node->right != nil && comp(node->right->key, node->key)) return false;
        return validateNode(node->left, blackCount, expectedBlackHeight) &&
               validateNode(node->right, blackCount, expectedBlackHeight);
    }
};

#endif
