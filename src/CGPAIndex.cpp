// Member 1 — CGPAIndex (Ibad)

#include "CGPAIndex.hpp"
#include <sstream>
#include <iomanip>

CGPAIndex::CGPAIndex() = default;

// Format: fixed 6-decimal-place CGPA + "#" + id
// e.g. makeKey(3.85, "sm09751") → "3.850000#sm09751"
// Use std::ostringstream with std::fixed and std::setprecision(6).
// The fixed width ensures lexicographic order matches numeric CGPA order.
std::string CGPAIndex::makeKey(double cgpa, const std::string& id) {}

// Wrap student in AugNode(student), build composite key, call tree.insert(key, augNode).
// subtreeSize will be set to 1 by AugNode's constructor (it's a fresh leaf).
// After insertFixup runs its rotations, the augmentation hook must update
// subtreeSize on the rotated nodes.
void CGPAIndex::insert(const Student& student) {}

// Rebuild key from (cgpa, id) and call tree.remove(key).
// Return false if not found.
bool CGPAIndex::remove(double cgpa, const std::string& id) {}

// 1. loKey = makeKey(low,  "")      ← empty id sorts before any real id
// 2. hiKey = makeKey(high, "\xff")  ← "\xff" sorts after any real id
// 3. node  = tree.lowerBound(loKey)
// 4. Walk forward while node != nullptr AND node->key <= hiKey
//      push node->value.student into result
// 5. Return result.
std::vector<Student> CGPAIndex::getInRange(double low, double high) const {}

// Count nodes in [low, high] using the subtreeSize augmentation.
// Approach A (simple): reuse getInRange and return result.size() — O(k).
// Approach B (optimal, O(log n)):
//   rank(high+ε) - rank(low-ε) using the subtreeSize walk.
int CGPAIndex::countInRange(double low, double high) const {}

// Find the k-th smallest CGPA student (1-indexed) using subtreeSize.
// Start at root:
//   leftSize = (root->left == NIL) ? 0 : root->left->value.subtreeSize
//   if leftSize + 1 == k  → this node is the answer
//   if leftSize >= k       → recurse into left subtree with same k
//   else                   → recurse into right subtree with k -= (leftSize + 1)
// O(log n)
Student CGPAIndex::kthSmallest(int k) const {}

// Return the top-k students sorted by CGPA descending (highest CGPA first).
// Use tree.inorder to collect all students, then take the LAST k entries
// (inorder gives ascending order, so the last k are the highest).
// Alternatively, walk from tree.back() backwards.
std::vector<Student> CGPAIndex::topK(int k) const {}
