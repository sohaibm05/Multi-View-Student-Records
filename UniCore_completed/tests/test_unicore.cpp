#include "StudentRecords.hpp"
#include "EventScheduler.hpp"
#include "LibraryManager.hpp"
#include "CourseRegistration.hpp"
#include <cassert>
#include <iostream>

int main() {
    StudentRecords students;
    assert(students.addStudent(1, "Ali", 3.70, 2022));
    assert(students.addStudent(2, "Sara", 3.90, 2021));
    assert(students.addStudent(3, "Bilal", 3.20, 2023));
    assert(students.addStudent(4, "Ayesha", 3.80, 2022));
    assert(students.size() == 4);
    assert(students.kthSmallestCgpa(1)->id == 3);
    assert(students.rankByCgpa(3.80) == 3);
    assert(students.countCgpaInRange(3.60, 3.95) == 3);
    assert(students.prefixSearch("A").size() == 2);
    assert(students.prefixSearch("a").size() == 2);
    assert(students.prefixSearch("AL").size() == 1);

    EventScheduler events;
    assert(events.addEvent("Lecture", 900, 1000, "A1"));
    assert(events.addEvent("Lab", 1100, 1200, "Lab"));
    assert(!events.addEvent("Conflict", 930, 1130, "B1"));
    assert(events.hasConflict(950, 970));
    assert(events.allConflicts(950, 970).size() == 1);

    LibraryManager library;
    assert(library.addBook("CLRS", "Cormen"));
    assert(library.addBook("Weiss", "Weiss"));
    assert(library.availableCount() == 2);
    assert(library.checkoutBookByTitle("CLRS", 31052025));
    assert(library.availableCount() == 1);
    assert(library.checkedOutDueInRange(31052025, 31052025).size() == 1);
    assert(library.checkedOutDueInRange(1012020, 31122999).size() == 1);
    assert(library.returnBookByTitle("clrs"));
    assert(library.availableCount() == 2);
    assert(library.overdueBooks(31122999).empty());

    CourseRegistration courses;
    Student* ali = students.findById(1);
    Student* sara = students.findById(2);
    Student* ayesha = students.findById(4);
    assert(ali && sara && ayesha);
    assert(courses.addToWaitlist("CS201", *ali));
    assert(courses.addToWaitlist("CS201", *sara));
    assert(courses.addToWaitlist("CS201", *ayesha));
    WaitlistEntry promoted;
    assert(courses.promoteFront("CS201", promoted));
    assert(promoted.studentId == 2); // Highest CGPA should be promoted first.
    assert(courses.topWaitlistedFromBatch("CS201", 2022, 5).size() == 2);

    std::cout << "All UniCore tests passed.\n";
    return 0;
}
