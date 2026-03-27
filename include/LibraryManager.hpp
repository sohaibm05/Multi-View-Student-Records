#pragma once
#include "RBTree.hpp"
#include "Book.hpp"
#include <vector>
#include <ctime>
#include <string>

// Member 3 — Library Manager (Saad)
// RBT keyed by due date; subtreeBooks counts available books per subtree
// subtreeBooks changes on checkout/return WITHOUT any tree rotation

class LibraryManager {
public:
    struct AugNode {
        Book book;
        int  subtreeBooks; // available (not checked out) books in subtree
        AugNode() : subtreeBooks(0) {}
        explicit AugNode(const Book& b)
            : book(b), subtreeBooks(b.checkedOut ? 0 : 1) {}
    };

    LibraryManager();

    void addBook(const Book& book);
    bool removeBook(const std::string& isbn);

    bool checkOut(const std::string& isbn);
    bool returnBook(const std::string& isbn);

    // How many available books have due dates in [d1, d2]
    int availableInRange(std::time_t d1, std::time_t d2) const;

    // All books past their due date (prunes branches where subtreeBooks == 0)
    std::vector<Book> overdueBooks(std::time_t today) const;

    // Fine in PKR based on days overdue
    double estimateFine(const Book& book, std::time_t today) const;

private:
    RBTree<std::time_t, AugNode> tree;
};
