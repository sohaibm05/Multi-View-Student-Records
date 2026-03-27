#pragma once
#include "RBTree.hpp"
#include "Student.hpp"
#include <vector>

// =============================================================================
// YearIndex  —  batch-year view with successor/predecessor  (Member 1 — Ibad)
//
// Wraps a RBTree<string, Student> where the key is:
//   zero-padded year + "#" + student.id
//   e.g. "2022#sm09751"
//
// Zero-padding the year (4 digits) ensures lexicographic order = numeric order.
// The "#id" suffix makes every key unique for students in the same year.
// =============================================================================

class YearIndex {
public:
    YearIndex();

    // Build composite key and call tree.insert.
    void insert(const Student& student);

    // Rebuild key from (batchYear, id) and call tree.remove.
    // Returns false if not found.
    bool remove(int batchYear, const std::string& id);

    // Return all students enrolled in exactly this year.
    // lowerBound(makeKey(year, ""))  → first node for this year
    // Walk forward while key starts with the year prefix.
    // O(log n + k)
    std::vector<Student> getByYear(int year) const;

    // Return all students with batchYear in [startYear, endYear].
    // lowerBound(makeKey(startYear, ""))  → first candidate
    // Walk forward while parsed year <= endYear.
    // O(log n + k)
    std::vector<Student> rangeByYear(int startYear, int endYear) const;

    // Count students in a specific year.  Can reuse getByYear and return size,
    // or do it without allocating a vector by counting during the walk.
    // O(log n + k)
    int countByYear(int year) const;

    // Return the next batch year that exists in the tree after the given year.
    // Use upperBound(makeKey(year, "\xff")) to skip past all keys for this year,
    // then parse the year out of the returned node's key.
    // Returns -1 if no later year exists.
    // O(log n)
    int successor(int year) const;

    // Return the previous batch year before the given year.
    // Use lowerBound(makeKey(year, "")) and walk one step to the left predecessor.
    // Returns -1 if no earlier year exists.
    // O(log n)
    int predecessor(int year) const;

private:
    // Key format: zero-padded 4-digit year + "#" + id
    // Use std::to_string(batchYear) with leading zeros to ensure 4 digits.
    RBTree<std::string, Student> tree;

    // Build key: e.g. makeKey(2022, "sm09751") → "2022#sm09751"
    static std::string makeKey(int batchYear, const std::string& id);
};
