#include "EventScheduler.hpp"

void EventScheduler::addEvent(const Event& e) {
    tree.insert(e.startTime, e);
}

bool EventSchedulerTree::conflictCheck(long long s, long long e) const {
    Node* x = root;
    while (x != NIL) {
        // check current
        if (!(x->value.endTime < s || e < x->value.startTime)) return true;
        // prune
        if (x->left != NIL && x->left->maxEnd >= s)
            x = x->left;
        else
            x = x->right;
    }
    return false;
}