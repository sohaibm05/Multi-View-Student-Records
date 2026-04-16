// =============================================================================
// test_rbtree.cpp  —  Comprehensive RBTree test & demo
// Covers: RB invariants, insert, find, remove, inorder, front/back,
//         lowerBound, upperBound, size/empty, edge cases, string keys.
// Run via: ctest  or  directly execute the binary.
// =============================================================================

#include "RBTree.hpp"
#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

// ── Separator helpers ─────────────────────────────────────────────────────────
static void section(const std::string& title) {
    std::cout << "\n══════════════════════════════════════════════\n";
    std::cout << "  " << title << "\n";
    std::cout << "══════════════════════════════════════════════\n";
}
static void pass(const std::string& name) {
    std::cout << "  [PASS] " << name << "\n";
}

// =============================================================================
// RBTreeValidator  —  subclass that exposes protected state for invariant checks
// =============================================================================
template <typename Key, typename Value>
class RBTreeValidator : public RBTree<Key, Value> {
    using Node  = typename RBTree<Key, Value>::Node;
    using Color = typename RBTree<Key, Value>::Color;

public:
    // Invariant 2: root is BLACK (or tree is empty)
    bool rootIsBlack() const {
        if (this->root == this->NIL) return true;
        return this->root->color == Color::BLACK;
    }

    // Invariant 4: no RED node has a RED child
    bool noRedRed() const { return checkNoRedRed(this->root); }

    // Invariant 5: every root-to-NIL path has the same number of BLACK nodes
    bool blackHeightConsistent() const { return blackHeight(this->root) != -1; }

    // All three structural invariants hold
    bool isValid() const {
        return rootIsBlack() && noRedRed() && blackHeightConsistent();
    }

    // Collect all keys via inorder into a vector
    std::vector<Key> keys() const {
        std::vector<Key> v;
        this->inorder([&](const Key& k, const Value&) { v.push_back(k); });
        return v;
    }

private:
    bool checkNoRedRed(Node* x) const {
        if (x == this->NIL) return true;
        if (x->color == Color::RED) {
            if (x->left->color  == Color::RED) return false;
            if (x->right->color == Color::RED) return false;
        }
        return checkNoRedRed(x->left) && checkNoRedRed(x->right);
    }

    // Returns black-height of subtree x, or -1 if inconsistent
    int blackHeight(Node* x) const {
        if (x == this->NIL) return 1;   // NIL sentinel counts as BLACK
        int lh = blackHeight(x->left);
        int rh = blackHeight(x->right);
        if (lh == -1 || rh == -1 || lh != rh) return -1;
        return lh + (x->color == Color::BLACK ? 1 : 0);
    }
};

// =============================================================================
// Helper: verify a vector is strictly ascending
// =============================================================================
static void assertSorted(const std::vector<int>& v) {
    for (int i = 0; i + 1 < (int)v.size(); ++i)
        assert(v[i] < v[i + 1]);
}

// =============================================================================
// 1. EMPTY TREE EDGE CASES
// =============================================================================
void testEmptyTree() {
    section("1. Empty Tree");
    RBTreeValidator<int, int> t;

    assert(t.empty());
    assert(t.size() == 0);
    assert(t.find(42)        == nullptr);
    assert(t.front()         == nullptr);
    assert(t.back()          == nullptr);
    assert(t.lowerBound(0)   == nullptr);
    assert(t.upperBound(0)   == nullptr);
    assert(t.remove(99)      == false);
    assert(t.isValid());

    pass("empty tree — all queries return nullptr/false, invariants hold");
}

