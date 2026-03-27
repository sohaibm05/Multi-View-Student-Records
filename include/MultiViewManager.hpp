#pragma once
#include "Student.hpp"
#include "NameIndex.hpp"
#include "CGPAIndex.hpp"
#include "YearIndex.hpp"
#include <vector>
#include <string>

// =============================================================================
// MultiViewManager  —  Student Records coordinator  (Member 1 — Ibad)
//
// Owns all three RBT indexes and keeps them in sync on every insert/delete.
// Also holds a flat std::vector<Student> for O(n) full-table access and for
// O(1) ID-based lookup during rollback.
//
// The rollback contract:
//   addStudent    → inserts into nameIdx, cgpaIdx, yearIdx in that order.
//                   If the second insert fails, remove from nameIdx and return false.
//                   If the third insert fails, remove from nameIdx + cgpaIdx and return false.
//   removeStudent → all three removals; the in-memory records vector is the
//                   source of truth for which student has which fields.
// =============================================================================

class MultiViewManager {
public:
    MultiViewManager();

    // ── Atomic multi-tree operations ──────────────────────────────────────────

    // Insert student into all three indexes atomically.
    // Pre-check: reject if student.id already exists (findById != nullptr).
    // On success: push to records vector, return true.
    bool addStudent(const Student& student);

    // Remove student from all three indexes by ID.
    // Looks up the full Student record first (needed to reconstruct keys).
    // Returns false if id not found.
    bool removeStudent(const std::string& id);

    // ── Single-view lookups (delegate to the right index) ─────────────────────

    // Delegate to nameIdx.prefixSearch.
    std::vector<Student> searchByNamePrefix(const std::string& prefix) const;

    // Delegate to cgpaIdx.getInRange.
    std::vector<Student> searchByCGPARange(double low, double high) const;

    // Delegate to yearIdx.getByYear.
    std::vector<Student> searchByYear(int year) const;

    // Delegate to yearIdx.rangeByYear.
    std::vector<Student> searchByYearRange(int startYear, int endYear) const;

    // ── Cross-view query ──────────────────────────────────────────────────────

    // Combine year, minCGPA, and name prefix in one query.
    // Strategy: use yearIdx.getByYear as the primary filter (typically the
    // smallest result set), then apply CGPA and name-prefix checks inline.
    // This reuses the index's O(log n) range walk instead of scanning everything.
    std::vector<Student> crossQuery(int year, double minCGPA,
                                    const std::string& namePrefix) const;

    // ── Display / benchmarking ────────────────────────────────────────────────

    // Print all students sorted by name (delegates to nameIdx.getAllSorted).
    void printAll() const;

    // Insert n random students and time RBT queries vs a sorted-vector doing the
    // same queries.  Print the comparison.  (Implemented by Member 4.)
    void runBenchmark(int n) const;

private:
    NameIndex  nameIdx;
    CGPAIndex  cgpaIdx;
    YearIndex  yearIdx;

    // Flat list used for:
    //   • O(1) duplicate-ID detection (linear scan — acceptable since we also
    //     need the full Student object for key reconstruction on remove).
    //   • Rollback: if a partial insert fails, we can find the already-inserted
    //     record and undo.
    std::vector<Student> records;

    // Linear scan by id.  Returns pointer into records, or nullptr if absent.
    // Keep records small — this scan is only called on insert/delete paths.
    Student* findById(const std::string& id);
};
