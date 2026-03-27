#pragma once
#include "RBTree.hpp"
#include "Event.hpp"
#include <vector>
#include <ctime>

// Member 2 — Event Scheduler (Krish)
// RBT keyed by event start time; each node stores maxEnd for O(log n) conflict detection

class EventScheduler {
public:
    struct AugNode {
        Event       event;
        std::time_t maxEnd; // largest end value anywhere in the subtree
        AugNode() : maxEnd(0) {}
        explicit AugNode(const Event& e) : event(e), maxEnd(e.end) {}
    };

    EventScheduler();

    void scheduleEvent(const Event& event);
    bool cancelEvent(int id);

    // Returns true if any existing event overlaps [start, end)
    bool conflictCheck(std::time_t start, std::time_t end) const;

    // Returns all events that overlap [start, end)
    std::vector<Event> allConflicts(std::time_t start, std::time_t end) const;

    // Returns all events whose interval intersects [dayStart, dayEnd]
    std::vector<Event> rangeQuery(std::time_t dayStart, std::time_t dayEnd) const;

    // Returns the soonest upcoming event (leftmost node)
    Event nextEvent() const;

private:
    RBTree<std::time_t, AugNode> tree;
};
