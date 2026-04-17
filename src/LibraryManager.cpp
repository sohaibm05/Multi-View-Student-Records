#include "LibraryManager.hpp"

void LibraryManager::addBook(const Book& b) {
    tree.insert(b.dueDate, b);
}

void LibraryTree::updateAvailability(Node* node, bool newStatus) {
    if (node == NIL || node == nullptr) return;
    node->value.isAvailable = newStatus;
    Node* cur = node;
    while (cur != NIL) {
        size_t self = cur->value.isAvailable ? 1 : 0;
        cur->subtreeBooks = self + cur->left->subtreeBooks + cur->right->subtreeBooks;
        cur = cur->parent;
    }
}

size_t LibraryTree::availableInRange(long long d1, long long d2) const {
    size_t cnt = 0;
    Node* x = root;
    while (x != NIL) {
        if (d2 < x->key) x = x->left;
        else if (d1 > x->key) x = x->right;
        else {
            if (x->value.isAvailable) cnt++;
            cnt += x->left->subtreeBooks;
            x = x->right;
        }
    }
    return cnt;
}