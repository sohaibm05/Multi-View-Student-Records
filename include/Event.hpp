#pragma once
#include <string>
#include <ctime>

// =============================================================================
// Event  —  core data record for the Event Scheduler module  (Member 2 — Krish)
//
// std::time_t is seconds since the Unix epoch (1970-01-01 00:00:00 UTC).
// Use std::mktime / std::localtime to convert to/from human-readable dates.
// =============================================================================

struct Event {
    int         id;         // unique event ID — used as a tiebreaker if two events share a start time
    std::string title;      // human-readable name, e.g. "CS201 Lecture"
    std::string organizer;  // person or department running the event
    std::time_t start;      // start time (inclusive)
    std::time_t end;        // end   time (exclusive) — event occupies [start, end)
};
