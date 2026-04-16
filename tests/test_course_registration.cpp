// Member 4 — Course Registration tests (Sohaib)

#include "CourseRegistration.hpp"
#include <cassert>
#include <iostream>

// ── helpers ───────────────────────────────────────────────────────────────────

static Student makeStudent(const std::string& id, const std::string& name,
                            double cgpa, int batchYear) {
    Student s;
    s.id        = id;
    s.name      = name;
    s.cgpa      = cgpa;
    s.batchYear = batchYear;
    return s;
}

// ── enqueue / size / empty ────────────────────────────────────────────────────

void testEnqueueAndSize() {
    CourseRegistration cr("CS101");
    assert(cr.empty());
    assert(cr.size() == 0);

    cr.enqueue(makeStudent("s001", "Alice", 3.9, 2022));
    assert(!cr.empty());
    assert(cr.size() == 1);

    cr.enqueue(makeStudent("s002", "Bob", 3.5, 2021));
    assert(cr.size() == 2);

    std::cout << "[PASS] enqueueAndSize\n";
}

// ── promoteFront on empty list ────────────────────────────────────────────────

void testPromoteFrontEmpty() {
    CourseRegistration cr("CS101");
    // Should return a default-constructed entry without crashing.
    auto e = cr.promoteFront();
    assert(e.student.id.empty());
    std::cout << "[PASS] promoteFrontEmpty\n";
}

// ── Priority rule 1: higher CGPA comes out first ──────────────────────────────

void testCGPAPriority() {
    CourseRegistration cr("CS101");
    cr.enqueue(makeStudent("s001", "Low",  2.5, 2022));
    cr.enqueue(makeStudent("s002", "High", 3.9, 2022));
    cr.enqueue(makeStudent("s003", "Mid",  3.2, 2022));

    auto first  = cr.promoteFront();
    auto second = cr.promoteFront();
    auto third  = cr.promoteFront();

    assert(first.student.id  == "s002");  // 3.9 highest
    assert(second.student.id == "s003");  // 3.2 next
    assert(third.student.id  == "s001");  // 2.5 last
    assert(cr.empty());

    std::cout << "[PASS] cgpaPriority\n";
}

// ── Priority rule 2: same CGPA — older batch year wins ───────────────────────

void testBatchYearPriority() {
    CourseRegistration cr("CS101");
    // All have the same CGPA, different batch years.
    cr.enqueue(makeStudent("s001", "New",    3.5, 2023));
    cr.enqueue(makeStudent("s002", "Oldest", 3.5, 2020));
    cr.enqueue(makeStudent("s003", "Middle", 3.5, 2021));

    assert(cr.promoteFront().student.id == "s002");  // 2020 oldest batch
    assert(cr.promoteFront().student.id == "s003");  // 2021 next
    assert(cr.promoteFront().student.id == "s001");  // 2023 newest

    std::cout << "[PASS] batchYearPriority\n";
}

// ── topN ──────────────────────────────────────────────────────────────────────

void testTopN() {
    CourseRegistration cr("CS101");
    cr.enqueue(makeStudent("s001", "Alice",   3.9, 2022));
    cr.enqueue(makeStudent("s002", "Bob",     3.2, 2022));
    cr.enqueue(makeStudent("s003", "Charlie", 3.7, 2022));
    cr.enqueue(makeStudent("s004", "Diana",   2.8, 2022));

    auto top2 = cr.topN(2);
    assert(top2.size() == 2);
    assert(top2[0].student.id == "s001");  // 3.9
    assert(top2[1].student.id == "s003");  // 3.7

    // Asking for more than the waitlist size should return all.
    auto topAll = cr.topN(100);
    assert(topAll.size() == 4);

    // topN should NOT remove entries.
    assert(cr.size() == 4);

    std::cout << "[PASS] topN\n";
}

// ── topNByYear ────────────────────────────────────────────────────────────────

void testTopNByYear() {
    CourseRegistration cr("CS101");
    cr.enqueue(makeStudent("s001", "Alice",   3.9, 2022));
    cr.enqueue(makeStudent("s002", "Bob",     3.5, 2021));
    cr.enqueue(makeStudent("s003", "Charlie", 3.7, 2022));
    cr.enqueue(makeStudent("s004", "Diana",   3.2, 2021));

    auto batch2022 = cr.topNByYear(10, 2022);
    assert(batch2022.size() == 2);
    assert(batch2022[0].student.id == "s001");  // 3.9
    assert(batch2022[1].student.id == "s003");  // 3.7

    auto batch2021 = cr.topNByYear(1, 2021);
    assert(batch2021.size() == 1);
    assert(batch2021[0].student.id == "s002");  // 3.5 > 3.2

    auto noMatch = cr.topNByYear(5, 2025);
    assert(noMatch.empty());

    std::cout << "[PASS] topNByYear\n";
}

// ── updateCGPA ────────────────────────────────────────────────────────────────

void testUpdateCGPA() {
    CourseRegistration cr("CS101");
    cr.enqueue(makeStudent("s001", "Alice", 3.9, 2022));
    cr.enqueue(makeStudent("s002", "Bob",   3.5, 2022));

    // Alice was first (3.9 > 3.5). Lower Bob's CGPA then boost him past Alice.
    cr.updateCGPA("s002", 4.0);

    auto first = cr.promoteFront();
    assert(first.student.id   == "s002");   // now has 4.0 — highest priority
    assert(first.student.cgpa == 4.0);

    auto second = cr.promoteFront();
    assert(second.student.id == "s001");    // Alice at 3.9

    std::cout << "[PASS] updateCGPA\n";
}

// ── updateCGPA for non-existent student is a no-op ───────────────────────────

void testUpdateCGPANotFound() {
    CourseRegistration cr("CS101");
    cr.enqueue(makeStudent("s001", "Alice", 3.9, 2022));

    // Should not crash or change the size.
    cr.updateCGPA("unknown", 2.0);
    assert(cr.size() == 1);

    std::cout << "[PASS] updateCGPANotFound\n";
}

// ── promoteFront removes from tree (size decreases) ──────────────────────────

void testPromoteFrontReducesSize() {
    CourseRegistration cr("CS101");
    cr.enqueue(makeStudent("s001", "A", 3.5, 2022));
    cr.enqueue(makeStudent("s002", "B", 3.8, 2022));

    assert(cr.size() == 2);
    cr.promoteFront();
    assert(cr.size() == 1);
    cr.promoteFront();
    assert(cr.size() == 0);
    assert(cr.empty());

    std::cout << "[PASS] promoteFrontReducesSize\n";
}

// ── WaitlistEntry carries correct courseId ───────────────────────────────────

void testCourseIdStoredInEntry() {
    CourseRegistration cr("EE202");
    cr.enqueue(makeStudent("s001", "Alice", 3.5, 2022));

    auto e = cr.promoteFront();
    assert(e.courseId == "EE202");

    std::cout << "[PASS] courseIdStoredInEntry\n";
}

// ── main ──────────────────────────────────────────────────────────────────────

int main() {
    testEnqueueAndSize();
    testPromoteFrontEmpty();
    testCGPAPriority();
    testBatchYearPriority();
    testTopN();
    testTopNByYear();
    testUpdateCGPA();
    testUpdateCGPANotFound();
    testPromoteFrontReducesSize();
    testCourseIdStoredInEntry();

    std::cout << "All CourseRegistration tests passed.\n";
    return 0;
}
