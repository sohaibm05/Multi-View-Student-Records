#include "StudentRecords.hpp"
#include "EventScheduler.hpp"
#include "LibraryManager.hpp"
#include "CourseRegistration.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <limits>
#include <cstdlib>

static void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin >> std::ws, s);
    return s;
}

static int readInt(const std::string& prompt) {
    int x;
    while (true) {
        std::cout << prompt;
        if (std::cin >> x) return x;
        std::cout << "Invalid number. Try again.\n";
        clearInput();
    }
}

static long long readLong(const std::string& prompt) {
    long long x;
    while (true) {
        std::cout << prompt;
        if (std::cin >> x) return x;
        std::cout << "Invalid number. Try again.\n";
        clearInput();
    }
}

static double readDouble(const std::string& prompt) {
    double x;
    while (true) {
        std::cout << prompt;
        if (std::cin >> x) return x;
        std::cout << "Invalid number. Try again.\n";
        clearInput();
    }
}

static void loadSampleData(StudentRecords& students, EventScheduler& events,
                           LibraryManager& library, CourseRegistration& courses) {
    students.addStudent(1, "Ali Khan", 3.78, 2022);
    students.addStudent(2, "Sara Ahmed", 3.92, 2021);
    students.addStudent(3, "Bilal Noor", 3.15, 2023);
    students.addStudent(4, "Ayesha Malik", 3.65, 2022);
    students.addStudent(5, "Zain Raza", 3.45, 2021);
    students.addStudent(6, "Hamza Shah", 3.88, 2022);

    events.addEvent("DS2 Lecture", 900, 1030, "Room A1");
    events.addEvent("Programming Lab", 1100, 1300, "Lab 3");
    events.addEvent("Society Meeting", 1500, 1600, "Auditorium");

    library.addBook("Introduction to Algorithms", "CLRS");
    library.addBook("Data Structures in C++", "Weiss");
    library.addBook("Algorithms", "Sedgewick");
    library.addBook("Database Systems", "Elmasri");
    library.checkoutBookByTitle("Data Structures", 31052025);

    int ids[] = {1, 2, 4, 6};
    for (int id : ids) {
        Student* s = students.findById(id);
        if (s) courses.addToWaitlist("CS201", *s);
    }

    std::cout << "Sample data loaded.\n";
}

