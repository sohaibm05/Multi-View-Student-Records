#pragma once
#include <string>
#include <ctime>

// Member 2 — Event Scheduler (Krish)

struct Event {
    int         id;
    std::string title;
    std::string organizer;
    std::time_t start;
    std::time_t end;
};
