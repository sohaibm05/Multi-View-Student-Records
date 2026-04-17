#ifndef LIBRARYMANAGER_HPP
#define LIBRARYMANAGER_HPP

#include "RBTree.hpp"
#include <string>

struct Book {
    std::string title;
    long long dueDate;
    bool isAvailable;
};

class LibraryTree : public RBTree<long long, Book> {
public:
    virtual void initializeNode(Node* node) override {
        if (node != NIL)
            node->subtreeBooks = node->value.isAvailable ? 1 : 0;
    }
    virtual void rotationHook(Node* node) override {
        if (node == NIL) return;
        size_t self = node->value.isAvailable ? 1 : 0;
        node->subtreeBooks = self + node->left->subtreeBooks + node->right->subtreeBooks;
    }

    void updateAvailability(Node* node, bool newStatus); // non-rotation update
    size_t availableInRange(long long d1, long long d2) const;
};

class LibraryManager {
public:
    void addBook(const Book& b);
    LibraryTree tree;
};

#endif