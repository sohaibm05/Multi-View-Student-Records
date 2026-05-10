#include "StudentRecords.hpp"
#include "EventScheduler.hpp"
#include "LibraryManager.hpp"
#include "CourseRegistration.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <limits>

using Clock = std::chrono::high_resolution_clock;
using Ms    = std::chrono::duration<double, std::milli>;

// Flush the rest of a bad input line so cin doesn't get stuck.
static void clearInput() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// ── Member 1: Student Records ─────────────────────────────────────────────────
// Lets the user build up a StudentRecords database within the session and run
// order-statistic queries against it.
void menuStudentRecords() {
    StudentRecords db;
    int choice;
    do {
        std::cout << "\n-- Student Records --\n"
                  << "1. Add student\n"
                  << "2. k-th lowest CGPA student\n"
                  << "3. Rank of a CGPA value\n"
                  << "4. Count students in CGPA range\n"
                  << "0. Back\n"
                  << "Choice: ";
        std::cin >> choice; clearInput();

        if (choice == 1) {
            Student s;
            std::cout << "ID: ";    std::getline(std::cin, s.id);
            std::cout << "Name: ";  std::getline(std::cin, s.name);
            std::cout << "CGPA: ";  std::cin >> s.cgpa;
            std::cout << "Batch: "; std::cin >> s.batchYear; clearInput();
            db.addStudent(s);
            std::cout << "  Added.\n";
        } else if (choice == 2) {
            size_t k;
            std::cout << "k: "; std::cin >> k; clearInput();
            auto* node = db.cgpaTree.kthSmallest(k);
            if (node) std::cout << "  " << node->value.name
                                << "  CGPA " << node->value.cgpa << "\n";
            else      std::cout << "  Not found (k out of range).\n";
        } else if (choice == 3) {
            double cgpa;
            std::cout << "CGPA: "; std::cin >> cgpa; clearInput();
            std::cout << "  Rank: " << db.cgpaTree.rank(cgpa) << "\n";
        } else if (choice == 4) {
            double lo, hi;
            std::cout << "Low CGPA: ";  std::cin >> lo;
            std::cout << "High CGPA: "; std::cin >> hi; clearInput();
            std::cout << "  Count: " << db.cgpaTree.countInRange(lo, hi) << "\n";
        }
    } while (choice != 0);
}

// ── Member 2: Event Scheduler ─────────────────────────────────────────────────
// maxEnd augmentation lets conflictCheck prune entire subtrees instead of
// checking every stored event.
void menuEventScheduler() {
    EventScheduler sched;
    int choice;
    do {
        std::cout << "\n-- Event Scheduler --\n"
                  << "1. Add event\n"
                  << "2. Conflict check\n"
                  << "0. Back\n"
                  << "Choice: ";
        std::cin >> choice; clearInput();

        if (choice == 1) {
            Event e;
            std::cout << "Name: ";  std::getline(std::cin, e.name);
            std::cout << "Start: "; std::cin >> e.startTime;
            std::cout << "End: ";   std::cin >> e.endTime; clearInput();
            sched.addEvent(e);
            std::cout << "  Added.\n";
        } else if (choice == 2) {
            long long s, e;
            std::cout << "Start: "; std::cin >> s;
            std::cout << "End: ";   std::cin >> e; clearInput();
            std::cout << "  Conflict: "
                      << (sched.tree.conflictCheck(s, e) ? "YES" : "NO") << "\n";
        }
    } while (choice != 0);
}

