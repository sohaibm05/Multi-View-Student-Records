// Member 4 — Course Registration & Integration (Sohaib)

#include "CourseRegistration.hpp"

// ── Priority comparator ───────────────────────────────────────────────────────
// The comparator defines what "leftmost node" means for our waitlist.
// Returning true means  *this  should sit to the LEFT of  o  in the tree
// (i.e., *this has higher priority and comes out first via promoteFront).
//
// Rule 1: higher CGPA → higher priority
//   if cgpa != o.cgpa → return cgpa > o.cgpa
//   (4.0 > 3.9 → true → 4.0 goes left → 4.0 comes out first ✓)
//
// Rule 2: older batch year → higher priority (they've waited longer)
//   if batchYear != o.batchYear → return batchYear < o.batchYear
//   (2020 < 2022 → true → 2020 batch goes left → comes out first ✓)
//
// Rule 3: earlier signup → higher priority (first-come, first-served)
//   return timestamp < o.timestamp
bool CourseRegistration::Priority::operator<(const Priority& o) const {
    if (cgpa != o.cgpa)           return cgpa > o.cgpa;
    if (batchYear != o.batchYear) return batchYear < o.batchYear;
    return timestamp < o.timestamp;
}

// Used by updateCGPA to locate the exact old key for removal.
bool CourseRegistration::Priority::operator==(const Priority& o) const {
    return cgpa == o.cgpa && batchYear == o.batchYear && timestamp == o.timestamp;
}

// ── Constructor ───────────────────────────────────────────────────────────────
CourseRegistration::CourseRegistration(const std::string& id) : courseId(id) {}

// ── enqueue ───────────────────────────────────────────────────────────────────
// Build a Priority from the student's current fields + std::time(nullptr) as the
// signup timestamp.  Wrap everything in a WaitlistEntry and insert into the tree.
// The tree's BST ordering (via Priority::operator<) automatically places this
// student in the correct priority position — no extra sorting needed.
void CourseRegistration::enqueue(const Student& student) {
    Priority p{ student.cgpa, student.batchYear, std::time(nullptr) };
    tree.insert(p, WaitlistEntry{ student, courseId, p });
}

// ── promoteFront ──────────────────────────────────────────────────────────────
// tree.front() returns the leftmost (minimum-key) node = highest-priority student.
// We capture the WaitlistEntry, then remove the node from the tree.
// The tree re-balances automatically; the next promoteFront call will find the
// new leftmost node in O(log n).
CourseRegistration::WaitlistEntry CourseRegistration::promoteFront() {
    auto* node = tree.front(); // leftmost node = highest priority
    if (!node) return {};      // empty waitlist
    WaitlistEntry entry = node->value;
    tree.remove(node->key);
    return entry;
}

// ── updateCGPA ────────────────────────────────────────────────────────────────
// A student's CGPA changed → their position in the priority queue must change.
// We can't update a key in-place (that would break BST ordering), so we:
//   1. Scan the tree to find the old entry by student ID.
//   2. Remove it (old key is gone from the tree).
//   3. Reinsert with a new Priority that has the updated CGPA but the SAME
//      original timestamp (signup time should not change).
void CourseRegistration::updateCGPA(const std::string& studentId, double newCGPA) {
    WaitlistEntry found;
    Priority      foundKey{};
    bool          exists = false;

    // O(n) scan — acceptable since re-prioritization is rare.
    // If it becomes a bottleneck, maintain a separate hash map: id → Priority.
    tree.inorder([&](const Priority& k, const WaitlistEntry& v) {
        if (!exists && v.student.id == studentId) {
            found    = v;
            foundKey = k;
            exists   = true;
        }
    });

    if (!exists) return;

    // Remove old entry, update CGPA, reinsert at new priority position.
    tree.remove(foundKey);
    found.student.cgpa = newCGPA;
    found.priority     = { newCGPA, found.student.batchYear, foundKey.timestamp };
    tree.insert(found.priority, found);
}

// ── topN ─────────────────────────────────────────────────────────────────────
// inorder gives nodes in ascending key order = descending priority order.
// So the first n nodes visited are the top-n highest-priority students.
std::vector<CourseRegistration::WaitlistEntry> CourseRegistration::topN(int n) const {
    std::vector<WaitlistEntry> result;
    tree.inorder([&](const Priority&, const WaitlistEntry& v) {
        if (static_cast<int>(result.size()) < n)
            result.push_back(v);
    });
    return result;
}

// ── topNByYear ────────────────────────────────────────────────────────────────
// Cross-system query: same inorder walk as topN but adds a batchYear filter.
// Called by main.cpp to answer "top N waitlisted students from batch YYYY".
// Note: n refers to the number of MATCHING results, not total nodes visited.
std::vector<CourseRegistration::WaitlistEntry>
CourseRegistration::topNByYear(int n, int batchYear) const {
    std::vector<WaitlistEntry> result;
    tree.inorder([&](const Priority&, const WaitlistEntry& v) {
        if (static_cast<int>(result.size()) < n && v.student.batchYear == batchYear)
            result.push_back(v);
    });
    return result;
}

int  CourseRegistration::size()  const { return tree.size(); }
bool CourseRegistration::empty() const { return tree.empty(); }
