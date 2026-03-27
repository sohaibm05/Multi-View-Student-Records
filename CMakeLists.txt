cmake_minimum_required(VERSION 3.16)
project(UniCore CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ---- Shared source files ----
set(STUDENT_RECORDS_SRCS
    src/NameIndex.cpp
    src/CGPAIndex.cpp
    src/YearIndex.cpp
    src/MultiViewManager.cpp
)

set(ALL_MODULE_SRCS
    ${STUDENT_RECORDS_SRCS}
    src/EventScheduler.cpp
    src/LibraryManager.cpp
    src/CourseRegistration.cpp
)

# ---- Main executable ----
add_executable(unicore
    src/main.cpp
    ${ALL_MODULE_SRCS}
)
target_include_directories(unicore PRIVATE include)

# ---- Tests ----
enable_testing()

add_executable(test_rbtree tests/test_rbtree.cpp)
target_include_directories(test_rbtree PRIVATE include)

add_executable(test_indexes
    tests/test_indexes.cpp
    ${STUDENT_RECORDS_SRCS}
)
target_include_directories(test_indexes PRIVATE include)

add_executable(test_event_scheduler
    tests/test_event_scheduler.cpp
    src/EventScheduler.cpp
)
target_include_directories(test_event_scheduler PRIVATE include)

add_executable(test_library_manager
    tests/test_library_manager.cpp
    src/LibraryManager.cpp
)
target_include_directories(test_library_manager PRIVATE include)

add_executable(test_course_registration
    tests/test_course_registration.cpp
    src/CourseRegistration.cpp
    ${STUDENT_RECORDS_SRCS}
)
target_include_directories(test_course_registration PRIVATE include)

add_test(NAME RBTreeTests            COMMAND test_rbtree)
add_test(NAME IndexTests             COMMAND test_indexes)
add_test(NAME EventSchedulerTests    COMMAND test_event_scheduler)
add_test(NAME LibraryManagerTests    COMMAND test_library_manager)
add_test(NAME CourseRegistrationTests COMMAND test_course_registration)