// ── Member 3: Library Manager ─────────────────────────────────────────────────
// subtreeBooks is updated by walking up the parent chain (no rotation needed),
// which is different from subtreeSize that only updates on rotations.
void menuLibraryManager() {
    LibraryManager lib;
    int choice;
    do {
        std::cout << "\n-- Library Manager --\n"
                  << "1. Add book\n"
                  << "2. Update availability (by due date)\n"
                  << "3. Count available books in date range\n"
                  << "0. Back\n"
                  << "Choice: ";
        std::cin >> choice; clearInput();

        if (choice == 1) {
            Book b;
            char av;
            std::cout << "Title: ";            std::getline(std::cin, b.title);
            std::cout << "Due date: ";         std::cin >> b.dueDate;
            std::cout << "Available (y/n): ";  std::cin >> av; clearInput();
            b.isAvailable = (av == 'y' || av == 'Y');
            lib.addBook(b);
            std::cout << "  Added.\n";
        } else if (choice == 2) {
            long long due; char av;
            std::cout << "Due date: ";         std::cin >> due;
            std::cout << "Available (y/n): ";  std::cin >> av; clearInput();
            auto* node = lib.tree.find(due);
            if (node) {
                lib.tree.updateAvailability(node, av == 'y' || av == 'Y');
                std::cout << "  Updated.\n";
            } else {
                std::cout << "  Book not found.\n";
            }
        } else if (choice == 3) {
            long long d1, d2;
            std::cout << "From date: "; std::cin >> d1;
            std::cout << "To date: ";   std::cin >> d2; clearInput();
            std::cout << "  Available: " << lib.tree.availableInRange(d1, d2) << "\n";
        }
    } while (choice != 0);
}

// ── Member 4: Course Registration ────────────────────────────────────────────
// The composite PriorityKey keeps the tree sorted so the highest-priority
// student is always the leftmost node, making promoteFront O(log n).
void menuCourseRegistration() {
    CourseRegistration reg;
    int choice;
    do {
        std::cout << "\n-- Course Registration --\n"
                  << "1. Add student to waitlist\n"
                  << "2. Promote front (seat opened)\n"
                  << "3. Reprioritize student (CGPA updated)\n"
                  << "4. Top-k from a batch year\n"
                  << "0. Back\n"
                  << "Choice: ";
        std::cin >> choice; clearInput();

        if (choice == 1) {
            std::string id; double cgpa; int batch; long long ts;
            std::cout << "Student ID: ";  std::getline(std::cin, id);
            std::cout << "CGPA: ";        std::cin >> cgpa;
            std::cout << "Batch year: ";  std::cin >> batch;
            std::cout << "Timestamp: ";   std::cin >> ts; clearInput();
            reg.addToWaitlist(id, cgpa, batch, ts);
            std::cout << "  Added to waitlist.\n";
        } else if (choice == 2) {
            std::string promoted = reg.waitlist.promoteFront();
            if (promoted.empty()) std::cout << "  Waitlist is empty.\n";
            else                  std::cout << "  Promoted: " << promoted << "\n";
        } else if (choice == 3) {
            // We need the exact old key to locate the node, then insert a new one.
            std::string id;
            double oc, nc; int ob, nb; long long ot, nt;
            std::cout << "Student ID: ";      std::getline(std::cin, id);
            std::cout << "Old CGPA: ";        std::cin >> oc;
            std::cout << "Old batch: ";       std::cin >> ob;
            std::cout << "Old timestamp: ";   std::cin >> ot;
            std::cout << "New CGPA: ";        std::cin >> nc;
            std::cout << "New batch: ";       std::cin >> nb;
            std::cout << "New timestamp: ";   std::cin >> nt; clearInput();
            reg.reprioritize(id, oc, ob, ot, nc, nb, nt);
            std::cout << "  Reprioritized.\n";
        } else if (choice == 4) {
            int batch; size_t k;
            std::cout << "Batch year: "; std::cin >> batch;
            std::cout << "k: ";          std::cin >> k; clearInput();
            auto results = reg.topKFromBatch(batch, k);
            if (results.empty()) std::cout << "  No matches.\n";
            for (size_t i = 0; i < results.size(); ++i)
                std::cout << "  " << (i + 1) << ". " << results[i] << "\n";
        }
    } while (choice != 0);
}

