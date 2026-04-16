// Member 4 — UniCore CLI & Integration (Sohaib)
//
// This file owns:
//   • One instance of each of the four subsystem objects.
//   • The text menu that lets a user drive all four modules from one terminal.
//   • The cross-system query function that joins data from Module 1 and Module 4.
//
// When adding a new menu option:
//   1. Add a case to the do-while switch in main().
//   2. Write a static helper function above main() (keeps main() short).
//   3. If the option needs data from two modules, pass both by reference.

#include "MultiViewManager.hpp"
#include "EventScheduler.hpp"
#include "LibraryManager.hpp"
#include "CourseRegistration.hpp"
#include <iostream>
#include <string>
#include <limits>

// ── helpers ───────────────────────────────────────────────────────────────────

// Discard the rest of the current input line so the next std::cin >> works cleanly.
static void clearInput() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Print a vector of students, one per line.  Prints "(none)" on empty input.
static void printStudents(const std::vector<Student>& v) {
    if (v.empty()) { std::cout << "  (none)\n"; return; }
    for (const auto& s : v)
        std::cout << "  " << s.id << "  " << s.name
                  << "  CGPA:" << s.cgpa << "  Year:" << s.batchYear << "\n";
}

// ── cross-system query ────────────────────────────────────────────────────────
// "Top N waitlisted students from batch year Y for a given course."
//
// How the two modules collaborate:
//   CourseRegistration::topNByYear(n, year)  →  walks the priority-sorted
//     waitlist and filters by batchYear in one O(n) pass.
//   MultiViewManager::searchByNamePrefix     →  used here to enrich the entry
//     with the department field (the waitlist only caches the snapshot at enqueue time).
//
// This is the programming-level integration point: neither module alone can
// answer this query — it requires the priority ordering from Module 4 AND the
// full student record from Module 1.
static void crossTopWaitlisted(const CourseRegistration& cr,
                                const MultiViewManager&   sr) {
    std::string courseId;
    int year, n;
    std::cout << "  Course ID : "; std::cin >> courseId;
    std::cout << "  Batch year: "; std::cin >> year;
    std::cout << "  Top N     : "; std::cin >> n;

    auto entries = cr.topNByYear(n, year);

    if (entries.empty()) {
        std::cout << "  No waitlisted students from batch " << year << ".\n";
        return;
    }

    std::cout << "\n  Rank  ID        Name                CGPA   Dept\n";
    int rank = 1;
    for (const auto& e : entries) {
        // Pull the current department from Student Records (Module 1).
        // The waitlist snapshot may be stale if the student changed departments.
        auto match = sr.searchByNamePrefix(e.student.name);
        std::string dept = match.empty() ? e.student.department : match[0].department;
        std::cout << "  " << rank++ << "     "
                  << e.student.id   << "  "
                  << e.student.name << "  "
                  << e.student.cgpa << "  "
                  << dept           << "\n";
    }
}

// ── Student Records sub-menu (Module 1) ──────────────────────────────────────
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
            // Cross-VIEW (within Module 1): combines year + CGPA + name prefix.
            // Distinct from the cross-SYSTEM query in main() which spans modules.
            int year; double minCGPA; std::string prefix;
            std::cout << "  Year: ";       std::cin >> year;
            std::cout << "  Min CGPA: ";   std::cin >> minCGPA;
            std::cout << "  Name prefix (or ): "; std::cin >> prefix;
            if (prefix == "-") prefix = "";
            printStudents(sr.crossQuery(year, minCGPA, prefix));
            break;
        }
        case 7: sr.printAll(); break;
    }
}

// ── Course Registration sub-menu (Module 4) ───────────────────────────────────
static void registrationMenu(CourseRegistration& cr) {
    std::cout << "\n  1 Add to waitlist\n"
                 "  2 Promote front (seat opened)\n"
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
            // A course seat just opened — pop the highest-priority student.
            if (cr.empty()) { std::cout << "  Waitlist is empty.\n"; break; }
            auto e = cr.promoteFront();
            std::cout << "  Promoted: " << e.student.name
                      << " (CGPA " << e.student.cgpa << ")\n";
            break;
        }
        case 3: {
            // Student's CGPA updated (e.g. semester results came in).
            // updateCGPA removes the old entry and reinserts at the new priority.
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
                std::cout << "  " << i + 1 << ". "
                          << entries[i].student.name
                          << "  CGPA:" << entries[i].student.cgpa
                          << "  Year:" << entries[i].student.batchYear << "\n";
            break;
        }
    }
}

// ── main ──────────────────────────────────────────────────────────────────────
int main() {
    // One instance of each subsystem.
    // Modules 2 and 3 (Krish and Saad) will fill in their implementations;
    // the objects are created here so main.cpp compiles and links at all times.
    MultiViewManager   sr;           // Module 1 — Student Records
    EventScheduler     es;           // Module 2 — Event Scheduler
    LibraryManager     lm;           // Module 3 — Library Manager
    CourseRegistration cr("CS101");  // Module 4 — Course Registration (waitlist for CS101)

    int choice;
    do {
        std::cout << "\n=== UniCore ===\n"
                     "  1  Student Records\n"
                     "  2  Event Scheduler      (Krish — TODO)\n"
                     "  3  Library Manager      (Saad  — TODO)\n"
                     "  4  Course Registration\n"
                     "  5  Cross-system: top waitlisted by batch year\n"
                     "  0  Exit\n  > ";
        std::cin >> choice;
        switch (choice) {
            case 1: studentMenu(sr);            break;
            case 2: std::cout << "  Not yet implemented.\n"; (void)es; break;
            case 3: std::cout << "  Not yet implemented.\n"; (void)lm; break;
            case 4: registrationMenu(cr);       break;
            case 5: crossTopWaitlisted(cr, sr); break;
            case 0: std::cout << "Bye.\n";      break;
            default: std::cout << "  Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