// =============================================================================
// 2. SINGLE ELEMENT
// =============================================================================
void testSingleElement() {
    section("2. Single Element");
    RBTreeValidator<int, std::string> t;
    t.insert(10, "ten");

    assert(t.size()  == 1);
    assert(!t.empty());
    assert(t.find(10) != nullptr && t.find(10)->value == "ten");
    assert(t.find(99) == nullptr);
    assert(t.front()->key == 10);
    assert(t.back()->key  == 10);

    // lowerBound / upperBound on single node
    assert(t.lowerBound(10)->key == 10);  // exact match
    assert(t.lowerBound(5)->key  == 10);  // below → returns the one node
    assert(t.lowerBound(15)      == nullptr); // above → nothing
    assert(t.upperBound(9)->key  == 10);  // below → returns the one node
    assert(t.upperBound(10)      == nullptr); // exact → strictly greater, none
    assert(t.isValid());

    // Remove the only node
    assert(t.remove(10));
    assert(t.empty());
    assert(t.find(10) == nullptr);
    assert(t.isValid());

    pass("single element — insert, find, bounds, remove");
}

// =============================================================================
// 3. INSERT & FIND  (basic)
// =============================================================================
void testInsertFind() {
    section("3. Insert & Find");
    RBTreeValidator<int, std::string> t;
    t.insert(5,  "five");
    t.insert(3,  "three");
    t.insert(7,  "seven");
    t.insert(1,  "one");
    t.insert(4,  "four");
    t.insert(6,  "six");
    t.insert(9,  "nine");

    assert(t.size() == 7);
    assert(t.find(5)->value  == "five");
    assert(t.find(1)->value  == "one");
    assert(t.find(9)->value  == "nine");
    assert(t.find(99) == nullptr);
    assert(t.find(0)  == nullptr);
    assert(t.isValid());

    pass("insert 7 nodes — find present/absent, invariants hold");
}

// =============================================================================
// 4. DUPLICATE KEY  (should update value, not grow tree)
// =============================================================================
void testDuplicateKey() {
    section("4. Duplicate Key");
    RBTreeValidator<int, std::string> t;
    t.insert(1, "first");
    t.insert(2, "alpha");
    t.insert(1, "second");   // update

    assert(t.size()           == 2);
    assert(t.find(1)->value   == "second");
    assert(t.find(2)->value   == "alpha");
    assert(t.isValid());

    // Update again
    t.insert(1, "third");
    assert(t.size()         == 2);
    assert(t.find(1)->value == "third");
    assert(t.isValid());

    pass("duplicate key updates value, size unchanged");
}

// =============================================================================
// 5. INORDER  (must be strictly ascending)
// =============================================================================
void testInorder() {
    section("5. Inorder Traversal");
    RBTreeValidator<int, int> t;
    std::vector<int> input = {4, 2, 6, 1, 3, 5, 7};
    for (int k : input) t.insert(k, k * 10);

    std::vector<int> result;
    t.inorder([&](const int& k, const int&) { result.push_back(k); });

    assert((int)result.size() == 7);
    assertSorted(result);
    assert(t.isValid());
    pass("inorder produces strictly ascending keys");

    // Values also correct
    t.inorder([&](const int& k, const int& v) {
        assert(v == k * 10);
    });
    pass("inorder values match inserted values");
}

// =============================================================================
// 6. SEQUENTIAL ASCENDING INSERT  (stress-tests left rotations)
// =============================================================================
void testAscendingInsert() {
    section("6. Sequential Ascending Insert (1..20)");
    RBTreeValidator<int, int> t;
    for (int i = 1; i <= 20; ++i) t.insert(i, i);

    assert(t.size() == 20);
    assert(t.front()->key == 1);
    assert(t.back()->key  == 20);

    auto k = t.keys();
    assertSorted(k);
    assert(t.isValid());
    pass("20 ascending inserts — sorted, invariants hold");
}

// =============================================================================
// 7. SEQUENTIAL DESCENDING INSERT  (stress-tests right rotations)
// =============================================================================
void testDescendingInsert() {
    section("7. Sequential Descending Insert (20..1)");
    RBTreeValidator<int, int> t;
    for (int i = 20; i >= 1; --i) t.insert(i, i);

    assert(t.size() == 20);
    assert(t.front()->key == 1);
    assert(t.back()->key  == 20);

    auto k = t.keys();
    assertSorted(k);
    assert(t.isValid());
    pass("20 descending inserts — sorted, invariants hold");
}

