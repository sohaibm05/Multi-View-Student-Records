#ifndef COURSEREGISTRATION_HPP
#define COURSEREGISTRATION_HPP

#include "RBTree.hpp"
#include <string>

struct PriorityKey {
    double cgpa;
    int batchYear;
    long long timestamp;
};

inline bool operator<(const PriorityKey& a, const PriorityKey& b) {
    if (a.cgpa != b.cgpa) return a.cgpa > b.cgpa;   // highest CGPA first
    if (a.batchYear != b.batchYear) return a.batchYear < b.batchYear;
    return a.timestamp < b.timestamp;
}

inline bool operator==(const PriorityKey& a, const PriorityKey& b) {
    return (a.cgpa == b.cgpa) &&
           (a.batchYear == b.batchYear) &&
           (a.timestamp == b.timestamp);
}

struct WaitlistEntry {
    std::string studentId;
};

class CourseWaitlist : public RBTree<PriorityKey, WaitlistEntry> {
public:
    std::string promoteFront();
};

class CourseRegistration {
public:
    void addToWaitlist(const std::string& id, double cgpa, int batch, long long time);
    CourseWaitlist waitlist;
};

#endif