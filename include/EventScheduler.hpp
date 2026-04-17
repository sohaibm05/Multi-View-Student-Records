#ifndef EVENTSCHEDULER_HPP
#define EVENTSCHEDULER_HPP

#include "RBTree.hpp"
#include <string>

struct Event {
    std::string name;
    long long startTime;
    long long endTime;
};

class EventSchedulerTree : public RBTree<long long, Event> {
public:
    virtual void initializeNode(Node* node) override {
        if (node != NIL) node->maxEnd = node->value.endTime;
    }
    virtual void rotationHook(Node* node) override {
        if (node == NIL) return;
        long long self = node->value.endTime;
        node->maxEnd = std::max(self, std::max(node->left->maxEnd, node->right->maxEnd));
    }

    bool conflictCheck(long long s, long long e) const;
};

class EventScheduler {
public:
    void addEvent(const Event& e);
    EventSchedulerTree tree;
};

#endif