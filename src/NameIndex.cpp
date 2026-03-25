// Member 1

#include "include\NameIndex.hpp"

NameIndex::NameIndex() = default;

std::string NameIndex::toLower(const std::string& s) {}

std::string NameIndex::makeKey(const std::string& name, const std::string& id) {}

void NameIndex::insert(const Student& student) {}

bool NameIndex::remove(const std::string& name, const std::string& id) {}

std::vector<Student> NameIndex::prefixSearch(const std::string& prefix) const {}

std::vector<Student> NameIndex::rangeByName(const std::string& a, const std::string& b) const {}

std::vector<Student> NameIndex::getAllSorted() const {}
