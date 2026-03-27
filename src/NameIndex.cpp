// Member 1 — NameIndex (Ibad)

#include "NameIndex.hpp"

NameIndex::NameIndex() = default;

// Convert every character to lowercase.
// Loop over s, apply std::tolower(c) to each char, build a new string.
std::string NameIndex::toLower(const std::string& s) {}

// Composite key = toLower(name) + "#" + id
// The "#" character sits outside the alphabet so no name can accidentally
// collide with the separator.
std::string NameIndex::makeKey(const std::string& name, const std::string& id) {}

// Build composite key from student.name and student.id, then call tree.insert.
void NameIndex::insert(const Student& student) {}

// Rebuild the composite key from the arguments, call tree.remove(key).
// Return whatever tree.remove returns (false if the key wasn't found).
bool NameIndex::remove(const std::string& name, const std::string& id) {}

// 1. lowercasedPrefix = toLower(prefix)
// 2. node = tree.lowerBound(lowercasedPrefix)   ← first key >= prefix
// 3. Walk forward while node != nullptr AND node->key starts with lowercasedPrefix
//      collect node->value into result
//      advance: node = tree.lowerBound(node->key + "\x01")  OR use inorder iterator
// 4. Return result.
// Tip: std::string::substr(0, prefix.size()) == prefix  checks the prefix.
std::vector<Student> NameIndex::prefixSearch(const std::string& prefix) const {}

// 1. start = tree.lowerBound(makeKey(a, ""))      ← first key >= "a#"
// 2. endKey = makeKey(b, "\xff")                  ← last possible key for name b
// 3. Walk from start forward while node->key <= endKey, collect node->value.
std::vector<Student> NameIndex::rangeByName(const std::string& a, const std::string& b) const {}

// Call tree.inorder with a lambda that pushes every value into a vector.
// Returns students sorted alphabetically (ascending key order = ascending name order).
std::vector<Student> NameIndex::getAllSorted() const {}
