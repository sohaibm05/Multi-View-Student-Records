// Member 2 — EventScheduler (Krish)

#include "EventScheduler.hpp"

EventScheduler::EventScheduler() = default;

// Key = event.start.
// If two events can share a start time, encode event.id into the key
// (e.g. use a pair<time_t, int> as the Key type, or use time_t * BIG + id).
// Wrap the event in AugNode(event) — constructor sets maxEnd = event.end.
// Call tree.insert(event.start, augNode).
// IMPORTANT: after insertFixup rotates, leftRotate/rightRotate must update maxEnd.
void EventScheduler::scheduleEvent(const Event& event) {}

// Scan the tree (via inorder) to find the node whose event.id == id.
// Once found, call tree.remove(foundKey).
// Returns false if no event with that id exists.
bool EventScheduler::cancelEvent(int id) { return false; }

// Iterative interval-overlap walk (O(log n)):
//   node = root
//   while node != NIL:
//     if node.event overlaps [start, end)  →  return true
//       (overlap condition: node.event.start < end  AND  start < node.event.end)
//     if node->left != NIL AND node->left->value.maxEnd >= start
//         → go left (left subtree might contain an overlapping event)
//     else
//         → go right
//   return false  (no overlap found)
bool EventScheduler::conflictCheck(std::time_t start, std::time_t end) const { return false; }

// Recursive (or iterative) version of conflictCheck that collects ALL matches.
// At each node:
//   if node.event overlaps [start, end) → add to results
//   if node->left != NIL AND node->left.maxEnd >= start → recurse left
//   if node->right != NIL AND node->right.maxEnd >= start → recurse right
//     (cannot prune right based on maxEnd alone when collecting all conflicts)
std::vector<Event> EventScheduler::allConflicts(std::time_t start, std::time_t end) const { return {}; }

// Return all events that intersect the date range [dayStart, dayEnd].
// An event intersects if: event.start <= dayEnd AND event.end >= dayStart
// Walk the tree; prune any subtree where subtree.maxEnd < dayStart
// (nothing in that subtree ends late enough to reach dayStart).
std::vector<Event> EventScheduler::rangeQuery(std::time_t dayStart, std::time_t dayEnd) const { return {}; }

// Return the soonest upcoming event = minimum-start-time node = leftmost node.
// Call tree.front() and return its value.event.
// If the tree is empty, return a default-constructed Event.
Event EventScheduler::nextEvent() const { return {}; }
