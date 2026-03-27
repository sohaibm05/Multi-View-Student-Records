// Member 3 — Library Manager (Saad)

#include "LibraryManager.hpp"

LibraryManager::LibraryManager() = default;

void LibraryManager::addBook(const Book& book) {}

bool LibraryManager::removeBook(const std::string& isbn) { return false; }

bool LibraryManager::checkOut(const std::string& isbn) { return false; }

bool LibraryManager::returnBook(const std::string& isbn) { return false; }

int LibraryManager::availableInRange(std::time_t d1, std::time_t d2) const { return 0; }

std::vector<Book> LibraryManager::overdueBooks(std::time_t today) const { return {}; }

double LibraryManager::estimateFine(const Book& book, std::time_t today) const { return 0.0; }
