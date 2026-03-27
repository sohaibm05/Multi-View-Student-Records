#pragma once
#include "RBTree.hpp"
#include "Student.hpp"
#include <string>
#include <vector>
#include <ctime>

// =============================================================================
// CourseRegistration  —  priority waitlist  (Member 4 — Sohaib)
//
// Uses a RBTree<Priority, WaitlistEntry> where the smallest key (leftmost node)
// always belongs to the highest-priority student.
//
// PRIORITY ORDERING (operator< on Priority):
//   1. Higher CGPA   → higher priority  (cgpa compared in DESCENDING order,
//                                        i.e., 4.0 is "less than" 3.5 so it
//                                        sits to the LEFT / comes out first)
//   2. Lower batchYear → higher priority (older batch gets preference)
//   3. Earlier timestamp → higher priority (first-come, first-served tiebreaker)
//
// This means:  Priority{4.0, 2021, t1} < Priority{3.9, 2021, t2}  → TRUE
//              Priority{3.5, 2020, t1} < Priority{3.5, 2022, t2}  → TRUE (older batch wins)
//              Priority{3.5, 2021, t1} < Priority{3.5, 2021, t2}  → TRUE (t1 < t2, earlier signup wins)
//
// CROSS-SYSTEM INTEGRATION (used by main.cpp):
//   topNByYear(n, year) lets the CLI answer "top N waitlisted from batch 2022"
//   by walking the waitlist in priority order and filtering by batchYear.
// =============================================================================

class CourseRegistration {
public:

    // ── Composite key ──────────────────────────────────────────────────────────
    struct Priority {
        double      cgpa;       // compared DESCENDING (higher = smaller key)
        int         batchYear;  // compared ASCENDING  (lower year = smaller key)
        std::time_t timestamp;  // compared ASCENDING  (earlier signup = smaller key)

        // See ordering rules above.  All three fields must be checked in order.
        bool operator<(const Priority& o) const;

        // Used by updateCGPA to find and remove the exact old key.
        bool operator==(const Priority& o) const;
    };

    // ── Waitlist entry stored in every tree node ───────────────────────────────
    struct WaitlistEntry {
        Student     student;   // full student record at the time of enqueue
        std::string courseId;  // which course this waitlist belongs to
        Priority    priority;  // stored alongside so promoteFront can return it
    };

    // courseId is the course this waitlist belongs to (e.g. "CS101").
    explicit CourseRegistration(const std::string& courseId);

    // ── Waitlist operations ────────────────────────────────────────────────────

    // Add student to the waitlist.
    // Build Priority{ student.cgpa, student.batchYear, std::time(nullptr) }.
    // Insert WaitlistEntry into the tree with that key.
    void enqueue(const Student& student);

    // A seat just opened — return and remove the highest-priority student.
    // Use tree.front() to get the leftmost (minimum-key) node.
    // Call tree.remove(node->key) to splice it out.
    // Returns a default-constructed WaitlistEntry if the waitlist is empty.
    // O(log n)
    WaitlistEntry promoteFront();

    // Student's CGPA changed — must be re-positioned in the waitlist.
    // 1. Scan the tree via inorder to find the entry with matching student.id.
    // 2. Call tree.remove(foundKey).
    // 3. Update found.student.cgpa and found.priority.cgpa = newCGPA,
    //    keep the ORIGINAL timestamp (sign-up time does not change).
    // 4. Call tree.insert(found.priority, found).
    // O(n) scan + O(log n) remove + O(log n) insert.
    void updateCGPA(const std::string& studentId, double newCGPA);

    // ── Queries ───────────────────────────────────────────────────────────────

    // Return the top n entries in priority order (inorder walk, stop after n).
    // O(n) worst case but exits early after n results.
    std::vector<WaitlistEntry> topN(int n) const;

    // Cross-system: return top n waitlisted students from a specific batch year.
    // Same as topN but additionally filters entries where student.batchYear == batchYear.
    // Called by main.cpp's cross-system menu option.
    std::vector<WaitlistEntry> topNByYear(int n, int batchYear) const;

    int  size()  const;
    bool empty() const;

private:
    std::string                     courseId;
    RBTree<Priority, WaitlistEntry> tree;
};
