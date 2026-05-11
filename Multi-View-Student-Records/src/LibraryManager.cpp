#include "LibraryManager.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cctype>

LibraryManager::LibraryManager() : nextId(1) {}

static std::tm makeDateTm(long long yyyymmdd) {
    std::tm tm{};
    int year = static_cast<int>(yyyymmdd / 10000);
    int month = static_cast<int>((yyyymmdd / 100) % 100);
    int day = static_cast<int>(yyyymmdd % 100);
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = 12;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    tm.tm_isdst = -1;
    return tm;
}

static long long tmToYmd(const std::tm& tm) {
    long long year = tm.tm_year + 1900;
    long long month = tm.tm_mon + 1;
    long long day = tm.tm_mday;
    return year * 10000 + month * 100 + day;
}

static std::string lowerCopy(const std::string& text) {
    std::string out = text;
    for (char& c : out) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return out;
}

long long LibraryManager::addDaysToDate(long long yyyymmdd, int days) {
    std::tm tm = makeDateTm(yyyymmdd);
    std::time_t tt = std::mktime(&tm);
    if (tt == static_cast<std::time_t>(-1)) return yyyymmdd;
    tt += static_cast<std::time_t>(days) * 24 * 60 * 60;
    std::tm out{};
#ifdef _WIN32
    localtime_s(&out, &tt);
#else
    localtime_r(&tt, &out);
#endif
    return tmToYmd(out);
}


long long LibraryManager::pakToInternal(long long ddmmyyyy) {
    long long day = ddmmyyyy / 1000000;
    long long month = (ddmmyyyy / 10000) % 100;
    long long year = ddmmyyyy % 10000;
    return year * 10000 + month * 100 + day;
}

long long LibraryManager::internalToPak(long long yyyymmdd) {
    if (yyyymmdd == 0) return 0;
    long long year = yyyymmdd / 10000;
    long long month = (yyyymmdd / 100) % 100;
    long long day = yyyymmdd % 100;
    return day * 1000000 + month * 10000 + year;
}

std::string LibraryManager::displayDate(long long yyyymmdd) {
    if (yyyymmdd == 0) return "N/A";
    std::ostringstream out;
    long long pak = internalToPak(yyyymmdd);
    out << std::setw(8) << std::setfill('0') << pak;
    return out.str();
}

int LibraryManager::daysBetweenDates(long long startYyyymmdd, long long endYyyymmdd) {
    std::tm start = makeDateTm(startYyyymmdd);
    std::tm end = makeDateTm(endYyyymmdd);
    std::time_t startTime = std::mktime(&start);
    std::time_t endTime = std::mktime(&end);
    if (startTime == static_cast<std::time_t>(-1) || endTime == static_cast<std::time_t>(-1)) return 0;
    double seconds = std::difftime(endTime, startTime);
    return static_cast<int>(seconds / (24 * 60 * 60));
}

bool LibraryManager::addBook(const std::string& title, const std::string& author) {
    return addBook(title, author, 0, true);
}

bool LibraryManager::addBook(const std::string& title, const std::string& author, long long dueDate, bool available) {
    if (available) dueDate = 0;               // Available books do not have a due date.

    Book b{nextId, title, author, dueDate, available};
    bool ok = books.insert(BookDueKey{dueDate, nextId}, b);
    if (ok) nextId++;
    return ok;
}

typename RBTree<BookDueKey, Book, std::less<BookDueKey>, LibraryAugment>::Node*
LibraryManager::findNodeById(int id) const {
    auto nil = books.getNil();
    auto found = nil;
    books.inorder([&](auto node) {
        if (found == nil && node->value.id == id) found = node;
    });
    return found;
}

bool LibraryManager::checkoutBookByTitle(const std::string& title, long long dueDate) {
    dueDate = pakToInternal(dueDate);
    std::string target = lowerCopy(title);
    auto nil = books.getNil();
    auto found = nil;

    books.inorder([&](auto node) {
        if (found == nil && node->value.available && lowerCopy(node->value.title) == target) {
            found = node;
        }
    });

    if (found == nil) return false;

    Book updated = found->value;
    books.erase(found->key);
    updated.available = false;
    updated.dueDate = dueDate;
    return books.insert(BookDueKey{updated.dueDate, updated.id}, updated);
}

