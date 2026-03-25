#pragma once
#include "RBTree.hpp"
#include "Student.hpp"
#include <vector>

// Member 2
// CGPA-based index with augmented subtree sizes for order-statistics

class CGPAIndex {
public:
    CGPAIndex();

    void insert(const Student& student);
    bool remove(double cgpa, const std::string& id);

    int countInRange(double low, double high) const;
    Student kthSmallest(int k) const;
    std::vector<Student> topK(int k) const;
    std::vector<Student> getInRange(double low, double high) const;

private:
    // Key: cgpa; augmented with subtreeSize per node
    // Using a specialized augmented tree node
    struct AugNode {
        Student student;
        int subtreeSize;
        AugNode(const Student& s) : student(s), subtreeSize(1) {}
    };

    // Key: cgpa + "#" + id to handle duplicate CGPAs
    RBTree<std::string, AugNode> tree;

    static std::string makeKey(double cgpa, const std::string& id);
};
