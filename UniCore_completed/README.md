# UniCore — University Management Suite using Red-Black Trees

This is a completed C++17 implementation of the CS201 Data Structures II proposal. The project contains four university management subsystems, all built on a shared generic Red-Black Tree implementation.

## Implemented Modules

1. **Student Records System**
   - Indexes students by ID, name, CGPA, and batch year.
   - Uses `subtreeSize` augmentation for order-statistics queries.
   - Supports `kthSmallestCgpa`, `rankByCgpa`, `countCgpaInRange`, batch queries, and case-insensitive prefix search.

2. **Event Scheduler**
   - Stores event intervals keyed by start time.
   - Uses `maxEnd` augmentation for interval conflict detection.
   - Supports conflict check, all conflicts, range query, and next event.

3. **Library Management System**
   - Stores books keyed by due date.
   - Uses `subtreeBooks` augmentation to track currently available books in each subtree.
   - Supports add book, checkout by title/name with due date, return by title/name, due-date range search, overdue book detection, and fine estimation.

4. **Course Registration and Waitlist Manager**
   - Stores waitlisted students using composite priority keys.
   - Priority order: CGPA descending, batch year ascending, signup timestamp ascending.
   - Supports promote-front, dynamic re-prioritization, course waitlist views, and cross-subsystem batch queries.

## Interfaces

The project now has two interfaces:

1. **Terminal CLI** using `src/main.cpp`
2. **SFML graphical UI** using `src/sfml_ui.cpp`

The SFML UI is now organized into four application tabs on the main screen: Student Records, Event Scheduler, Library Management, and Course Registration. Each tab only shows the queries relevant to that subsystem. Type the required input format, then click the query button inside that tab.

Example UI inputs:

```text
Add Student: 10,Zaid Naqvi,3.75,2022
Prefix Search: za or ZA
CGPA Range: 3.4,3.9
Add Event: Viva,1300,1400,Room B
Event Conflict: 1330,1430
Add Book: CLRS,Cormen,01052025,1
Checkout Book: CLRS,31052025
Add Waitlist: CS201,10
Cross Query: CS201,2022,5
```

## SFML UI Layout

On launch, the graphical interface shows four tabs:

1. **Student Records** — add student, show students, case-insensitive prefix search, rank by CGPA, k-th CGPA, and CGPA range query.
2. **Event Scheduler** — add event, show all events, conflict check, all conflicts, range query, and next event.
3. **Library Management** — add book, show books, available count, checked-out due range, overdue books, checkout by book name/title with a manually entered due date, and return by title/name.
4. **Course Registration** — add to waitlist, show course waitlist, promote front student, and cross batch waitlist query.

The prefix search treats uppercase and lowercase as the same, so `ali`, `ALI`, and `Ali` return the same matching names.

## Build Instructions

### Build terminal version with g++

```bash
g++ -std=c++17 -Iinclude src/main.cpp src/StudentRecords.cpp src/EventScheduler.cpp src/LibraryManager.cpp src/CourseRegistration.cpp -o unicore
./unicore
```

### Build tests

```bash
g++ -std=c++17 -Iinclude tests/test_unicore.cpp src/StudentRecords.cpp src/EventScheduler.cpp src/LibraryManager.cpp src/CourseRegistration.cpp -o unicore_tests
./unicore_tests
```

### Build using CMake

```bash
mkdir build
cd build
cmake ..
cmake --build .
./unicore
./unicore_tests
```

### Build SFML UI

Install SFML first.

Ubuntu/Linux:

```bash
sudo apt install libsfml-dev
mkdir build
cd build
cmake ..
cmake --build .
./unicore_sfml
```

Windows with MinGW:

1. Install SFML for the same MinGW compiler version you are using.
2. Configure CMake.
3. If CMake cannot find SFML automatically, set `SFML_DIR` to the SFML CMake folder.
4. Build and run `unicore_sfml.exe`.

## Project Structure

```text
include/                 Header files
src/main.cpp             Terminal CLI
src/sfml_ui.cpp          SFML graphical UI
src/*.cpp                Module source files
tests/                   Basic assertion-based tests
CMakeLists.txt           CMake build file
README.md                Project documentation
```

## Notes

- The core tree is implemented from scratch in `RBTree.hpp`.
- STL containers are used only for temporary results and demo output.
- The CLI version has no external dependencies.
- The SFML UI requires SFML, but the project still builds the CLI and tests even if SFML is not installed.
