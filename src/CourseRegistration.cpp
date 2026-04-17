// src/CourseRegistration.cpp
#include "CourseRegistration.hpp"

void CourseRegistration::addToWaitlist(const std::string& id, double cgpa, int batch, long long time) {
    PriorityKey key{ cgpa, batch, time };
    WaitlistEntry entry{ id };
    waitlist.insert(key, entry);
}

std::string CourseWaitlist::promoteFront() {
    Node* node = front();                    // RBTree::front()

    // Fixed: use this->NIL (protected member of base class)
    if (node == nullptr || node == NIL) {
        return "";
    }

    std::string id = node->value.studentId;
    remove(node->key);                       // RBTree::remove()
    return id;
}