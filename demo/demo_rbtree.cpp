// =============================================================================
// demo_rbtree.cpp  —  Human-readable RBTree walkthrough
// Run: g++ -std=c++17 -I../include demo_rbtree.cpp -o demo_rbtree
// =============================================================================

#include "RBTree.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

// ── RBTreeValidator (needed for isValid()) ────────────────────────────────────
template <typename Key, typename Value>
class RBTreeValidator : public RBTree<Key, Value> {
    using Node  = typename RBTree<Key, Value>::Node;
    using Color = typename RBTree<Key, Value>::Color;
public:
    bool isValid() const { return rootIsBlack() && noRedRed() && blackHeightConsistent(); }
private:
    bool rootIsBlack() const {
        return this->root == this->NIL || this->root->color == Color::BLACK;
    }
    bool noRedRed() const { return checkNoRedRed(this->root); }
    bool blackHeightConsistent() const { return blackHeight(this->root) != -1; }

    bool checkNoRedRed(Node* x) const {
        if (x == this->NIL) return true;
        if (x->color == Color::RED) {
            if (x->left->color  == Color::RED) return false;
            if (x->right->color == Color::RED) return false;
        }
        return checkNoRedRed(x->left) && checkNoRedRed(x->right);
    }
    int blackHeight(Node* x) const {
        if (x == this->NIL) return 1;
        int lh = blackHeight(x->left);
        int rh = blackHeight(x->right);
        if (lh == -1 || rh == -1 || lh != rh) return -1;
        return lh + (x->color == Color::BLACK ? 1 : 0);
    }
};

// ── demo ──────────────────────────────────────────────────────────────────────
int main() {

    std::cout << "  DEMO Student GPA Index (key=GPA*100, value=name)\n";

    RBTreeValidator<int, std::string> t;

    struct Student { int gpa; std::string name; };
    std::vector<Student> students = {
        {350, "Alice"},  {280, "Bob"},   {390, "Carol"},
        {310, "Dave"},   {265, "Eve"},   {375, "Frank"},
        {300, "Grace"},  {410, "Heidi"}, {290, "Ivan"},
    };

    std::cout << "\n  Inserting students:\n";
    for (auto& s : students) {
        t.insert(s.gpa, s.name);
        std::cout << "    insert GPA=" << s.gpa / 100.0
                  << "  name=" << s.name << "\n";
    }

    std::cout << "\n  Tree valid: " << (t.isValid() ? "YES" : "NO") << "\n";
    std::cout << "  Size: " << t.size() << "\n";
    std::cout << "  Min GPA: " << t.front()->value
              << " (" << t.front()->key / 100.0 << ")\n";
    std::cout << "  Max GPA: " << t.back()->value
              << " (" << t.back()->key / 100.0 << ")\n";

    std::cout << "\n  All students in GPA order:\n";
    t.inorder([](const int& gpa, const std::string& name) {
        std::cout << "    " << gpa / 100.0 << "  " << name << "\n";
    });

    // Range query: GPA between 3.00 and 3.75
    int lo = 300, hi = 375;
    std::cout << "\n  Students with GPA in [3.00, 3.75]:\n";
    auto* node = t.lowerBound(lo);
    while (node != nullptr && node->key <= hi) {
        std::cout << "    " << node->key / 100.0
                  << "  " << node->value << "\n";
        node = t.upperBound(node->key);
    }

    // Remove a student
    std::cout << "\n  Removing Bob (GPA=2.80)...\n";
    t.remove(280);
    std::cout << "  Size after removal: " << t.size() << "\n";
    std::cout << "  Tree valid: " << (t.isValid() ? "YES" : "NO") << "\n";

    // Duplicate key update
    std::cout << "\n  Alice retakes exam GPA updated to 4.00\n";
    t.insert(350, "Alice*");
    t.insert(400, "Alice");
    std::cout << "  find(350): " << t.find(350)->value << "\n";
    std::cout << "  find(400): " << t.find(400)->value << "\n";
    std::cout << "  Size: " << t.size() << " (unchanged for key 350 update)\n";

    assert(t.isValid());
    std::cout << "\n  [PASS] demo completed  all assertions passed\n\n";
    return 0;
}