static void runBenchmarks() {
    StudentRecords treeStudents;
    std::vector<Student> vectorStudents;
    const int n = 5000;

    for (int i = 1; i <= n; i++) {
        double cgpa = 2.0 + (i % 200) / 100.0;
        int batch = 2020 + (i % 5);
        std::string name = "Student" + std::to_string(i);
        treeStudents.addStudent(i, name, cgpa, batch);
        vectorStudents.push_back(Student{i, name, cgpa, batch});
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    int treeCount = treeStudents.countCgpaInRange(3.0, 3.5);
    auto t2 = std::chrono::high_resolution_clock::now();

    auto v1 = std::chrono::high_resolution_clock::now();
    int vectorCount = 0;
    for (const Student& s : vectorStudents) {
        if (s.cgpa >= 3.0 && s.cgpa <= 3.5) vectorCount++;
    }
    auto v2 = std::chrono::high_resolution_clock::now();

    auto treeTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto vectorTime = std::chrono::duration_cast<std::chrono::microseconds>(v2 - v1).count();

    std::cout << "Benchmark on " << n << " student records\n";
    std::cout << "RBT count result: " << treeCount << ", time: " << treeTime << " microseconds\n";
    std::cout << "Vector scan result: " << vectorCount << ", time: " << vectorTime << " microseconds\n";
}

static void studentMenu(StudentRecords& students) {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n===== Student Records Queries =====\n";
        std::cout << "1. Add student\n";
        std::cout << "2. Remove student\n";
        std::cout << "3. Update CGPA\n";
        std::cout << "4. Search by ID\n";
        std::cout << "5. Show all students alphabetically\n";
        std::cout << "6. Prefix search by name\n";
        std::cout << "7. kth lowest CGPA\n";
        std::cout << "8. Rank by CGPA\n";
        std::cout << "9. Count students in CGPA range\n";
        std::cout << "10. Show students from batch\n";
        std::cout << "0. Back\n";
        choice = readInt("Choose query: ");

        if (choice == 1) {
            int id = readInt("ID: ");
            std::string name = readLine("Name: ");
            double cgpa = readDouble("CGPA: ");
            int batch = readInt("Batch year: ");
            std::cout << (students.addStudent(id, name, cgpa, batch) ? "Student added.\n" : "Could not add student. ID may already exist.\n");
        } else if (choice == 2) {
            int id = readInt("ID to remove: ");
            std::cout << (students.removeStudent(id) ? "Student removed.\n" : "Student not found.\n");
        } else if (choice == 3) {
            int id = readInt("Student ID: ");
            double cgpa = readDouble("New CGPA: ");
            std::cout << (students.updateCgpa(id, cgpa) ? "CGPA updated.\n" : "Student not found.\n");
        } else if (choice == 4) {
            int id = readInt("Student ID: ");
            Student* s = students.findById(id);
            if (s) std::cout << s->id << " | " << s->name << " | CGPA " << s->cgpa << " | Batch " << s->batchYear << "\n";
            else std::cout << "Student not found.\n";
        } else if (choice == 5) {
            students.printAll();
        } else if (choice == 6) {
            std::string prefix = readLine("Name prefix: ");
            std::vector<Student> ans = students.prefixSearch(prefix);
            for (const Student& s : ans) std::cout << s.id << " | " << s.name << " | CGPA " << s.cgpa << " | Batch " << s.batchYear << "\n";
            if (ans.empty()) std::cout << "No matching students.\n";
        } else if (choice == 7) {
            int k = readInt("k: ");
            Student* s = students.kthSmallestCgpa(k);
            if (s) std::cout << k << "th lowest CGPA: " << s->name << " | " << s->cgpa << "\n";
            else std::cout << "Invalid k.\n";
        } else if (choice == 8) {
            double cgpa = readDouble("CGPA value: ");
            std::cout << "Rank position: " << students.rankByCgpa(cgpa) << "\n";
        } else if (choice == 9) {
            double low = readDouble("Low CGPA: ");
            double high = readDouble("High CGPA: ");
            std::cout << "Count: " << students.countCgpaInRange(low, high) << "\n";
        } else if (choice == 10) {
            int batch = readInt("Batch year: ");
            std::vector<Student> ans = students.studentsInBatch(batch);
            for (const Student& s : ans) std::cout << s.id << " | " << s.name << " | CGPA " << s.cgpa << " | Batch " << s.batchYear << "\n";
            if (ans.empty()) std::cout << "No students in this batch.\n";
        }
    }
}

static void eventMenu(EventScheduler& events) {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n===== Event Scheduler Queries =====\n";
        std::cout << "1. Add event\n";
        std::cout << "2. Remove event\n";
        std::cout << "3. Show all conflicts\n";
        std::cout << "4. Range query\n";
        std::cout << "5. Show next event\n";
        std::cout << "6. Show all events\n";
        std::cout << "0. Back\n";
        choice = readInt("Choose query: ");

        if (choice == 1) {
            std::string title = readLine("Event title: ");
            long long start = readLong("Start time/date number: ");
            long long end = readLong("End time/date number: ");
            std::string location = readLine("Location: ");
            Event conflict;
            if (end <= start) {
                std::cout << "Could not add event: end time must be greater than start time.\n";
            } else if (events.findFirstConflict(start, end, conflict)) {
                std::cout << "Could not add event because it conflicts with:\n";
                std::cout << conflict.id << " | " << conflict.title << " | " << conflict.start << "-" << conflict.end << " | " << conflict.location << "\n";
            } else if (events.addEvent(title, start, end, location)) {
                std::cout << "Event added.\n";
            } else {
                std::cout << "Could not add event.\n";
            }
        } else if (choice == 2) {
            int id = readInt("Event ID: ");
            std::cout << (events.removeEvent(id) ? "Event removed.\n" : "Event not found.\n");
        } else if (choice == 3) {
            long long start = readLong("Query start: ");
            long long end = readLong("Query end: ");
            std::vector<Event> ans = events.allConflicts(start, end);
            for (const Event& e : ans) std::cout << e.id << " | " << e.title << " | " << e.start << "-" << e.end << " | " << e.location << "\n";
            if (ans.empty()) std::cout << "No conflicts.\n";
        } else if (choice == 4) {
            long long start = readLong("Range start: ");
            long long end = readLong("Range end: ");
            std::vector<Event> ans = events.rangeQuery(start, end);
            for (const Event& e : ans) std::cout << e.id << " | " << e.title << " | " << e.start << "-" << e.end << " | " << e.location << "\n";
            if (ans.empty()) std::cout << "No events in range.\n";
        } else if (choice == 5) {
            Event* e = events.nextEvent();
            if (e) std::cout << e->id << " | " << e->title << " | " << e->start << "-" << e->end << " | " << e->location << "\n";
            else std::cout << "No events.\n";
        } else if (choice == 6) {
            events.printAll();
        }
    }
}

