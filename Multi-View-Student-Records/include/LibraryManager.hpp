#ifndef UNICORE_LIBRARY_MANAGER_HPP
#define UNICORE_LIBRARY_MANAGER_HPP

#include "RBTree.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>

struct Book {
    int id;
    std::string title;
    std::string author;
    long long dueDate;   // 0 means N/A because the book is currently available.
    bool available;
};

struct BookDueKey {
    long long dueDate;
    int id;
    bool operator<(const BookDueKey& other) const {
        if (dueDate != other.dueDate) return dueDate < other.dueDate;
        return id < other.id;
    }
};

struct LibraryAugment {
    void update(RBNode<BookDueKey, Book>* node, RBNode<BookDueKey, Book>* nil) const {
        if (node == nil) return;
        node->subtreeSize = node->left->subtreeSize + node->right->subtreeSize + 1;
        node->maxEnd = 0;
        int self = node->value.available ? 1 : 0;
        node->subtreeBooks = node->left->subtreeBooks + node->right->subtreeBooks + self;
    }
};

class LibraryManager {
private:
    RBTree<BookDueKey, Book, std::less<BookDueKey>, LibraryAugment> books;
    int nextId;

    typename RBTree<BookDueKey, Book, std::less<BookDueKey>, LibraryAugment>::Node* findNodeById(int id) const;
    void collectOverdue(typename RBTree<BookDueKey, Book, std::less<BookDueKey>, LibraryAugment>::Node* node,
                        long long today, std::vector<Book>& result) const;
    void collectCheckedOutDueRange(typename RBTree<BookDueKey, Book, std::less<BookDueKey>, LibraryAugment>::Node* node,
                                   long long d1, long long d2, std::vector<Book>& result) const;

public:
    static long long addDaysToDate(long long yyyymmdd, int days);
    static int daysBetweenDates(long long startYyyymmdd, long long endYyyymmdd);
    // User-facing Pakistani date format is DDMMYYYY, e.g. 31052025.
    static long long pakToInternal(long long ddmmyyyy);
    static long long internalToPak(long long yyyymmdd);
    static std::string displayDate(long long yyyymmdd);
    LibraryManager();

    // Preferred for the UI: available books have no due date, so only title and author are entered.
    bool addBook(const std::string& title, const std::string& author);

    // Kept for compatibility/tests. If available is true, dueDate is stored as 0/N/A.
    bool addBook(const std::string& title, const std::string& author, long long dueDate, bool available = true);

    // Checkout can be done by book title/name. User enters the due date manually.
    bool checkoutBookByTitle(const std::string& title, long long dueDate);

    // Compatibility/helper checkout by ID. The second argument is the due date.
    bool checkoutBook(int id, long long dueDate);

    // Compatibility helper: checks out with a default due date using the computer date.
    bool checkoutBook(int id);
    // Return can be done by book title/name. The first checked-out matching title is returned.
    bool returnBookByTitle(const std::string& title);

    // Compatibility/helper return by ID.
    bool returnBook(int id);

    // Compatibility overload. Return only needs ID; the date argument is ignored.
    bool returnBook(int id, long long ignoredDate);

    int availableCount() const;
    std::vector<Book> checkedOutDueInRange(long long d1, long long d2) const;
    std::vector<Book> overdueBooks(long long today) const;
    std::vector<Book> allBooks() const;
    int estimateFine(const Book& book, long long today, int finePerDay = 10) const;
    int size() const;
    void printAll() const;
};

#endif
