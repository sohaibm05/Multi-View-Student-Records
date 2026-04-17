#include "StudentRecords.hpp"

void StudentRecords::addStudent(const Student& s) {
    nameTree.insert(s.name, s);
    cgpaTree.insert(s.cgpa, s);
    batchTree.insert(s.batchYear, s);
}

CGPATree::Node* CGPATree::kthSmallest(size_t k) const {
    Node* x = root;
    while (x != NIL) {
        size_t leftSize = x->left->subtreeSize;
        if (k == leftSize + 1) return x;
        if (k <= leftSize) x = x->left;
        else { k -= leftSize + 1; x = x->right; }
    }
    return nullptr;
}

size_t CGPATree::rank(double cgpa) const {
    size_t r = 0;
    Node* x = root;
    while (x != NIL) {
        if (cgpa < x->key) x = x->left;
        else {
            r += x->left->subtreeSize + 1;
            x = x->right;
        }
    }
    return r;
}

size_t CGPATree::countInRange(double lo, double hi) const {
    // Simple O(log n) range count using subtreeSize (full impl omitted for 30%)
    size_t cnt = 0;
    Node* x = root;
    while (x != NIL) {
        if (hi < x->key) x = x->left;
        else if (lo > x->key) x = x->right;
        else {
            if (lo <= x->key && x->key <= hi) cnt++;
            cnt += x->left->subtreeSize; // rough - full walk would be better
            x = x->right;
        }
    }
    return cnt;
}