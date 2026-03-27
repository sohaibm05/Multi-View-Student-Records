// Member 4 — UniCore CLI & Integration (Sohaib)

#include "MultiViewManager.hpp"
#include "EventScheduler.hpp"
#include "LibraryManager.hpp"
#include "CourseRegistration.hpp"
#include <iostream>
#include <string>
#include <limits>

// ── helpers ──────────────────────────────────────────────────────────────────

static void clearInput() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static void printStudents(const std::vector<Student>& v) {
    if (v.empty()) { std::cout << "  (none)\n"; return; }
    for (const auto& s : v)
        std::cout << "  " << s.id << "  " << s.name
                  << "  CGPA:" << s.cgpa << "  Year:" << s.batchYear << "\n";
}

// ── cross-system query ────────────────────────────────────────────────────────
// "Top N waitlisted students from batch year Y"
// Combines Module 1 (year-filtered student data) with Module 4 (waitlist order).

static void crossTopWaitlisted(const CourseRegistration& cr,
                                const MultiViewManager&   sr) {
    std::string courseId;
    int year, n;
    std::cout << "  Course ID : "; std::cin >> courseId;
    std::cout << "  Batch year: "; std::cin >> year;
    std::cout << "  Top N     : "; std::cin >> n;

    // Waitlist is already priority-sorted; filter by year in one pass.
    auto entries = cr.topNByYear(n, year);

    // Enrich with any extra student data from Student Records (e.g. department).
    if (entries.empty()) {
        std::cout << "  No waitlisted students from batch " << year << ".\n";
        return;
    }

    std::cout << "\n  Rank  ID        Name                CGPA   Dept\n";
    int rank = 1;
    for (const auto& e : entries) {
        // Pull full record from Student Records for the department field.
        auto match = sr.searchByNamePrefix(e.student.name);
        std::string dept = match.empty() ? e.student.department : match[0].department;
        std::cout << "  " << rank++ << "     "
                  << e.student.id   << "  "
                  << e.student.name << "  "
                  << e.student.cgpa << "  "
                  << dept           << "\n";
    }
}

// ── sub-menus ─────────────────────────────────────────────────────────────────

static void studentMenu(MultiViewManager& sr) {
    std::cout << "\n  1 Add student\n"
                 "  2 Remove student\n"
                 "  3 Search by name prefix\n"
                 "  4 Search by CGPA range\n"
                 "  5 Search by year\n"
                 "  6 Cross-view query (year + min CGPA + name prefix)\n"
                 "  7 Print all\n"
                 "  0 Back\n  > ";
    int ch; std::cin >> ch;
    switch (ch) {
        case 1: {
            Student s;
            std::cout << "  ID: ";         std::cin  >> s.id;
            std::cout << "  Name: ";       clearInput(); std::getline(std::cin, s.name);
            std::cout << "  CGPA: ";       std::cin  >> s.cgpa;
            std::cout << "  Batch year: "; std::cin  >> s.batchYear;
            std::cout << "  Department: "; clearInput(); std::getline(std::cin, s.department);
            std::cout << (sr.addStudent(s) ? "  Added.\n" : "  Failed (duplicate ID?).\n");
            break;
        }
        case 2: {
            std::string id;
            std::cout << "  ID: "; std::cin >> id;
            std::cout << (sr.removeStudent(id) ? "  Removed.\n" : "  Not found.\n");
            break;
        }
        case 3: {
            std::string prefix;
            std::cout << "  Prefix: "; std::cin >> prefix;
            printStudents(sr.searchByNamePrefix(prefix));
            break;
        }
        case 4: {
            double lo, hi;
            std::cout << "  Low CGPA: ";  std::cin >> lo;
            std::cout << "  High CGPA: "; std::cin >> hi;
            printStudents(sr.searchByCGPARange(lo, hi));
            break;
        }
        case 5: {
            int year;
            std::cout << "  Year: "; std::cin >> year;
            printStudents(sr.searchByYear(year));
            break;
        }
        case 6: {
            int year; double minCGPA; std::string prefix;
            std::cout << "  Year: ";       std::cin >> year;
            std::cout << "  Min CGPA: ";   std::cin >> minCGPA;
            std::cout << "  Name prefix (or -): "; std::cin >> prefix;
            if (prefix == "-") prefix = "";
            printStudents(sr.crossQuery(year, minCGPA, prefix));
            break;
        }
        case 7: sr.printAll(); break;
    }
}

static void registrationMenu(CourseRegistration& cr) {
    std::cout << "\n  1 Add to waitlist\n"
                 "  2 Promote (seat opened)\n"
                 "  3 Update student CGPA\n"
                 "  4 Show top N waitlisted\n"
                 "  0 Back\n  > ";
    int ch; std::cin >> ch;
    switch (ch) {
        case 1: {
            Student s;
            std::cout << "  ID: ";         std::cin  >> s.id;
            std::cout << "  Name: ";       clearInput(); std::getline(std::cin, s.name);
            std::cout << "  CGPA: ";       std::cin  >> s.cgpa;
            std::cout << "  Batch year: "; std::cin  >> s.batchYear;
            cr.enqueue(s);
            std::cout << "  Enqueued. Waitlist size: " << cr.size() << "\n";
            break;
        }
        case 2: {
            if (cr.empty()) { std::cout << "  Waitlist is empty.\n"; break; }
            auto e = cr.promoteFront();
            std::cout << "  Promoted: " << e.student.name
                      << " (CGPA " << e.student.cgpa << ")\n";
            break;
        }
        case 3: {
            std::string id; double cgpa;
            std::cout << "  Student ID: "; std::cin >> id;
            std::cout << "  New CGPA: ";   std::cin >> cgpa;
            cr.updateCGPA(id, cgpa);
            std::cout << "  Updated and re-prioritized.\n";
            break;
        }
        case 4: {
            int n;
            std::cout << "  N: "; std::cin >> n;
            auto entries = cr.topN(n);
            for (int i = 0; i < static_cast<int>(entries.size()); ++i)
                std::cout << "  " << i+1 << ". "
                          << entries[i].student.name
                          << "  CGPA:" << entries[i].student.cgpa
                          << "  Year:" << entries[i].student.batchYear << "\n";
            break;
        }
    }
}

// ── main ─────────────────────────────────────────────────────────────────────

int main() {
    MultiViewManager   sr;
    EventScheduler     es;
    LibraryManager     lm;
    CourseRegistration cr("CS101");

    int choice;
    do {
        std::cout << "\n=== UniCore ===\n"
                     "  1 Student Records\n"
                     "  2 Event Scheduler      (Krish — TODO)\n"
                     "  3 Library Manager      (Saad  — TODO)\n"
                     "  4 Course Registration\n"
                     "  5 Cross-system: top waitlisted by batch year\n"
                     "  0 Exit\n  > ";
        std::cin >> choice;
        switch (choice) {
            case 1: studentMenu(sr);              break;
            case 2: std::cout << "  Not yet implemented.\n"; (void)es; break;
            case 3: std::cout << "  Not yet implemented.\n"; (void)lm; break;
            case 4: registrationMenu(cr);         break;
            case 5: crossTopWaitlisted(cr, sr);   break;
            case 0: std::cout << "Bye.\n";        break;
            default: std::cout << "  Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
