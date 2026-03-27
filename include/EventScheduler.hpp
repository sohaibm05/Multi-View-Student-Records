#pragma once
#include "RBTree.hpp"
#include "Event.hpp"
#include <vector>
#include <ctime>

// =============================================================================
// EventScheduler  —  interval tree for conflict detection  (Member 2 — Krish)
//
// Uses a RBTree<time_t, AugNode> keyed by event start time.
// Each node stores the event's [start, end) interval plus a maxEnd field,
// which equals the largest end value anywhere in that node's subtree.
//
// AUGMENTATION — maxEnd
//   maxEnd(node) = max( node.event.end,
//                       maxEnd(node->left),
//                       maxEnd(node->right) )
//   maxEnd(NIL)  = 0  (or TIME_MIN — any value smaller than all real ends)
//
//   Update rule (must be called inside leftRotate and rightRotate, on the
//   node that went DOWN first, then on the node that went UP):
//     node->value.maxEnd = max(node->value.event.end,
//                              node->left->value.maxEnd,
//                              node->right->value.maxEnd)
//
// This augmentation enables O(log n) overlap queries:
//   "Does any existing event overlap [query.start, query.end)?"
//   → At each node: if node.event overlaps → return true
//                   if node->left != NIL and node->left.maxEnd >= query.start
//                       → recurse left (there might be an overlap there)
//                   else → recurse right
// =============================================================================

class EventScheduler {
public:
    struct AugNode {
        Event       event;
        std::time_t maxEnd; // largest end value in this subtree (augmentation)

        AugNode() : maxEnd(0) {}
        explicit AugNode(const Event& e) : event(e), maxEnd(e.end) {}
    };

    EventScheduler();

    // Insert event into the tree.
    // Key = event.start (if two events share a start time, encode the id into
    // the key or use a composite key to avoid collisions).
    // After insert, insertFixup will rotate; ensure maxEnd is updated in rotations.
    void scheduleEvent(const Event& event);

    // Find and remove the event with the given id.
    // You'll need to scan (inorder) or maintain a separate id→key map.
    // Returns false if not found.
    bool cancelEvent(int id);

    // Return true if any existing event overlaps the interval [start, end).
    // Two intervals [a,b) and [c,d) overlap iff a < d && c < b.
    // Algorithm (O(log n)):
    //   node = root
    //   while node != NIL:
    //     if overlaps(node.event, start, end) → return true
    //     if node->left != NIL and node->left.maxEnd >= start → node = node->left
    //     else → node = node->right
    bool conflictCheck(std::time_t start, std::time_t end) const;

    // Return ALL events that overlap [start, end).
    // Same walk as conflictCheck but collect every match instead of stopping.
    // O(k log n) where k = number of overlapping events.
    std::vector<Event> allConflicts(std::time_t start, std::time_t end) const;

    // Return all events whose interval intersects [dayStart, dayEnd].
    // Prune branches where node->left.maxEnd < dayStart (nothing in that
    // subtree can end late enough to overlap).
    // O(k log n)
    std::vector<Event> rangeQuery(std::time_t dayStart, std::time_t dayEnd) const;

    // Return the soonest upcoming event (the leftmost node = minimum start time).
    // Delegates to tree.front()->value.event.
    // O(log n)
    Event nextEvent() const;

private:
    // Key: event.start.  If collisions on start time are possible,
    // encode event.id into the key (e.g., as a pair or composite).
    RBTree<std::time_t, AugNode> tree;
};
