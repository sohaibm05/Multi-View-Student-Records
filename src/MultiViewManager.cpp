// Member 4

#include "MultiViewManager.hpp"

MultiViewManager::MultiViewManager() = default;

Student* MultiViewManager::findById(const std::string& id) {}

bool MultiViewManager::addStudent(const Student& student) {}

bool MultiViewManager::removeStudent(const std::string& id) {}

std::vector<Student> MultiViewManager::searchByNamePrefix(const std::string& prefix) const {}

std::vector<Student> MultiViewManager::searchByCGPARange(double low, double high) const {}

std::vector<Student> MultiViewManager::searchByYear(int year) const {}

std::vector<Student> MultiViewManager::searchByYearRange(int startYear, int endYear) const {}

std::vector<Student> MultiViewManager::crossQuery(int year, double minCGPA, const std::string& namePrefix) const {}

void MultiViewManager::printAll() const {}

void MultiViewManager::runBenchmark(int n) const {}
