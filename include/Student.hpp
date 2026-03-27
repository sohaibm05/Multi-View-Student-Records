#pragma once
#include <string>

// =============================================================================
// Student  —  core data record for the Student Records module (Member 1)
//
// ONE Student object lives in MultiViewManager::records.
// The three RBTs (NameIndex, CGPAIndex, YearIndex) do NOT copy the struct —
// they store it by value in their nodes, so keep this struct small.
// =============================================================================

struct Student {
    std::string id;          // unique student ID, e.g. "sm09751" — used as tiebreaker in keys
    std::string name;        // full name, e.g. "Sohaib Muhammad"
    double      cgpa;        // 0.0 – 4.0
    int         batchYear;   // enrolment year, e.g. 2022
    std::string department;  // e.g. "CS", "EE"
};
