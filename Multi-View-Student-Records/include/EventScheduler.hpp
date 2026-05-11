#ifndef UNICORE_EVENT_SCHEDULER_HPP
#define UNICORE_EVENT_SCHEDULER_HPP

#include "RBTree.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>

struct Event {
    int id;
    std::string title;
    long long start;
    long long end;
    std::string location;
};

struct EventKey {
    long long start;
    int id;
    bool operator<(const EventKey& other) const {
        if (start != other.start) return start < other.start;
        return id < other.id;
    }
};

struct EventAugment {
    void update(RBNode<EventKey, Event>* node, RBNode<EventKey, Event>* nil) const {
        if (node == nil) return;
        node->subtreeSize = node->left->subtreeSize + node->right->subtreeSize + 1;
        node->maxEnd = std::max(node->value.end, std::max(node->left->maxEnd, node->right->maxEnd));
        node->subtreeBooks = 0;
    }
};

class EventScheduler {
private:
    RBTree<EventKey, Event, std::less<EventKey>, EventAugment> events;
    int nextId;

    static bool overlaps(long long s1, long long e1, long long s2, long long e2);
    void collectConflicts(typename RBTree<EventKey, Event, std::less<EventKey>, EventAugment>::Node* node,
                          long long start, long long end, std::vector<Event>& result) const;
    void collectRange(typename RBTree<EventKey, Event, std::less<EventKey>, EventAugment>::Node* node,
                      long long start, long long end, std::vector<Event>& result) const;

public:
    EventScheduler();
    bool addEvent(const std::string& title, long long start, long long end, const std::string& location);
    bool removeEvent(int id);
    bool removeEvent(int id, long long start); // compatibility overload
    bool hasConflict(long long start, long long end) const;
    bool findFirstConflict(long long start, long long end, Event& conflict) const;
    std::vector<Event> allConflicts(long long start, long long end) const;
    std::vector<Event> rangeQuery(long long dayStart, long long dayEnd) const;
    std::vector<Event> allEvents() const;
    Event* nextEvent();
    int size() const;
    void printAll() const;
};

#endif
