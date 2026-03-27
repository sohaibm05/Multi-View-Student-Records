#pragma once
#include "RBTree.hpp"
#include "Student.hpp"
#include <vector>

// =============================================================================
// CGPAIndex  —  CGPA-ranked view with order-statistics  (Member 1 — Ibad)
//
// Wraps a RBTree<string, AugNode> where:
//   key   = makeKey(cgpa, id)   e.g. "3.850000#sm09751"
//             (fixed-precision string so lexicographic order = numeric order)
//   value = AugNode { student, subtreeSize }
//
// The subtreeSize augmentation on every node enables O(log n) rank queries:
//   subtreeSize(node) = 1 + subtreeSize(left) + subtreeSize(right)
//
// IMPORTANT — keep subtreeSize correct after every rotation:
//   In leftRotate(x):  update x->value.subtreeSize first (x went down),
//                      then  y->value.subtreeSize       (y went up).
//   Same logic applies in rightRotate.
// =============================================================================

class CGPAIndex {
public:
    CGPAIndex();

    // Wrap student in AugNode(subtreeSize=1), build composite key, call tree.insert.
    // After insert the fix-up rotations will call the augmentation update.
    void insert(const Student& student);

    // Rebuild key from (cgpa, id) and call tree.remove.
    // Returns false if not found.
    bool remove(double cgpa, const std::string& id);

    // Return all students with CGPA in [low, high].
    // lowerBound(makeKey(low,  ""))     → first candidate
    // Walk forward while key <= makeKey(high, "\xff")
    // O(log n + k)
    std::vector<Student> getInRange(double low, double high) const;

    // Count students with CGPA in [low, high] using the subtreeSize augmentation.
    // Alternatively, use lowerBound + upperBound and do arithmetic on subtreeSizes.
    // O(log n)
    int countInRange(double low, double high) const;

    // Return the student with the k-th lowest CGPA (1-indexed).
    // Walk the tree: if left.subtreeSize >= k → go left
    //               if left.subtreeSize + 1 == k → this is it
    //               else k -= left.subtreeSize + 1; go right
    // O(log n)
    Student kthSmallest(int k) const;

    // Return the top-k students by CGPA (highest first).
    // Start from back() (maximum node) and walk k steps toward smaller keys.
    // O(k log n)  or use a reverse inorder with early exit.
    std::vector<Student> topK(int k) const;

private:
    // AugNode wraps a Student and adds the subtreeSize field.
    // subtreeSize starts at 1 for a freshly inserted leaf.
    struct AugNode {
        Student student;
        int     subtreeSize;
        AugNode() : subtreeSize(0) {}
        explicit AugNode(const Student& s) : student(s), subtreeSize(1) {}
    };

    // Key: fixed-width CGPA string + "#" + id.
    // Use std::to_string or snprintf with 6 decimal places so that
    // "3.850000" < "4.000000" lexicographically (matches numeric order).
    RBTree<std::string, AugNode> tree;

    // Format: std::to_string(cgpa) + "#" + id
    static std::string makeKey(double cgpa, const std::string& id);
};
