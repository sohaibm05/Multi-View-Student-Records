// src/CourseRegistration.cpp
#include "CourseRegistration.hpp"
#include <vector>

void CourseRegistration::addToWaitlist(const std::string& id, double cgpa, int batch, long long time) {
    PriorityKey key{ cgpa, batch, time };
    WaitlistEntry entry{ id };
    waitlist.insert(key, entry);
}

// Removes the student's old waitlist entry and reinserts with updated priority fields.
// This is necessary because the RBTree is sorted by key — changing key fields in-place
// would silently break BST ordering. Remove + reinsert is the only correct approach.
void CourseRegistration::reprioritize(const std::string& id,
                                      double oldCgpa, int oldBatch, long long oldTime,
                                      double newCgpa, int newBatch, long long newTime) {
    PriorityKey oldKey{ oldCgpa, oldBatch, oldTime };
    waitlist.remove(oldKey);

    PriorityKey newKey{ newCgpa, newBatch, newTime };
    waitlist.insert(newKey, WaitlistEntry{ id });
}

// Returns up to k student IDs from the waitlist who belong to batchYear,
// in descending priority order (highest CGPA first, then earlier batch, then earlier signup).
// inorder() visits nodes from smallest PriorityKey to largest, and because the comparator
// puts higher CGPA as "smaller", inorder = highest priority first.
std::vector<std::string> CourseRegistration::topKFromBatch(int batchYear, size_t k) const {
    std::vector<std::string> result;
    waitlist.inorder([&](const PriorityKey& key, const WaitlistEntry& entry) {
        if (result.size() < k && key.batchYear == batchYear)
            result.push_back(entry.studentId);
    });
    return result;
}

std::string CourseWaitlist::promoteFront() {
    Node* node = front();                    // RBTree::front()

    if (node == nullptr || node == NIL) {
        return "";
    }

    std::string id = node->value.studentId;
    remove(node->key);                       // RBTree::remove()
    return id;
}