// =============================================================================
// 8. REMOVE  (leaf, one child, two children, root, non-existent)
// =============================================================================
void testRemove() {
    section("8. Remove");
    RBTreeValidator<int, int> t;
    for (int i = 1; i <= 10; ++i) t.insert(i, i);

    // Remove a leaf-like node
    assert(t.remove(1));
    assert(t.find(1) == nullptr);
    assert(t.size()  == 9);
    assert(t.isValid());
    pass("remove minimum (leaf)");

    // Remove a node with two children
    assert(t.remove(5));
    assert(t.find(5) == nullptr);
    assert(t.size()  == 8);
    assert(t.isValid());
    pass("remove node with two children");

    // Remove maximum
    assert(t.remove(10));
    assert(t.find(10) == nullptr);
    assert(t.size()   == 7);
    assert(t.isValid());
    pass("remove maximum");

    // Tree still sorted after removals
    auto k = t.keys();
    assertSorted(k);
    pass("remaining keys still sorted after removals");

    // Remove non-existent key
    assert(t.remove(99) == false);
    assert(t.size()     == 7);
    pass("remove non-existent returns false, size unchanged");

    // Remove all remaining
    for (int key : k) {
        assert(t.remove(key));
        assert(t.isValid());
    }
    assert(t.empty());
    assert(t.front()  == nullptr);
    assert(t.back()   == nullptr);
    pass("remove all nodes one by one — tree stays valid throughout");
}

// =============================================================================
// 9. REMOVE ROOT REPEATEDLY
// =============================================================================
void testRemoveRoot() {
    section("9. Remove Root Repeatedly");
    RBTreeValidator<int, int> t;
    std::vector<int> vals = {15, 10, 20, 5, 12, 18, 25};
    for (int v : vals) t.insert(v, v);

    // Keep removing front (minimum) — exercises deleteFixup on various structures
    while (!t.empty()) {
        int minKey = t.front()->key;
        assert(t.remove(minKey));
        assert(t.find(minKey) == nullptr);
        assert(t.isValid());
    }
    pass("remove minimum repeatedly until empty — invariants hold each step");
}

// =============================================================================
// 10. FRONT & BACK
// =============================================================================
void testFrontBack() {
    section("10. Front & Back");
    RBTreeValidator<int, int> t;
    t.insert(50, 50);
    t.insert(20, 20);
    t.insert(80, 80);
    t.insert(10, 10);
    t.insert(90, 90);

    assert(t.front()->key == 10);
    assert(t.back()->key  == 90);

    t.remove(10);
    assert(t.front()->key == 20);

    t.remove(90);
    assert(t.back()->key == 80);

    assert(t.isValid());
    pass("front/back track min/max correctly after removals");
}

// =============================================================================
// 11. LOWER BOUND
// =============================================================================
void testLowerBound() {
    section("11. lowerBound");
    RBTreeValidator<int, int> t;
    // Insert even keys: 2, 4, 6, 8, 10
    for (int i = 1; i <= 5; ++i) t.insert(i * 2, i * 2);

    // Exact match
    assert(t.lowerBound(4)->key  == 4);
    assert(t.lowerBound(10)->key == 10);

    // Between values — returns next higher
    assert(t.lowerBound(3)->key  == 4);
    assert(t.lowerBound(7)->key  == 8);
    assert(t.lowerBound(9)->key  == 10);

    // Below minimum — returns minimum
    assert(t.lowerBound(1)->key  == 2);

    // Above maximum — nothing
    assert(t.lowerBound(11)      == nullptr);

    assert(t.isValid());
    pass("lowerBound — exact, between, below min, above max");
}

