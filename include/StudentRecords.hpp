#ifndef STUDENTRECORDS_HPP
#define STUDENTRECORDS_HPP

#include "RBTree.hpp"
#include <string>
#include <vector>

struct Student {
    std::string id;
    std::string name;
    double cgpa;
    int batchYear;
};

class CGPATree : public RBTree<double, Student> {
public:
    virtual void initializeNode(Node* node) override {
        if (node != NIL) node->subtreeSize = 1;
    }
    virtual void rotationHook(Node* node) override {
        if (node != NIL) {
            node->subtreeSize = 1 + node->left->subtreeSize + node->right->subtreeSize;
        }
    }

    // Order-statistic queries (O(log n))
    Node* kthSmallest(size_t k) const;           // 1-based
    size_t rank(double cgpa) const;              // position of this CGPA
    size_t countInRange(double lo, double hi) const;
};

class StudentRecords {
public:
    void addStudent(const Student& s);
    CGPATree cgpaTree;
    RBTree<std::string, Student> nameTree;
    RBTree<int, Student> batchTree;
};

#endif