// ── Benchmarks ────────────────────────────────────────────────────────────────
// Each benchmark inserts the same N items into both an RBTree-based structure
// and a plain std::vector, then times Q identical queries on each.
// The goal is to show the practical O(log n) vs O(n) difference.
void runBenchmarks() {
    const int N = 5000;   // dataset size
    const int Q = 1000;   // number of queries
    std::mt19937 rng(42); // fixed seed so results are reproducible

    std::uniform_real_distribution<double>  cgpaDist(1.0, 4.0);
    std::uniform_int_distribution<int>      batchDist(2018, 2024);
    std::uniform_int_distribution<long long> timeDist(0, 10000);

    std::cout << "\n=== Benchmarks  (N=" << N
              << " entries,  Q=" << Q << " queries) ===\n";
    std::cout << std::fixed << std::setprecision(3);

    // ── 1. Course Registration: N × promoteFront ─────────────────────────────
    // RBTree: remove leftmost node each time  → O(log n) per call
    // Vector: erase(begin()) on a sorted vector → O(n) per call (shifts elements)
    // Total: O(n log n) vs O(n²)
    {
        CourseRegistration reg;
        // Build a parallel sorted vector that mimics the same priority order.
        using PQ = std::tuple<double, int, long long>; // (cgpa desc, batch asc, time asc)
        std::vector<PQ> vec;

        for (int i = 0; i < N; ++i) {
            double c = cgpaDist(rng); int b = batchDist(rng); long long t = i;
            reg.addToWaitlist("S" + std::to_string(i), c, b, t);
            vec.emplace_back(c, b, t);
        }
        // Sort vector into the same priority order so we're comparing fairly.
        std::sort(vec.begin(), vec.end(), [](const PQ& a, const PQ& b) {
            if (std::get<0>(a) != std::get<0>(b)) return std::get<0>(a) > std::get<0>(b);
            if (std::get<1>(a) != std::get<1>(b)) return std::get<1>(a) < std::get<1>(b);
            return std::get<2>(a) < std::get<2>(b);
        });

        auto t0 = Clock::now();
        while (reg.waitlist.size() > 0)
            reg.waitlist.promoteFront();
        double treeMs = Ms(Clock::now() - t0).count();

        auto t1 = Clock::now();
        while (!vec.empty())
            vec.erase(vec.begin()); // O(n): shifts all remaining elements left
        double vecMs = Ms(Clock::now() - t1).count();

        std::cout << "\n[Course Registration  -  promoteFront x" << N << "]\n"
                  << "  RBTree (O(n log n) total): " << treeMs << " ms\n"
                  << "  Sorted vector (O(n²) total): " << vecMs  << " ms\n"
                  << "  Speedup: " << vecMs / treeMs << "x\n";
    }

    // ── 2. Event Scheduler: Q × conflictCheck ────────────────────────────────
    // RBTree: prunes subtrees using maxEnd augmentation → O(log n) per query
    // Vector: checks every event one by one             → O(n) per query
    {
        EventScheduler sched;
        std::vector<std::pair<long long, long long>> evVec; // (start, end)

        for (int i = 0; i < N; ++i) {
            long long s = timeDist(rng), e = s + timeDist(rng) % 100 + 1;
            sched.addEvent({"E", s, e});
            evVec.emplace_back(s, e);
        }

        std::uniform_int_distribution<long long> qDist(0, 10000);

        auto t0 = Clock::now();
        for (int q = 0; q < Q; ++q) {
            long long s = qDist(rng), e = s + 50;
            sched.tree.conflictCheck(s, e);
        }
        double treeMs = Ms(Clock::now() - t0).count();

        auto t1 = Clock::now();
        for (int q = 0; q < Q; ++q) {
            long long s = qDist(rng), e = s + 50;
            for (auto& [es, ee] : evVec)
                if (!(ee < s || e < es)) break; // stop at first conflict
        }
        double vecMs = Ms(Clock::now() - t1).count();

        std::cout << "\n[Event Scheduler  -  conflictCheck x" << Q << "]\n"
                  << "  RBTree (O(log n) per query): " << treeMs << " ms\n"
                  << "  Linear scan (O(n) per query): " << vecMs  << " ms\n"
                  << "  Speedup: " << vecMs / treeMs << "x\n";
    }

    // ── 3. Library Manager: Q × availableInRange ─────────────────────────────
    // RBTree: uses subtreeBooks to skip unavailable subtrees → O(log n)
    // Vector: scans every book and checks date + availability → O(n)
    {
        LibraryManager lib;
        std::vector<std::pair<long long, bool>> books; // (dueDate, isAvailable)
        std::bernoulli_distribution avDist(0.6);

        for (int i = 0; i < N; ++i) {
            long long d = timeDist(rng); bool av = avDist(rng);
            lib.addBook({"B", d, av});
            books.emplace_back(d, av);
        }

        auto t0 = Clock::now();
        for (int q = 0; q < Q; ++q)
            lib.tree.availableInRange(q * 10, q * 10 + 500);
        double treeMs = Ms(Clock::now() - t0).count();

        auto t1 = Clock::now();
        for (int q = 0; q < Q; ++q) {
            long long lo = q * 10, hi = q * 10 + 500;
            size_t cnt = 0;
            for (auto& [d, av] : books)
                if (av && d >= lo && d <= hi) ++cnt;
        }
        double vecMs = Ms(Clock::now() - t1).count();

        std::cout << "\n[Library Manager  -  availableInRange x" << Q << "]\n"
                  << "  RBTree (O(log n) per query): " << treeMs << " ms\n"
                  << "  Linear scan (O(n) per query): " << vecMs  << " ms\n"
                  << "  Speedup: " << vecMs / treeMs << "x\n";
    }

    // ── 4. Student Records: Q × countInRange ─────────────────────────────────
    // RBTree: uses subtreeSize to count without visiting every node → O(log n)
    // Vector: iterates every CGPA value in the array               → O(n)
    {
        StudentRecords db;
        std::vector<double> cgpas;

        for (int i = 0; i < N; ++i) {
            double c = cgpaDist(rng); int b = batchDist(rng);
            db.addStudent({"S" + std::to_string(i), "Name", c, b});
            cgpas.push_back(c);
        }

        auto t0 = Clock::now();
        for (int q = 0; q < Q; ++q)
            db.cgpaTree.countInRange(2.0 + (q % 20) * 0.1, 2.5 + (q % 20) * 0.1);
        double treeMs = Ms(Clock::now() - t0).count();

        auto t1 = Clock::now();
        for (int q = 0; q < Q; ++q) {
            double lo = 2.0 + (q % 20) * 0.1, hi = 2.5 + (q % 20) * 0.1;
            size_t cnt = 0;
            for (double c : cgpas)
                if (c >= lo && c <= hi) ++cnt;
        }
        double vecMs = Ms(Clock::now() - t1).count();

        std::cout << "\n[Student Records  -  countInRange x" << Q << "]\n"
                  << "  RBTree (O(log n) per query): " << treeMs << " ms\n"
                  << "  Linear scan (O(n) per query): " << vecMs  << " ms\n"
                  << "  Speedup: " << vecMs / treeMs << "x\n";
    }

    std::cout << "\n";
}

// ── Main menu ─────────────────────────────────────────────────────────────────
int main() {
    int choice;
    do {
        std::cout << "\n=== UniCore University Management Suite ===\n"
                  << "1. Student Records\n"
                  << "2. Event Scheduler\n"
                  << "3. Library Management\n"
                  << "4. Course Registration\n"
                  << "5. Run Benchmarks\n"
                  << "0. Exit\n"
                  << "Choice: ";
        std::cin >> choice; clearInput();

        switch (choice) {
            case 1: menuStudentRecords();     break;
            case 2: menuEventScheduler();     break;
            case 3: menuLibraryManager();     break;
            case 4: menuCourseRegistration(); break;
            case 5: runBenchmarks();          break;
            case 0: std::cout << "Goodbye.\n"; break;
            default: std::cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    return 0;
}
