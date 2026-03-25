// Basic tests for RBTree
// Run via: ctest or directly

#include "RBTree.hpp"
#include <cassert>
#include <iostream>
#include <vector>

void testInsertFind() {
    RBTree<int, std::string> t;
    t.insert(5, "five");
    t.insert(3, "three");
    t.insert(7, "seven");
    t.insert(1, "one");

    assert(t.find(5) != nullptr && t.find(5)->value == "five");
    assert(t.find(3) != nullptr);
    assert(t.find(99) == nullptr);
    assert(t.size() == 4);
    std::cout << "[PASS] insertFind\n";
}

void testInorder() {
    RBTree<int, int> t;
    std::vector<int> keys = {4, 2, 6, 1, 3, 5, 7};
    for (int k : keys) t.insert(k, k);

    std::vector<int> result;
    t.inorder([&](const int& k, const int&) { result.push_back(k); });

    for (int i = 0; i < (int)result.size() - 1; ++i)
        assert(result[i] < result[i+1]);
    std::cout << "[PASS] inorder\n";
}

void testDelete() {
    RBTree<int, int> t;
    for (int i = 1; i <= 10; ++i) t.insert(i, i);
    assert(t.size() == 10);

    t.remove(5);
    assert(t.find(5) == nullptr);
    assert(t.size() == 9);

    t.remove(1);
    t.remove(10);
    assert(t.size() == 7);

    // Verify tree is still sorted
    std::vector<int> result;
    t.inorder([&](const int& k, const int&) { result.push_back(k); });
    for (int i = 0; i < (int)result.size() - 1; ++i)
        assert(result[i] < result[i+1]);
    std::cout << "[PASS] delete\n";
}

void testDuplicateKey() {
    RBTree<int, std::string> t;
    t.insert(1, "first");
    t.insert(1, "second"); // should update
    assert(t.size() == 1);
    assert(t.find(1)->value == "second");
    std::cout << "[PASS] duplicateKey\n";
}

int main() {
    testInsertFind();
    testInorder();
    testDelete();
    testDuplicateKey();
    std::cout << "All RBTree tests passed.\n";
    return 0;
}
