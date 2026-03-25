cmake_minimum_required(VERSION 3.16)
project(MultiViewStudentRecords CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ---- Main executable ----
add_executable(student_records
    src/main.cpp
    src/NameIndex.cpp
    src/CGPAIndex.cpp
    src/YearIndex.cpp
    src/MultiViewManager.cpp
)
target_include_directories(student_records PRIVATE include)

# ---- Tests ----
enable_testing()

add_executable(test_rbtree tests/test_rbtree.cpp)
target_include_directories(test_rbtree PRIVATE include)

add_executable(test_indexes
    tests/test_indexes.cpp
    src/NameIndex.cpp
    src/CGPAIndex.cpp
    src/YearIndex.cpp
    src/MultiViewManager.cpp
)
target_include_directories(test_indexes PRIVATE include)

add_test(NAME RBTreeTests  COMMAND test_rbtree)
add_test(NAME IndexTests   COMMAND test_indexes)
