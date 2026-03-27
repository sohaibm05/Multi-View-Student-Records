#pragma once
#include <string>
#include <ctime>

// =============================================================================
// Book  —  core data record for the Library Manager module  (Member 3 — Saad)
//
// The RBT in LibraryManager is keyed by dueDate so that overdue queries
// are a simple left-subtree walk.
// =============================================================================

struct Book {
    std::string isbn;        // unique identifier, e.g. "978-0-13-284737-7"
    std::string title;
    std::string author;
    std::time_t dueDate;     // Unix timestamp of the return deadline
    bool        checkedOut;  // true = currently borrowed, false = on the shelf
};
