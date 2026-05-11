#include "CourseRegistration.hpp"

CourseRegistration::CourseRegistration() : currentTime(1) {}

WaitlistKey CourseRegistration::makeKey(const WaitlistEntry& e) {
    return WaitlistKey{e.courseCode, -e.cgpa, e.batchYear, e.timestamp, e.studentId};
}

bool CourseRegistration::addToWaitlist(const std::string& courseCode, const Student& student) {
    // Reject duplicate: same student already waiting for the same course.
    std::vector<WaitlistEntry> existing = courseWaitlist(courseCode);
    for (const WaitlistEntry& e : existing)
        if (e.studentId == student.id) return false;
    WaitlistEntry e{student.id, courseCode, student.cgpa, student.batchYear, currentTime++};
    return waitlist.insert(makeKey(e), e);
}

bool CourseRegistration::removeFromWaitlist(const std::string& courseCode, const Student& student) {
    std::vector<WaitlistEntry> entries = courseWaitlist(courseCode);
    for (const WaitlistEntry& e : entries) {
        if (e.studentId == student.id) return waitlist.erase(makeKey(e));
    }
    return false;
}

bool CourseRegistration::updateStudentPriority(const std::string& courseCode, const Student& oldStudent, const Student& newStudent) {
    std::vector<WaitlistEntry> entries = courseWaitlist(courseCode);
    for (WaitlistEntry e : entries) {
        if (e.studentId == oldStudent.id) {
            waitlist.erase(makeKey(e));
            e.cgpa = newStudent.cgpa;
            e.batchYear = newStudent.batchYear;
            return waitlist.insert(makeKey(e), e);
        }
    }
    return false;
}

WaitlistEntry* CourseRegistration::front(const std::string& courseCode) {
    WaitlistKey lo{courseCode, -5.0, std::numeric_limits<int>::min(), std::numeric_limits<long long>::min(), std::numeric_limits<int>::min()};
    auto node = waitlist.lowerBoundNode(lo);
    if (node == waitlist.getNil()) return nullptr;
    if (node->value.courseCode != courseCode) return nullptr;
    return &node->value;
}

bool CourseRegistration::promoteFront(const std::string& courseCode, WaitlistEntry& promoted) {
    WaitlistEntry* f = front(courseCode);
    if (f == nullptr) return false;
    promoted = *f;
    return waitlist.erase(makeKey(promoted));
}

std::vector<WaitlistEntry> CourseRegistration::courseWaitlist(const std::string& courseCode) const {
    std::vector<WaitlistEntry> ans;
    waitlist.inorder([&](auto node) {
        if (node->value.courseCode == courseCode) ans.push_back(node->value);
    });
    return ans;
}

std::vector<WaitlistEntry> CourseRegistration::topWaitlistedFromBatch(const std::string& courseCode, int batchYear, int limit) const {
    std::vector<WaitlistEntry> ans;
    waitlist.inorder([&](auto node) {
        if ((int)ans.size() >= limit) return;
        if (node->value.courseCode == courseCode && node->value.batchYear == batchYear) ans.push_back(node->value);
    });
    return ans;
}

int CourseRegistration::size() const { return waitlist.size(); }

void CourseRegistration::printAll() const {
    waitlist.inorder([](auto node) {
        const WaitlistEntry& e = node->value;
        std::cout << e.courseCode << " | Student " << e.studentId
                  << " | CGPA " << e.cgpa << " | Batch " << e.batchYear
                  << " | Time " << e.timestamp << "\n";
    });
}
