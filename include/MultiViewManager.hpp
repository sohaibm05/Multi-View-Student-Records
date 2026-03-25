#pragma once
#include "Student.hpp"
#include "NameIndex.hpp"
#include "CGPAIndex.hpp"
#include "YearIndex.hpp"
#include <vector>
#include <string>

// Member 4
// Coordinator: keeps all three indexes in sync, cross-view queries, benchmarking

class MultiViewManager {
public:
    MultiViewManager();

    // Atomic insert/delete across all three trees
    bool addStudent(const Student& student);
    bool removeStudent(const std::string& id);

    // Single-view lookups (delegates to the appropriate index)
    std::vector<Student> searchByNamePrefix(const std::string& prefix) const;
    std::vector<Student> searchByCGPARange(double low, double high) const;
    std::vector<Student> searchByYear(int year) const;
    std::vector<Student> searchByYearRange(int startYear, int endYear) const;

    // Cross-view queries
    std::vector<Student> crossQuery(int year, double minCGPA, const std::string& namePrefix) const;

    // Benchmarking
    void runBenchmark(int n) const;

    // Display helpers
    void printAll() const;

private:
    NameIndex  nameIdx;
    CGPAIndex  cgpaIdx;
    YearIndex  yearIdx;

    // In-memory map for O(1) lookup by ID (for rollback on partial failure)
    std::vector<Student> records;

    Student* findById(const std::string& id);
};
