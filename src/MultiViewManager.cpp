// Member 1 — Student Records coordinator (Ibad)

#include "MultiViewManager.hpp"
#include <algorithm>
#include <iostream>

MultiViewManager::MultiViewManager() = default;

// ── private helper ────────────────────────────────────────────────────────────

// Linear scan through the flat records vector.
// Returns a pointer into the vector (valid until records is modified).
// Used to:  (a) detect duplicate IDs before insert
//           (b) retrieve the full Student struct for key reconstruction on remove
Student* MultiViewManager::findById(const std::string& id) {
    for (Student& s : records)
        if (s.id == id) return &s;
    return nullptr;
}

// ── atomic insert with duplicate guard ───────────────────────────────────────

bool MultiViewManager::addStudent(const Student& student) {
    // Reject duplicates before touching any tree.
    if (findById(student.id)) return false;

    // Insert into all three trees.
    // TODO (Ibad): wrap each in a try/catch and rollback the earlier inserts
    // if a later one throws, then return false.
    nameIdx.insert(student);
    cgpaIdx.insert(student);
    yearIdx.insert(student);

    // Keep the canonical copy in the flat records vector.
    records.push_back(student);
    return true;
}

// ── atomic remove ─────────────────────────────────────────────────────────────

bool MultiViewManager::removeStudent(const std::string& id) {
    // findById gives us the full Student — needed to reconstruct the tree keys.
    Student* s = findById(id);
    if (!s) return false;

    Student copy = *s; // capture before erasing from records

    // Remove from each index using the original field values.
    nameIdx.remove(copy.name, id);
    cgpaIdx.remove(copy.cgpa, id);
    yearIdx.remove(copy.batchYear, id);

    // Erase from the flat records vector.
    records.erase(
        std::remove_if(records.begin(), records.end(),
            [&](const Student& r) { return r.id == id; }),
        records.end());
    return true;
}

// ── single-view lookups ───────────────────────────────────────────────────────

// Delegates directly — no logic here, just routing to the right index.
std::vector<Student> MultiViewManager::searchByNamePrefix(const std::string& prefix) const
 {
    return nameIdx.prefixSearch(prefix);
}

std::vector<Student> MultiViewManager::searchByCGPARange(double low, double high) const 
{
    return cgpaIdx.getInRange(low, high);
}

std::vector<Student> MultiViewManager::searchByYear(int year) const 
{
    return yearIdx.getByYear(year);
}

std::vector<Student> MultiViewManager::searchByYearRange(int startYear, int endYear) const 
{
    return yearIdx.rangeByYear(startYear, endYear);
}

// ── cross-view query 

// Strategy: YearIndex is the primary filter (usually the smallest result set).
// We get the year-filtered list from the tree, then check CGPA and name prefix
// inline — no need for a separate full scan.
std::vector<Student> MultiViewManager::crossQuery(int year, double minCGPA, const std::string& namePrefix) const 
{
    std::vector<Student> byYear = yearIdx.getByYear(year);
    std::vector<Student> result;

    for (const Student& s : byYear) 
    {
        // CGPA filter
        if (s.cgpa < minCGPA) continue;

        // Case-insensitive name prefix filter
        if (!namePrefix.empty()) 
        {
            if (s.name.size() < namePrefix.size()) continue;
            bool match = true;
            for (size_t i = 0; i < namePrefix.size(); ++i) {
                if (std::tolower(s.name[i]) != std::tolower(namePrefix[i])) 
                {
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

//  display 

// getAllSorted() returns students in ascending name order (inorder on NameIndex).
void MultiViewManager::printAll() const 
{
    std::vector<Student> sorted = nameIdx.getAllSorted();
    for (const Student& s : sorted)
        std::cout << s.id << "\t" << s.name << "\tCGPA:" << s.cgpa
                  << "\t" << s.batchYear << "\t" << s.department << "\n";
}

// benchmark 
// TODO (Member 4 — Sohaib): insert n random students, then time:
//   • cgpaIdx.getInRange  vs  a sorted std::vector doing binary search
//   • nameIdx.prefixSearch vs  a linear scan on a std::vector
// Print wall-clock time for each using std::chrono::high_resolution_clock.
void MultiViewManager::runBenchmark(int n) const {
    (void)n;
}
