// Member 2 — Event Scheduler (Krish)

#include "EventScheduler.hpp"

EventScheduler::EventScheduler() = default;

void EventScheduler::scheduleEvent(const Event& event) {}

bool EventScheduler::cancelEvent(int id) { return false; }

bool EventScheduler::conflictCheck(std::time_t start, std::time_t end) const { return false; }

std::vector<Event> EventScheduler::allConflicts(std::time_t start, std::time_t end) const { return {}; }

std::vector<Event> EventScheduler::rangeQuery(std::time_t dayStart, std::time_t dayEnd) const { return {}; }

Event EventScheduler::nextEvent() const { return {}; }
