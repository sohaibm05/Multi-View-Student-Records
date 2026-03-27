#pragma once
#include "RBTree.hpp"
#include "Book.hpp"
#include <vector>
#include <ctime>
#include <string>

// =============================================================================
// LibraryManager  —  availability-aware library tree  (Member 3 — Saad)
//
// Uses a RBTree<time_t, AugNode> keyed by book due date.
// Each node stores the book plus a subtreeBooks counter.
//
// AUGMENTATION — subtreeBooks
//   subtreeBooks(node) = (node.book.checkedOut ? 0 : 1)
//                       + subtreeBooks(node->left)
//                       + subtreeBooks(node->right)
//   subtreeBooks(NIL)  = 0
//
//   Unlike subtreeSize, this count changes WITHOUT any rotation:
//     • When a book is checked out or returned, flip book.checkedOut
//       then walk UP the parent chain to root, recomputing subtreeBooks
//       at each ancestor.  This is O(log n) propagation with zero rotations.
//
//   It ALSO changes during rotations (same as subtreeSize):
//     update the node that went DOWN first, then the one that went UP.
//
// This augmentation enables:
//   availableInRange — skip entire subtrees where subtreeBooks == 0
//   overdueBooks     — prune branches where subtreeBooks == 0 (no point
//                      descending into a subtree with no available books)
// =============================================================================

class LibraryManager {
public:
    struct AugNode {
        Book book;
        int  subtreeBooks; // # of available (not checked out) books in this subtree

        AugNode() : subtreeBooks(0) {}
        explicit AugNode(const Book& b)
            : book(b), subtreeBooks(b.checkedOut ? 0 : 1) {}
    };

    LibraryManager();

    // Insert book into the tree keyed by book.dueDate.
    // initialise subtreeBooks = (checkedOut ? 0 : 1) for the new node.
    void addBook(const Book& book);

    // Find the book by isbn (scan via inorder or maintain a side map),
    // then call tree.remove on its dueDate key.
    // Returns false if not found.
    bool removeBook(const std::string& isbn);

    // Mark the book as checkedOut = true.
    // After flipping the flag, walk up the parent chain and decrement
    // subtreeBooks by 1 at every ancestor.
    // Returns false if book not found or already checked out.
    bool checkOut(const std::string& isbn);

    // Mark the book as checkedOut = false.
    // After flipping the flag, walk up the parent chain and increment
    // subtreeBooks by 1 at every ancestor.
    // Returns false if book not found or not currently checked out.
    bool returnBook(const std::string& isbn);

    // Count available books whose dueDate falls in [d1, d2].
    // Use lowerBound(d1) as start, walk while key <= d2.
    // Skip subtrees where subtreeBooks == 0 (no available books there).
    // O(log n) on average.
    int availableInRange(std::time_t d1, std::time_t d2) const;

    // Return all books with dueDate < today (overdue).
    // Start at root; go left while left subtree exists.
    // Prune any right subtree (or the current node) where subtreeBooks == 0.
    // O(k log n) where k = overdue + available books.
    std::vector<Book> overdueBooks(std::time_t today) const;

    // Estimate the fine for a single overdue book.
    // Fine = daysOverdue * ratePerDay (choose a constant, e.g. 50 PKR/day).
    // daysOverdue = (today - book.dueDate) / 86400   (integer division).
    // Returns 0 if the book is not overdue.
    double estimateFine(const Book& book, std::time_t today) const;

private:
    RBTree<std::time_t, AugNode> tree;
};
