#pragma once
#include "RBTree.hpp"
#include "Student.hpp"
#include <string>
#include <vector>

// Member 1
// Name-based index with case-insensitive search and prefix queries

class NameIndex {
public:
    NameIndex();

    void insert(const Student& student);
    bool remove(const std::string& name, const std::string& id);

    std::vector<Student> prefixSearch(const std::string& prefix) const;
    std::vector<Student> rangeByName(const std::string& a, const std::string& b) const;
    std::vector<Student> getAllSorted() const;

private:
    // Key: lowercase name + "#" + id (to allow duplicates with same name)
    RBTree<std::string, Student> tree;

    static std::string toLower(const std::string& s);
    static std::string makeKey(const std::string& name, const std::string& id);
};
