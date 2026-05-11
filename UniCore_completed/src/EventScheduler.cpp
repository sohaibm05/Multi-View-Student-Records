#include "EventScheduler.hpp"

EventScheduler::EventScheduler() : nextId(1) {}

bool EventScheduler::overlaps(long long s1, long long e1, long long s2, long long e2) {
    return s1 < e2 && s2 < e1;
}

bool EventScheduler::addEvent(const std::string& title, long long start, long long end, const std::string& location) {
    if (end <= start) return false;
    if (hasConflict(start, end)) return false;
    Event ev{nextId, title, start, end, location};
    bool ok = events.insert(EventKey{start, nextId}, ev);
    if (ok) nextId++;
    return ok;
}

bool EventScheduler::removeEvent(int id, long long start) {
    return events.erase(EventKey{start, id});
}

bool EventScheduler::hasConflict(long long start, long long end) const {
    Event conflict;
    return findFirstConflict(start, end, conflict);
}

bool EventScheduler::findFirstConflict(long long start, long long end, Event& conflict) const {
    auto nil = events.getNil();
    auto x = events.getRoot();
    while (x != nil) {
        if (overlaps(x->value.start, x->value.end, start, end)) {
            conflict = x->value;
            return true;
        }
        if (x->left != nil && x->left->maxEnd > start) x = x->left;
        else x = x->right;
    }
    return false;
}

void EventScheduler::collectConflicts(typename RBTree<EventKey, Event, std::less<EventKey>, EventAugment>::Node* node,
                                      long long start, long long end, std::vector<Event>& result) const {
    auto nil = events.getNil();
    if (node == nil) return;
    if (node->left != nil && node->left->maxEnd > start) collectConflicts(node->left, start, end, result);
    if (overlaps(node->value.start, node->value.end, start, end)) result.push_back(node->value);
    if (node->value.start < end) collectConflicts(node->right, start, end, result);
}

std::vector<Event> EventScheduler::allConflicts(long long start, long long end) const {
    std::vector<Event> ans;
    collectConflicts(events.getRoot(), start, end, ans);
    return ans;
}

void EventScheduler::collectRange(typename RBTree<EventKey, Event, std::less<EventKey>, EventAugment>::Node* node,
                                  long long start, long long end, std::vector<Event>& result) const {
    auto nil = events.getNil();
    if (node == nil) return;
    if (node->left != nil && node->left->maxEnd >= start) collectRange(node->left, start, end, result);
    if (node->value.start >= start && node->value.start <= end) result.push_back(node->value);
    if (node->value.start <= end) collectRange(node->right, start, end, result);
}

std::vector<Event> EventScheduler::rangeQuery(long long dayStart, long long dayEnd) const {
    std::vector<Event> ans;
    collectRange(events.getRoot(), dayStart, dayEnd, ans);
    return ans;
}

std::vector<Event> EventScheduler::allEvents() const {
    std::vector<Event> ans;
    events.inorder([&](auto node) { ans.push_back(node->value); });
    return ans;
}

Event* EventScheduler::nextEvent() {
    auto node = events.minimum(events.getRoot());
    if (node == events.getNil()) return nullptr;
    return &node->value;
}

int EventScheduler::size() const { return events.size(); }

void EventScheduler::printAll() const {
    events.inorder([](auto node) {
        const Event& e = node->value;
        std::cout << e.id << " | " << e.title << " | " << e.start << "-" << e.end
                  << " | " << e.location << "\n";
    });
}
