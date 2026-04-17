#include "StudentRecords.hpp"
#include "EventScheduler.hpp"
#include "LibraryManager.hpp"
#include "CourseRegistration.hpp"
#include <iostream>
#include <iomanip>

int main() {
    std::cout << "=== UniCore 30% Demo ===\n\n";

    // Member 1 - Student Records
    StudentRecords students;
    students.addStudent({"S001", "Alice", 3.9, 2022});
    students.addStudent({"S002", "Bob",   3.7, 2023});
    students.addStudent({"S003", "Carol", 3.9, 2022});
    std::cout << "Student Records:\n";
    std::cout << "  2nd lowest CGPA student: " << students.cgpaTree.kthSmallest(2)->value.name << "\n";
    std::cout << "  Rank of 3.9: " << students.cgpaTree.rank(3.9) << "\n\n";

    // Member 2 - Event Scheduler
    EventScheduler events;
    events.addEvent({"Midterm", 100, 120});
    events.addEvent({"Lab", 110, 130});
    std::cout << "Event Scheduler:\n";
    std::cout << "  Conflict with [115,125]? " << (events.tree.conflictCheck(115,125) ? "YES" : "NO") << "\n\n";

    // Member 3 - Library
    LibraryManager lib;
    lib.addBook({"BookA", 100, true});
    lib.addBook({"BookB", 150, false});
    auto bookNode = lib.tree.find(100);
    lib.tree.updateAvailability(bookNode, false);
    std::cout << "Library:\n";
    std::cout << "  Available books due 90-200: " << lib.tree.availableInRange(90,200) << "\n\n";

    // Member 4 - Course Registration
    CourseRegistration reg;
    reg.addToWaitlist("S001", 3.9, 2022, 1);
    reg.addToWaitlist("S002", 3.7, 2023, 2);
    reg.addToWaitlist("S003", 3.9, 2022, 0);
    std::cout << "Course Waitlist (promote front):\n";
    std::cout << "  Promoted: " << reg.waitlist.promoteFront() << "\n\n";

    std::cout << "30% of UniCore complete - each subsystem has its own augmentation + core queries.\n";
    std::cout << "Next 70% will add full CLI, cross-subsystem queries, benchmarks, rollback, etc.\n";
    return 0;
}