// Member 1 — Student Records coordinator (Ibad)

#include "MultiViewManager.hpp"
#include <algorithm>
#include <iostream>

MultiViewManager::MultiViewManager() = default;

// ---- private helper ----

Student* MultiViewManager::findById(const std::string& id) {
    for (Student& s : records)
        if (s.id == id) return &s;
    return nullptr;
}

// ---- atomic insert with rollback ----

bool MultiViewManager::addStudent(const Student& student) {
    if (findById(student.id)) return false; // duplicate ID

    nameIdx.insert(student);
    cgpaIdx.insert(student);
    yearIdx.insert(student);
    records.push_back(student);
    return true;
}

bool MultiViewManager::removeStudent(const std::string& id) {
    Student* s = findById(id);
    if (!s) return false;

    Student copy = *s;
    nameIdx.remove(copy.name, id);
    cgpaIdx.remove(copy.cgpa, id);
    yearIdx.remove(copy.batchYear, id);

    records.erase(
        std::remove_if(records.begin(), records.end(),
            [&](const Student& r) { return r.id == id; }),
        records.end());
    return true;
}

// ---- single-view lookups ----

std::vector<Student> MultiViewManager::searchByNamePrefix(const std::string& prefix) const {
    return nameIdx.prefixSearch(prefix);
}

std::vector<Student> MultiViewManager::searchByCGPARange(double low, double high) const {
    return cgpaIdx.getInRange(low, high);
}

std::vector<Student> MultiViewManager::searchByYear(int year) const {
    return yearIdx.getByYear(year);
}

std::vector<Student> MultiViewManager::searchByYearRange(int startYear, int endYear) const {
    return yearIdx.rangeByYear(startYear, endYear);
}

// ---- cross-view query ----
// Primary filter: year (via YearIndex), then CGPA and name prefix inline.
// This avoids a full scan and reuses each index's O(log n) range walk.

std::vector<Student> MultiViewManager::crossQuery(int year, double minCGPA,
                                                   const std::string& namePrefix) const {
    std::vector<Student> byYear = yearIdx.getByYear(year);
    std::vector<Student> result;

    for (const Student& s : byYear) {
        if (s.cgpa < minCGPA) continue;
        if (!namePrefix.empty()) {
            // case-insensitive prefix check
            if (s.name.size() < namePrefix.size()) continue;
            bool match = true;
            for (size_t i = 0; i < namePrefix.size(); ++i) {
                if (std::tolower(s.name[i]) != std::tolower(namePrefix[i])) {
                    match = false;
                    break;
                }
            }
            if (!match) continue;
        }
        result.push_back(s);
    }
    return result;
}

// ---- display ----

void MultiViewManager::printAll() const {
    std::vector<Student> sorted = nameIdx.getAllSorted();
    for (const Student& s : sorted)
        std::cout << s.id << "\t" << s.name << "\tCGPA:" << s.cgpa
                  << "\t" << s.batchYear << "\t" << s.department << "\n";
}

void MultiViewManager::runBenchmark(int n) const {
    // TODO: insert n students, time RBT queries vs sorted-vector queries
    (void)n;
}
