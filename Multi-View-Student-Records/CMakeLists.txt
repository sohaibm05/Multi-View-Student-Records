cmake_minimum_required(VERSION 3.10)
project(UniCore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(UNICORE_CORE
    src/StudentRecords.cpp
    src/EventScheduler.cpp
    src/LibraryManager.cpp
    src/CourseRegistration.cpp
)

add_executable(unicore
    src/main.cpp
    ${UNICORE_CORE}
)

add_executable(unicore_tests
    tests/test_unicore.cpp
    ${UNICORE_CORE}
)

# SFML graphical UI target.
# Install SFML first, then build this target:
#   Ubuntu: sudo apt install libsfml-dev
#   Windows/MinGW: install SFML and set SFML_DIR if CMake cannot find it.
find_package(SFML 2.5 COMPONENTS graphics window system QUIET)
if (SFML_FOUND)
    add_executable(unicore_sfml
        src/sfml_ui.cpp
        ${UNICORE_CORE}
    )
    target_link_libraries(unicore_sfml sfml-graphics sfml-window sfml-system)
else()
    message(STATUS "SFML not found. Console targets will build, but unicore_sfml will be skipped.")
endif()
