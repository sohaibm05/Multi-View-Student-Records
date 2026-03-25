#pragma once
#include "RBTree.hpp"
#include "Student.hpp"
#include <vector>

// Member 3
// Batch-year-based index with successor/predecessor navigation

class YearIndex {
public:
    YearIndex();

    void insert(const Student& student);
    bool remove(int batchYear, const std::string& id);

    std::vector<Student> rangeByYear(int startYear, int endYear) const;
    int countByYear(int year) const;

    // Returns the next/previous batch year that exists in the tree
    int successor(int year) const;
    int predecessor(int year) const;

    std::vector<Student> getByYear(int year) const;

private:
    // Key: batchYear * 1000000 + numeric suffix for ordering within same year
    // Simpler approach: key = "YYYY#id"
    RBTree<std::string, Student> tree;

    static std::string makeKey(int batchYear, const std::string& id);
};
