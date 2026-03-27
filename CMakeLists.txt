# =============================================================================
# HOW TO BUILD AND RUN
# -----------------------------------------------------------------------------
# Step 1 — create a build folder and generate build files (run once):
#   mkdir build
#   cd build
#   cmake ..
#
# Step 2 — compile everything (re-run this every time you change code):
#   cmake --build .
#
# Step 3 — run the main app:
#   ./unicore          (Linux / Mac)
#   unicore.exe        (Windows)
#
# Step 4 — run all tests at once:
#   ctest
#
# To run a single test binary directly (shows more detail):
#   ./test_rbtree
#   ./test_indexes
#   ./test_event_scheduler
#   ./test_library_manager
#   ./test_course_registration
# =============================================================================

# Minimum CMake version required to understand this file.
cmake_minimum_required(VERSION 3.16)

# Name the project and tell CMake we are using C++.
project(UniCore CXX)

# Use the C++17 standard.  REQUIRED means CMake will error if the compiler
# does not support it (instead of silently falling back to an older standard).
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Turn on warnings so the compiler tells us about potential mistakes.
# MSVC (Visual Studio) uses /W4; GCC and Clang use -Wall -Wextra -Wpedantic.
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# =============================================================================
# SOURCE FILE LISTS
# Using variables means we only list each .cpp file once.
# If you add a new .cpp file, add it to the right list here and it will
# automatically be included in the main binary AND the relevant test binary.
# =============================================================================

# Module 1 — Student Records (Ibad)
set(STUDENT_RECORDS_SRCS
    src/NameIndex.cpp
    src/CGPAIndex.cpp
    src/YearIndex.cpp
    src/MultiViewManager.cpp
)

# All four modules combined — used by the main executable.
set(ALL_MODULE_SRCS
    ${STUDENT_RECORDS_SRCS}
    src/EventScheduler.cpp       # Module 2 — Krish
    src/LibraryManager.cpp       # Module 3 — Saad
    src/CourseRegistration.cpp   # Module 4 — Sohaib
)

# =============================================================================
# MAIN EXECUTABLE
# add_executable(<name> <source files ...>) tells CMake to compile all the
# listed .cpp files and link them into one binary called "unicore".
# target_include_directories tells the compiler to look inside the "include/"
# folder when it sees  #include "SomeHeader.hpp"
# =============================================================================

add_executable(unicore
    src/main.cpp
    ${ALL_MODULE_SRCS}
)
target_include_directories(unicore PRIVATE include)

# =============================================================================
# TESTS
# enable_testing() activates the  ctest  command.
# Each add_executable below builds a separate test binary.
# Each add_test registers that binary with ctest so  ctest  can run them all.
# =============================================================================

enable_testing()

# Tests for the shared RBTree (no module .cpp files needed — RBTree is header-only).
add_executable(test_rbtree tests/test_rbtree.cpp)
target_include_directories(test_rbtree PRIVATE include)

# Tests for Module 1 indexes.
add_executable(test_indexes
    tests/test_indexes.cpp
    ${STUDENT_RECORDS_SRCS}
)
target_include_directories(test_indexes PRIVATE include)

# Tests for Module 2 — Event Scheduler.
add_executable(test_event_scheduler
    tests/test_event_scheduler.cpp
    src/EventScheduler.cpp
)
target_include_directories(test_event_scheduler PRIVATE include)

# Tests for Module 3 — Library Manager.
add_executable(test_library_manager
    tests/test_library_manager.cpp
    src/LibraryManager.cpp
)
target_include_directories(test_library_manager PRIVATE include)

# Tests for Module 4 — Course Registration.
# Also needs Student Records sources because CourseRegistration uses Student.
add_executable(test_course_registration
    tests/test_course_registration.cpp
    src/CourseRegistration.cpp
    ${STUDENT_RECORDS_SRCS}
)
target_include_directories(test_course_registration PRIVATE include)

# Register every test binary with ctest.
# NAME  = label shown when you run  ctest -V
# COMMAND = which binary to execute
add_test(NAME RBTreeTests             COMMAND test_rbtree)
add_test(NAME IndexTests              COMMAND test_indexes)
add_test(NAME EventSchedulerTests     COMMAND test_event_scheduler)
add_test(NAME LibraryManagerTests     COMMAND test_library_manager)
add_test(NAME CourseRegistrationTests COMMAND test_course_registration)
