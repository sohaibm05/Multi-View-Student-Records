#include "CourseRegistration.hpp"

void CourseRegistration::addToWaitlist(const std::string& id, double cgpa, int batch, long long time) {
    PriorityKey key{ cgpa, batch, time };
    WaitlistEntry entry{ id };
    waitlist.insert(key, entry);
}

std::string CourseWaitlist::promoteFront() {
    Node* node = front();
    if (node == NIL || node == nullptr) return "";
    std::string id = node->value.studentId;
    remove(node->key);
    return id;
}