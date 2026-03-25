// Integration tests for NameIndex, CGPAIndex, YearIndex, MultiViewManager

#include "MultiViewManager.hpp"
#include <cassert>
#include <iostream>

static MultiViewManager buildSample() {
    MultiViewManager mgr;
    mgr.addStudent({"s001", "Alice",   3.9, 2021, "CS"});
    mgr.addStudent({"s002", "Bob",     3.2, 2022, "EE"});
    mgr.addStudent({"s003", "Charlie", 3.7, 2022, "CS"});
    mgr.addStudent({"s004", "alice",   2.8, 2023, "ME"}); // duplicate lowercase name
    mgr.addStudent({"s005", "Diana",   3.5, 2021, "CS"});
    return mgr;
}

void testNamePrefix() {
    auto mgr = buildSample();
    auto r = mgr.searchByNamePrefix("ali");
    assert(r.size() == 2);
    std::cout << "[PASS] namePrefix\n";
}

void testCGPARange() {
    auto mgr = buildSample();
    auto r = mgr.searchByCGPARange(3.5, 4.0);
    assert(r.size() == 3); // 3.9, 3.7, 3.5
    std::cout << "[PASS] cgpaRange\n";
}

void testYearSearch() {
    auto mgr = buildSample();
    auto r = mgr.searchByYear(2022);
    assert(r.size() == 2);
    std::cout << "[PASS] yearSearch\n";
}

void testCrossQuery() {
    auto mgr = buildSample();
    auto r = mgr.crossQuery(2022, 3.5, "");
    assert(r.size() == 1); // only Charlie (Bob is 3.2)
    std::cout << "[PASS] crossQuery\n";
}

void testRemove() {
    auto mgr = buildSample();
    bool ok = mgr.removeStudent("s002");
    assert(ok);
    auto r = mgr.searchByYear(2022);
    assert(r.size() == 1);
    std::cout << "[PASS] remove\n";
}

int main() {
    testNamePrefix();
    testCGPARange();
    testYearSearch();
    testCrossQuery();
    testRemove();
    std::cout << "All index tests passed.\n";
    return 0;
}
