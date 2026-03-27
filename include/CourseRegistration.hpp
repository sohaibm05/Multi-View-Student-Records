#pragma once
#include "RBTree.hpp"
#include "Student.hpp"
#include <string>
#include <vector>
#include <ctime>

// Member 4 — Course Registration & Integration (Sohaib)
// Composite priority: CGPA desc, batchYear asc, timestamp asc
// Highest-priority student is always the leftmost node

class CourseRegistration {
public:
    // Composite key — comparator enforces priority ordering
    struct Priority {
        double      cgpa;       // higher CGPA → higher priority
        int         batchYear;  // lower year  → higher priority (older batch)
        std::time_t timestamp;  // earlier signup → higher priority

        bool operator<(const Priority& o) const;
        bool operator==(const Priority& o) const;
    };

    struct WaitlistEntry {
        Student     student;
        std::string courseId;
        Priority    priority;
    };

    explicit CourseRegistration(const std::string& courseId);

    // Add student to waitlist
    void enqueue(const Student& student);

    // Pop and return the highest-priority student (leftmost node)
    WaitlistEntry promoteFront();

    // Re-prioritize after CGPA change: remove + reinsert
    void updateCGPA(const std::string& studentId, double newCGPA);

    // Top-n waitlisted students (optionally filtered by batchYear)
    std::vector<WaitlistEntry> topN(int n) const;
    std::vector<WaitlistEntry> topNByYear(int n, int batchYear) const;

    int  size() const;
    bool empty() const;

private:
    std::string                      courseId;
    RBTree<Priority, WaitlistEntry>  tree;
};
