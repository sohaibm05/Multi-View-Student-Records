#pragma once
#include "RBTree.hpp"
#include "Student.hpp"
#include <string>
#include <vector>

// =============================================================================
// NameIndex  —  alphabetical view of students  (Member 1 — Ibad)
//
// Wraps a RBTree<string, Student> where the key is:
//   toLower(student.name) + "#" + student.id
//
// The "#id" suffix makes every key unique even when two students share a name,
// and it sorts same-name students consistently by ID.
//
// Supports O(log n) prefix search and alphabetical range queries.
// =============================================================================

class NameIndex {
public:
    NameIndex();

    // Insert student into the name tree.
    // Build the composite key with makeKey, then call tree.insert.
    void insert(const Student& student);

    // Remove the student identified by (name, id).
    // Rebuild the composite key and call tree.remove.
    // Returns false if the student is not found.
    bool remove(const std::string& name, const std::string& id);

    // Return all students whose lowercase name starts with prefix.
    // Use lowerBound(toLower(prefix)) to find the first candidate,
    // then walk forward while node->key starts with the lowercased prefix.
    // O(log n + k) where k = number of matches.
    std::vector<Student> prefixSearch(const std::string& prefix) const;

    // Return all students with names in [a, b] alphabetically.
    // Use lowerBound(toLower(a)) as start, walk while key <= makeKey(b, "\xff").
    // O(log n + k)
    std::vector<Student> rangeByName(const std::string& a, const std::string& b) const;

    // Return every student in ascending name order (full inorder traversal).
    // O(n)
    std::vector<Student> getAllSorted() const;

private:
    // Composite key: lowercase name + "#" + id.
    // Example: "sohaib muhammad#sm09751"
    // The "#" separator guarantees no name can accidentally be a prefix of the key.
    RBTree<std::string, Student> tree;

    // Convert every character in s to lowercase.
    // Use std::tolower inside a loop (or std::transform with ::tolower).
    static std::string toLower(const std::string& s);

    // Build the composite key from a (possibly mixed-case) name and an id.
    static std::string makeKey(const std::string& name, const std::string& id);
};