// =============================================================================
// 12. UPPER BOUND
// =============================================================================
void testUpperBound() {
    section("12. upperBound");
    RBTreeValidator<int, int> t;
    for (int i = 1; i <= 5; ++i) t.insert(i * 2, i * 2);

    // Exact key — upper bound skips it, returns next
    assert(t.upperBound(4)->key  == 6);
    assert(t.upperBound(8)->key  == 10);

    // Between values — same as lowerBound
    assert(t.upperBound(3)->key  == 4);
    assert(t.upperBound(7)->key  == 8);

    // Below minimum — returns minimum
    assert(t.upperBound(1)->key  == 2);

    // At or above maximum — nothing
    assert(t.upperBound(10)      == nullptr);
    assert(t.upperBound(11)      == nullptr);

    assert(t.isValid());
    pass("upperBound — exact (skipped), between, below min, at/above max");
}

// =============================================================================
// 13. STRING KEYS
// =============================================================================
void testStringKeys() {
    section("13. String Keys");
    RBTreeValidator<std::string, int> t;
    t.insert("banana", 2);
    t.insert("apple",  1);
    t.insert("cherry", 3);
    t.insert("date",   4);
    t.insert("apricot", 0);

    assert(t.size()                   == 5);
    assert(t.find("apple")->value     == 1);
    assert(t.find("cherry")->value    == 3);
    assert(t.find("mango")            == nullptr);
    assert(t.front()->key             == "apple");
    assert(t.back()->key              == "date");

    std::vector<std::string> result;
    t.inorder([&](const std::string& k, const int&) { result.push_back(k); });
    for (int i = 0; i + 1 < (int)result.size(); ++i)
        assert(result[i] < result[i + 1]);

    assert(t.lowerBound("cherry")->key == "cherry");
    assert(t.upperBound("cherry")->key == "date");
    assert(t.isValid());

    pass("string keys — insert, find, inorder, bounds, invariants");
}

// =============================================================================
// 14. LARGE SCALE  (100 nodes, mixed order)
// =============================================================================
void testLargeScale() {
    section("14. Large Scale (100 nodes)");
    RBTreeValidator<int, int> t;

    // Insert in a zigzag pattern to hit all fixup cases
    std::vector<int> input;
    for (int i = 0; i < 50; ++i) {
        input.push_back(i * 2);        // 0,2,4,...,98
        input.push_back(99 - i * 2);   // 99,97,95,...,1
    }
    for (int v : input) t.insert(v, v);

    assert(t.size()        == 100);
    assert(t.front()->key  == 0);
    assert(t.back()->key   == 99);
    assert(t.isValid());
    pass("100 nodes inserted — size, front, back, invariants hold");

    // Verify sorted
    auto k = t.keys();
    assert((int)k.size() == 100);
    assertSorted(k);
    pass("inorder produces 100 sorted keys");

    // Remove every even key (50 removals)
    for (int i = 0; i < 100; i += 2) {
        assert(t.remove(i));
        assert(t.find(i) == nullptr);
    }
    assert(t.size() == 50);
    assert(t.isValid());
    pass("removed 50 even keys — size 50, invariants hold");

    // Remaining keys are all odd
    auto remaining = t.keys();
    for (int key : remaining) assert(key % 2 == 1);
    assertSorted(remaining);
    pass("remaining 50 keys are all odd and sorted");
}

// =============================================================================
// main
// =============================================================================
int main() {
    std::cout << "╔══════════════════════════════════════════════╗\n";
    std::cout << "║       RBTree Comprehensive Test Suite        ║\n";
    std::cout << "╚══════════════════════════════════════════════╝\n";

    testEmptyTree();
    testSingleElement();
    testInsertFind();
    testDuplicateKey();
    testInorder();
    testAscendingInsert();
    testDescendingInsert();
    testRemove();
    testRemoveRoot();
    testFrontBack();
    testLowerBound();
    testUpperBound();
    testStringKeys();
    testLargeScale();

    std::cout << "\n══════════════════════════════════════════════\n";
    std::cout << "  ALL TESTS PASSED\n";
    std::cout << "══════════════════════════════════════════════\n\n";
    return 0;
}
