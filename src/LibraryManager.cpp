// Member 3 — LibraryManager (Saad)

#include "LibraryManager.hpp"

LibraryManager::LibraryManager() = default;

// Wrap book in AugNode(book) — constructor sets subtreeBooks = (checkedOut ? 0 : 1).
// Call tree.insert(book.dueDate, augNode).
// After rotations, leftRotate/rightRotate must update subtreeBooks on rotated nodes.
void LibraryManager::addBook(const Book& book) {}

// Scan via inorder to find the node whose book.isbn == isbn.
// Once found, call tree.remove(foundNode->key).
// Returns false if not found.
bool LibraryManager::removeBook(const std::string& isbn) { return false; }

// 1. Scan via inorder to find the node with matching isbn.
// 2. If not found or already checked out → return false.
// 3. Set node->value.book.checkedOut = true.
// 4. Walk up the parent chain (node->parent, parent->parent, … until NIL):
//      ancestor->value.subtreeBooks -= 1
//    This propagates the availability change upward in O(log n).
bool LibraryManager::checkOut(const std::string& isbn) { return false; }

// Mirror of checkOut:
// 1. Find node by isbn.
// 2. If not found or not currently checked out → return false.
// 3. Set node->value.book.checkedOut = false.
// 4. Walk up the parent chain: ancestor->value.subtreeBooks += 1
bool LibraryManager::returnBook(const std::string& isbn) { return false; }

// 1. node = tree.lowerBound(d1)   ← first node with dueDate >= d1
// 2. Walk while node != nullptr AND node->key <= d2:
//      if node->value.subtreeBooks == 0 → can skip; but note: subtreeBooks
//        only helps for pruning subtrees, not individual nodes on the path.
//      if !node->value.book.checkedOut → count++
//      advance to next node (use lowerBound(node->key + 1) or an inorder walk)
// 3. Return count.
int LibraryManager::availableInRange(std::time_t d1, std::time_t d2) const { return 0; }

// Walk the tree looking for books with dueDate < today.
// Recursive helper signature:  void collect(Node* x, time_t today, vector<Book>& out)
//   if x == NIL → return
//   if x->value.subtreeBooks == 0 → return (no available books here, skip)
//   recurse left  (left subtree has earlier due dates = more likely overdue)
//   if x->value.book.dueDate < today AND !x->value.book.checkedOut → add x->value.book
//   recurse right (may also be overdue)
std::vector<Book> LibraryManager::overdueBooks(std::time_t today) const { return {}; }

// Fine = max(0, daysOverdue) * RATE_PER_DAY
// daysOverdue = (int)((today - book.dueDate) / 86400)
// RATE_PER_DAY = 50.0  (PKR per day, or choose your constant)
// Return 0.0 if the book is not overdue (today <= book.dueDate).
double LibraryManager::estimateFine(const Book& book, std::time_t today) const { return 0.0; }
