// Member 1 — YearIndex (Ibad)

#include "YearIndex.hpp"
#include <sstream>
#include <iomanip>

YearIndex::YearIndex() = default;

// Format: zero-padded 4-digit year + "#" + id
// e.g. makeKey(2022, "sm09751") → "2022#sm09751"
// Use std::ostringstream with std::setw(4) and std::setfill('0') for the year.
// Zero-padding guarantees "2009" < "2022" lexicographically.
std::string YearIndex::makeKey(int batchYear, const std::string& id) {}

// Build composite key from student.batchYear and student.id, call tree.insert.
void YearIndex::insert(const Student& student) {}

// Rebuild key from (batchYear, id), call tree.remove(key).
// Return false if not found.
bool YearIndex::remove(int batchYear, const std::string& id) {}

// Return all students enrolled in exactly this year.
// yearPrefix = makeKey(year, "")   ← e.g. "2022#"
// node = tree.lowerBound(yearPrefix)
// Walk forward while node->key starts with yearPrefix (check first 5 chars: "YYYY#").
// Collect node->value into result.
std::vector<Student> YearIndex::getByYear(int year) const {}

// Return all students with batchYear in [startYear, endYear].
// node = tree.lowerBound(makeKey(startYear, ""))
// Walk forward while the year parsed from node->key <= endYear.
// Parse year: std::stoi(node->key.substr(0, 4))
std::vector<Student> YearIndex::rangeByYear(int startYear, int endYear) const {}

// Count students in a specific year.
// Simplest: return getByYear(year).size().
// Optimal: do the same walk as getByYear but only increment a counter.
int YearIndex::countByYear(int year) const {}

// Find the next batch year that exists AFTER the given year.
// node = tree.upperBound(makeKey(year, "\xff"))
//   ← upperBound with "\xff" as id skips ALL keys for this year and beyond.
// If node != nullptr, return std::stoi(node->key.substr(0, 4)).
// Return -1 if no later year exists.
int YearIndex::successor(int year) const {}

// Find the previous batch year that exists BEFORE the given year.
// node = tree.lowerBound(makeKey(year, ""))
//   ← this lands on the first key for this year.
// Walk one step left using the BST predecessor logic:
//   if node->left != NIL → go to maximum of left subtree
//   else walk up through parent until you go right.
// Return the year of that predecessor node, or -1 if none.
// Alternatively: do a full inorder, take the element just before the year.
int YearIndex::predecessor(int year) const {}
