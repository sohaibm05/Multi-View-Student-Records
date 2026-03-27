#pragma once
#include <string>
#include <ctime>

// Member 3 — Library Manager (Saad)

struct Book {
    std::string isbn;
    std::string title;
    std::string author;
    std::time_t dueDate;
    bool        checkedOut;
};
