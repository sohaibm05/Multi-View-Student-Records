#include "CourseRegistration.hpp"

CourseRegistration::CourseRegistration() : currentTime(1) {}

WaitlistKey CourseRegistration::makeKey(const WaitlistEntry& e) 
{
    return WaitlistKey{e.courseCode, -e.cgpa, e.batchYear, e.timestamp, e.studentId};
}

bool CourseRegistration::addToWaitlist(const std::string& courseCode, const Student& student) 
{
    if (index.contains(CourseStudentKey{courseCode, student.id}))
    {
        return false;
    }
    WaitlistEntry e{student.id, courseCode, student.cgpa, student.batchYear, currentTime++};
    WaitlistKey k = makeKey(e);
    index.insert(CourseStudentKey{courseCode, student.id}, k);
    return waitlist.insert(k, e);
}

bool CourseRegistration::removeFromWaitlist(const std::string& courseCode, const Student& student) 
{
    WaitlistKey* k = index.find(CourseStudentKey{courseCode, student.id});
    if (k == nullptr)
    {
        return false;
    }
    waitlist.erase(*k);
    index.erase(CourseStudentKey{courseCode, student.id});
    return true;
}

bool CourseRegistration::updateStudentPriority(const std::string& courseCode, const Student& oldStudent, const Student& newStudent) 
{
    CourseStudentKey ck{courseCode, oldStudent.id};
    WaitlistKey* storedKey = index.find(ck);
    if (storedKey == nullptr)
    { 
        return false;
    }
    WaitlistEntry* entry = waitlist.find(*storedKey);
    if (entry == nullptr)
    { 
        return false;
    }
    WaitlistEntry updated = *entry;
    WaitlistKey oldKey = *storedKey;
    waitlist.erase(oldKey);
    updated.cgpa = newStudent.cgpa;
    updated.batchYear = newStudent.batchYear;
    WaitlistKey newKey = makeKey(updated);
    *storedKey = newKey;
    return waitlist.insert(newKey, updated);
}

WaitlistEntry* CourseRegistration::front(const std::string& courseCode) 
{
    WaitlistKey lo{courseCode, -5.0, std::numeric_limits<int>::min(), std::numeric_limits<long long>::min(), std::numeric_limits<int>::min()};
    auto node = waitlist.lowerBoundNode(lo);
    if (node == waitlist.getNil())
    {
        return nullptr;
    }
    if (node->value.courseCode != courseCode) 
    {
        return nullptr;
    }
    return &node->value;
}

bool CourseRegistration::promoteFront(const std::string& courseCode, WaitlistEntry& promoted) 
{
    WaitlistEntry* f = front(courseCode);
    if (f == nullptr)
    { 
        return false;
    }
    promoted = *f;
    index.erase(CourseStudentKey{courseCode, promoted.studentId});
    return waitlist.erase(makeKey(promoted));
}

std::vector<WaitlistEntry> CourseRegistration::courseWaitlist(const std::string& courseCode) const 
{
    std::vector<WaitlistEntry> ans;
    waitlist.inorder([&](auto node) 
    {
        if (node->value.courseCode == courseCode) ans.push_back(node->value);
    });
    return ans;
}

std::vector<WaitlistEntry> CourseRegistration::allWaitlistEntries() const 
{
    std::vector<WaitlistEntry> ans;
    waitlist.inorder([&](auto node) { ans.push_back(node->value); });
    return ans;
}

std::vector<WaitlistEntry> CourseRegistration::topWaitlistedFromBatch(const std::string& courseCode, int batchYear, int limit) const {
    std::vector<WaitlistEntry> ans;
    waitlist.inorder([&](auto node) 
    {
        if ((int)ans.size() >= limit)
        { 
            return;
        }
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
