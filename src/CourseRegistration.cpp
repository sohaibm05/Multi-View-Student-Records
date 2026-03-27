// Member 4 — Course Registration & Integration (Sohaib)

#include "CourseRegistration.hpp"

// ---- Priority comparator ----
// Leftmost node = highest priority student.
// Higher CGPA wins; ties broken by older batch year, then earlier signup.

bool CourseRegistration::Priority::operator<(const Priority& o) const {
    if (cgpa != o.cgpa)           return cgpa > o.cgpa;       // higher CGPA → left
    if (batchYear != o.batchYear) return batchYear < o.batchYear; // older batch → left
    return timestamp < o.timestamp;                            // earlier signup → left
}

bool CourseRegistration::Priority::operator==(const Priority& o) const {
    return cgpa == o.cgpa && batchYear == o.batchYear && timestamp == o.timestamp;
}

// ---- constructor ----

CourseRegistration::CourseRegistration(const std::string& id) : courseId(id) {}

// ---- waitlist operations ----

void CourseRegistration::enqueue(const Student& student) {
    Priority p{ student.cgpa, student.batchYear, std::time(nullptr) };
    tree.insert(p, WaitlistEntry{ student, courseId, p });
}

// Pop and return the highest-priority (leftmost) student.
CourseRegistration::WaitlistEntry CourseRegistration::promoteFront() {
    auto* node = tree.front();
    if (!node) return {};
    WaitlistEntry entry = node->value;
    tree.remove(node->key);
    return entry;
}

// Re-prioritize: remove by scanning for student ID, reinsert with new CGPA.
void CourseRegistration::updateCGPA(const std::string& studentId, double newCGPA) {
    WaitlistEntry found;
    Priority      foundKey{};
    bool          exists = false;

    tree.inorder([&](const Priority& k, const WaitlistEntry& v) {
        if (!exists && v.student.id == studentId) {
            found    = v;
            foundKey = k;
            exists   = true;
        }
    });

    if (!exists) return;

    tree.remove(foundKey);
    found.student.cgpa = newCGPA;
    found.priority     = { newCGPA, found.student.batchYear, foundKey.timestamp };
    tree.insert(found.priority, found);
}

// ---- queries ----

std::vector<CourseRegistration::WaitlistEntry> CourseRegistration::topN(int n) const {
    std::vector<WaitlistEntry> result;
    tree.inorder([&](const Priority&, const WaitlistEntry& v) {
        if (static_cast<int>(result.size()) < n)
            result.push_back(v);
    });
    return result;
}

// Cross-subsystem helper: top N waitlisted from a specific batch year.
// Called by main.cpp to combine the waitlist with Student Records year data.
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