bool LibraryManager::checkoutBook(int id, long long dueDate) {
    dueDate = pakToInternal(dueDate);
    auto node = findNodeById(id);
    if (node == books.getNil() || !node->value.available) return false;

    Book updated = node->value;
    books.erase(node->key);
    updated.available = false;
    updated.dueDate = dueDate;
    return books.insert(BookDueKey{updated.dueDate, updated.id}, updated);
}

bool LibraryManager::checkoutBook(int id) {
    using namespace std::chrono;
    auto now = system_clock::now();
    std::time_t tt = system_clock::to_time_t(now);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &tt);
#else
    localtime_r(&tt, &tm);
#endif
    return checkoutBook(id, internalToPak(addDaysToDate(tmToYmd(tm), 30)));
}

bool LibraryManager::returnBookByTitle(const std::string& title) {
    std::string target = lowerCopy(title);
    auto nil = books.getNil();
    auto found = nil;

    books.inorder([&](auto node) {
        if (found == nil && !node->value.available && lowerCopy(node->value.title) == target) {
            found = node;
        }
    });

    if (found == nil) return false;

    Book updated = found->value;
    books.erase(found->key);
    updated.available = true;
    updated.dueDate = 0; // Back to N/A after return.
    return books.insert(BookDueKey{updated.dueDate, updated.id}, updated);
}

bool LibraryManager::returnBook(int id) {
    auto node = findNodeById(id);
    if (node == books.getNil() || node->value.available) return false;

    Book updated = node->value;
    books.erase(node->key);
    updated.available = true;
    updated.dueDate = 0; // Back to N/A after return.
    return books.insert(BookDueKey{updated.dueDate, updated.id}, updated);
}

bool LibraryManager::returnBook(int id, long long /*ignoredDate*/) {
    return returnBook(id);
}

int LibraryManager::availableCount() const {
    return books.getRoot()->subtreeBooks;
}

void LibraryManager::collectCheckedOutDueRange(
    typename RBTree<BookDueKey, Book, std::less<BookDueKey>, LibraryAugment>::Node* node,
    long long d1, long long d2, std::vector<Book>& result) const {

    auto nil = books.getNil();
    if (node == nil) return;

    if (node->key.dueDate >= d1) collectCheckedOutDueRange(node->left, d1, d2, result);
    if (!node->value.available && node->value.dueDate >= d1 && node->value.dueDate <= d2) {
        result.push_back(node->value);
    }
    if (node->key.dueDate <= d2) collectCheckedOutDueRange(node->right, d1, d2, result);
}

std::vector<Book> LibraryManager::checkedOutDueInRange(long long d1, long long d2) const {
    d1 = pakToInternal(d1);
    d2 = pakToInternal(d2);
    std::vector<Book> ans;
    if (d1 > d2) return ans;
    collectCheckedOutDueRange(books.getRoot(), d1, d2, ans);
    return ans;
}

void LibraryManager::collectOverdue(
    typename RBTree<BookDueKey, Book, std::less<BookDueKey>, LibraryAugment>::Node* node,
    long long today, std::vector<Book>& result) const {

    auto nil = books.getNil();
    if (node == nil) return;

    collectOverdue(node->left, today, result);
    if (!node->value.available && node->value.dueDate != 0 && node->value.dueDate < today) {
        result.push_back(node->value);
    }
    if (node->key.dueDate < today) collectOverdue(node->right, today, result);
}

std::vector<Book> LibraryManager::overdueBooks(long long today) const {
    today = pakToInternal(today);
    std::vector<Book> ans;
    collectOverdue(books.getRoot(), today, ans);
    return ans;
}

std::vector<Book> LibraryManager::allBooks() const {
    std::vector<Book> ans;
    books.inorder([&](auto node) { ans.push_back(node->value); });
    return ans;
}

int LibraryManager::estimateFine(const Book& book, long long today, int finePerDay) const {
    today = pakToInternal(today);
    if (book.available || book.dueDate == 0 || today <= book.dueDate) return 0;
    int overdueDays = daysBetweenDates(book.dueDate, today);
    if (overdueDays < 0) overdueDays = 0;
    return overdueDays * finePerDay;
}

int LibraryManager::size() const { return books.size(); }

void LibraryManager::printAll() const {
    books.inorder([](auto node) {
        const Book& b = node->value;
        std::cout << b.id << " | " << b.title << " | " << b.author << " | due ";
        if (b.available) std::cout << "N/A";
        else std::cout << displayDate(b.dueDate);
        std::cout << " | " << (b.available ? "Available" : "Checked out") << "\n";
    });
}