static void libraryMenu(LibraryManager& library) {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n===== Library Management Queries =====\n";
        std::cout << "1. Add book\n";
        std::cout << "2. Checkout book\n";
        std::cout << "3. Return book\n";
        std::cout << "4. Count currently available books\n";
        std::cout << "5. Show checked-out books due in range\n";
        std::cout << "6. Show overdue checked-out books\n";
        std::cout << "7. Show all books\n";
        std::cout << "0. Back\n";
        choice = readInt("Choose query: ");

        if (choice == 1) {
            std::string title = readLine("Book title: ");
            std::string author = readLine("Author: ");
            std::cout << (library.addBook(title, author) ? "Book added as available with due date N/A.\n" : "Could not add book.\n");
        } else if (choice == 2) {
            int id = readInt("Book ID to checkout: ");
            long long today = readLong("Today/checkout date (DDMMYYYY, e.g. 12052025): ");
            bool ok = library.checkoutBook(id, today);
            if (ok) {
                long long due = LibraryManager::addDaysToDate(LibraryManager::pakToInternal(today), 30);
                std::cout << "Book checked out. Due date = " << LibraryManager::displayDate(due) << "\n";
            } else {
                std::cout << "Book not found, not available, or already checked out.\n";
            }
        } else if (choice == 3) {
            int id = readInt("Book ID to return: ");
            std::cout << (library.returnBook(id) ? "Book returned. Due date is now N/A.\n" : "Book not found or already available.\n");
        } else if (choice == 4) {
            std::cout << "Currently available books: " << library.availableCount() << "\n";
        } else if (choice == 5) {
            long long d1 = readLong("Start due date (DDMMYYYY): ");
            long long d2 = readLong("End due date (DDMMYYYY): ");
            std::vector<Book> ans = library.checkedOutDueInRange(d1, d2);
            for (const Book& b : ans) {
                std::cout << b.id << " | " << b.title << " | due " << LibraryManager::displayDate(b.dueDate) << " | Checked out\n";
            }
            if (ans.empty()) std::cout << "No checked-out books due in this range.\n";
        } else if (choice == 6) {
            long long today = readLong("Today date (DDMMYYYY): ");
            std::vector<Book> ans = library.overdueBooks(today);
            for (const Book& b : ans) {
                std::cout << b.id << " | " << b.title << " | due " << LibraryManager::displayDate(b.dueDate) << " | Fine " << library.estimateFine(b, today) << "\n";
            }
            if (ans.empty()) std::cout << "No overdue checked-out books.\n";
        } else if (choice == 7) {
            library.printAll();
        }
    }
}

