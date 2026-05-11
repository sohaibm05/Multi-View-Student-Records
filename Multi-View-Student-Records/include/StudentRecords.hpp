#ifndef UNICORE_STUDENT_RECORDS_HPP
#define UNICORE_STUDENT_RECORDS_HPP

#include "RBTree.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <limits>

struct Student {
    int id;
    std::string name;
    double cgpa;
    int batchYear;
};

struct StudentNameKey {
    std::string name;
    int id;
    bool operator<(const StudentNameKey& other) const {
        if (name != other.name) return name < other.name;
        return id < other.id;
    }
};

struct StudentCgpaKey {
    double cgpa;
    int id;
    bool operator<(const StudentCgpaKey& other) const {
        if (cgpa != other.cgpa) return cgpa < other.cgpa;
        return id < other.id;
    }
};

struct StudentBatchKey {
    int batchYear;
    int id;
    bool operator<(const StudentBatchKey& other) const {
        if (batchYear != other.batchYear) return batchYear < other.batchYear;
        return id < other.id;
    }
};

template <typename Key>
struct StudentAugment {
    void update(RBNode<Key, Student*>* node, RBNode<Key, Student*>* nil) const {
        if (node == nil) return;
        node->subtreeSize = node->left->subtreeSize + node->right->subtreeSize + 1;
        node->maxEnd = 0;
        node->subtreeBooks = 0;
    }
};

class StudentRecords {
private:
    RBTree<int, Student, std::less<int>, DefaultAugment<int, Student> > byId;
    RBTree<StudentNameKey, Student*, std::less<StudentNameKey>, StudentAugment<StudentNameKey> > byName;
    RBTree<StudentCgpaKey, Student*, std::less<StudentCgpaKey>, StudentAugment<StudentCgpaKey> > byCgpa;
    RBTree<StudentBatchKey, Student*, std::less<StudentBatchKey>, StudentAugment<StudentBatchKey> > byBatch;

    static bool startsWith(const std::string& text, const std::string& prefix);
    int countLessOrEqual(double cgpa) const;

public:
    bool addStudent(int id, const std::string& name, double cgpa, int batchYear);
    bool removeStudent(int id);
    bool updateCgpa(int id, double newCgpa);
    Student* findById(int id);
    const Student* findById(int id) const;

    std::vector<Student> alphabeticalList() const;
    std::vector<Student> prefixSearch(const std::string& prefix) const;
    std::vector<Student> studentsInBatch(int batchYear) const;
    std::vector<Student> studentsInCgpaRange(double low, double high) const;

    Student* kthSmallestCgpa(int k);
    int rankByCgpa(double cgpa) const;
    int countCgpaInRange(double low, double high) const;
    int size() const;
    void printAll() const;
};

#endif
