#ifndef UNICORE_COURSE_REGISTRATION_HPP
#define UNICORE_COURSE_REGISTRATION_HPP

#include "RBTree.hpp"
#include "StudentRecords.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <limits>

struct WaitlistEntry {
    int studentId;
    std::string courseCode;
    double cgpa;
    int batchYear;
    long long timestamp;
};

struct WaitlistKey {
    std::string courseCode;
    double negCgpa;
    int batchYear;
    long long timestamp;
    int studentId;

    bool operator<(const WaitlistKey& other) const {
        if (courseCode != other.courseCode) return courseCode < other.courseCode;
        if (negCgpa != other.negCgpa) return negCgpa < other.negCgpa;
        if (batchYear != other.batchYear) return batchYear < other.batchYear;
        if (timestamp != other.timestamp) return timestamp < other.timestamp;
        return studentId < other.studentId;
    }
};

template <typename Key>
struct BasicAugment {
    void update(RBNode<Key, WaitlistEntry>* node, RBNode<Key, WaitlistEntry>* nil) const {
        if (node == nil) return;
        node->subtreeSize = node->left->subtreeSize + node->right->subtreeSize + 1;
        node->maxEnd = 0;
        node->subtreeBooks = 0;
    }
};

struct CourseStudentKey {
    std::string courseCode;
    int studentId;
    bool operator<(const CourseStudentKey& other) const {
        if (courseCode != other.courseCode) return courseCode < other.courseCode;
        return studentId < other.studentId;
    }
};

class CourseRegistration {
private:
    RBTree<WaitlistKey, WaitlistEntry, std::less<WaitlistKey>, BasicAugment<WaitlistKey> > waitlist;
    RBTree<CourseStudentKey, WaitlistKey> index;
    long long currentTime;

    static WaitlistKey makeKey(const WaitlistEntry& e);

public:
    CourseRegistration();
    bool addToWaitlist(const std::string& courseCode, const Student& student);
    bool removeFromWaitlist(const std::string& courseCode, const Student& student);
    bool updateStudentPriority(const std::string& courseCode, const Student& oldStudent, const Student& newStudent);
    WaitlistEntry* front(const std::string& courseCode);
    bool promoteFront(const std::string& courseCode, WaitlistEntry& promoted);
    std::vector<WaitlistEntry> courseWaitlist(const std::string& courseCode) const;
    std::vector<WaitlistEntry> allWaitlistEntries() const;
    std::vector<WaitlistEntry> topWaitlistedFromBatch(const std::string& courseCode, int batchYear, int limit) const;
    int size() const;
    void printAll() const;
};

#endif
