// Member 3

#include "YearIndex.hpp"

YearIndex::YearIndex() = default;

std::string YearIndex::makeKey(int batchYear, const std::string& id) {}

void YearIndex::insert(const Student& student) {}

bool YearIndex::remove(int batchYear, const std::string& id) {}

std::vector<Student> YearIndex::getByYear(int year) const {}

std::vector<Student> YearIndex::rangeByYear(int startYear, int endYear) const {}

int YearIndex::countByYear(int year) const {}

int YearIndex::successor(int year) const {}

int YearIndex::predecessor(int year) const {}