static void courseMenu(CourseRegistration& courses, StudentRecords& students) {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n===== Course Registration Queries =====\n";
        std::cout << "1. Add student to waitlist\n";
        std::cout << "2. Remove student from waitlist\n";
        std::cout << "3. Promote front student\n";
        std::cout << "4. Show front student\n";
        std::cout << "5. Show course waitlist\n";
        std::cout << "6. Top waitlisted students from batch\n";
        std::cout << "7. Update student CGPA and waitlist priority\n";
        std::cout << "8. Show all waitlists\n";
        std::cout << "0. Back\n";
        choice = readInt("Choose query: ");

        if (choice == 1) {
            std::string course = readLine("Course code: ");
            int id = readInt("Student ID: ");
            Student* s = students.findById(id);
            if (!s) std::cout << "Student not found. Add student first.\n";
            else std::cout << (courses.addToWaitlist(course, *s) ? "Added to waitlist.\n" : "Could not add to waitlist.\n");
        } else if (choice == 2) {
            std::string course = readLine("Course code: ");
            int id = readInt("Student ID: ");
            Student* s = students.findById(id);
            if (!s) std::cout << "Student not found.\n";
            else std::cout << (courses.removeFromWaitlist(course, *s) ? "Removed from waitlist.\n" : "Entry not found.\n");
        } else if (choice == 3) {
            std::string course = readLine("Course code: ");
            WaitlistEntry e;
            if (courses.promoteFront(course, e)) std::cout << "Promoted student " << e.studentId << " from " << course << " waitlist.\n";
            else std::cout << "No waitlisted student found.\n";
        } else if (choice == 4) {
            std::string course = readLine("Course code: ");
            WaitlistEntry* e = courses.front(course);
            if (e) std::cout << "Front: Student " << e->studentId << " | CGPA " << e->cgpa << " | Batch " << e->batchYear << "\n";
            else std::cout << "No waitlisted student found.\n";
        } else if (choice == 5) {
            std::string course = readLine("Course code: ");
            std::vector<WaitlistEntry> ans = courses.courseWaitlist(course);
            for (const WaitlistEntry& e : ans) std::cout << e.courseCode << " | Student " << e.studentId << " | CGPA " << e.cgpa << " | Batch " << e.batchYear << " | Time " << e.timestamp << "\n";
            if (ans.empty()) std::cout << "No waitlist entries for this course.\n";
        } else if (choice == 6) {
            std::string course = readLine("Course code: ");
            int batch = readInt("Batch year: ");
            int limit = readInt("How many students: ");
            std::vector<WaitlistEntry> ans = courses.topWaitlistedFromBatch(course, batch, limit);
            for (const WaitlistEntry& e : ans) std::cout << e.courseCode << " | Student " << e.studentId << " | CGPA " << e.cgpa << " | Batch " << e.batchYear << "\n";
            if (ans.empty()) std::cout << "No matching entries.\n";
        } else if (choice == 7) {
            std::string course = readLine("Course code: ");
            int id = readInt("Student ID: ");
            double newCgpa = readDouble("New CGPA: ");
            Student* oldPtr = students.findById(id);
            if (!oldPtr) {
                std::cout << "Student not found.\n";
            } else {
                Student oldStudent = *oldPtr;
                students.updateCgpa(id, newCgpa);
                Student* newPtr = students.findById(id);
                bool ok = newPtr && courses.updateStudentPriority(course, oldStudent, *newPtr);
                std::cout << (ok ? "Student CGPA and waitlist priority updated.\n" : "CGPA updated, but waitlist entry was not found.\n");
            }
        } else if (choice == 8) {
            courses.printAll();
        }
    }
}

static void crossSubsystemMenu(CourseRegistration& courses, StudentRecords& students) {
    std::cout << "\n===== Cross-Subsystem Query =====\n";
    std::string course = readLine("Course code: ");
    int batch = readInt("Batch year: ");
    int limit = readInt("How many top waitlisted students: ");
    std::vector<WaitlistEntry> top = courses.topWaitlistedFromBatch(course, batch, limit);
    for (const WaitlistEntry& e : top) {
        Student* s = students.findById(e.studentId);
        if (s) std::cout << s->id << " | " << s->name << " | CGPA " << s->cgpa << " | Batch " << s->batchYear << " | Course " << e.courseCode << "\n";
    }
    if (top.empty()) std::cout << "No matching students found.\n";
}

static void mainMenu() {
    std::cout << "\n===== UniCore University Management Suite =====\n";
    std::cout << "1. Student Records queries\n";
    std::cout << "2. Event Scheduler queries\n";
    std::cout << "3. Library Management queries\n";
    std::cout << "4. Course Registration queries\n";
    std::cout << "5. Cross-subsystem query\n";
    std::cout << "6. Run benchmark\n";
    std::cout << "7. Load sample data\n";
    std::cout << "8. Show all current data\n";
    std::cout << "0. Exit\n";
}

int main() {
    StudentRecords students;
    EventScheduler events;
    LibraryManager library;
    CourseRegistration courses;

    std::cout << "UniCore started. Use option 7 if you want sample data, or add your own records manually.\n";

    int choice = -1;
    while (choice != 0) {
        mainMenu();
        choice = readInt("Choose: ");

        if (choice == 1) studentMenu(students);
        else if (choice == 2) eventMenu(events);
        else if (choice == 3) libraryMenu(library);
        else if (choice == 4) courseMenu(courses, students);
        else if (choice == 5) crossSubsystemMenu(courses, students);
        else if (choice == 6) runBenchmarks();
        else if (choice == 7) loadSampleData(students, events, library, courses);
        else if (choice == 8) {
            std::cout << "\nStudents:\n"; students.printAll();
            std::cout << "\nEvents:\n"; events.printAll();
            std::cout << "\nBooks:\n"; library.printAll();
            std::cout << "\nWaitlists:\n"; courses.printAll();
        }
    }

    return 0;
}
