#pragma once
#include <functional>
#include <vector>

// =============================================================================
// RBTree<Key, Value>  —  Generic Red-Black Tree
// Shared base built collectively by all four team members.
//
// A Red-Black Tree is a self-balancing BST that maintains the following
// invariants after every insert/delete:
//   1. Every node is RED or BLACK.
//   2. The root is always BLACK.
//   3. Every leaf (NIL sentinel) is BLACK.
//   4. If a node is RED, both its children are BLACK.
//   5. Every path from a node to any descendant NIL has the same number
//      of BLACK nodes (black-height).
//
// These invariants guarantee O(log n) height → O(log n) insert, delete, find.
//
// Augmentation: individual modules extend this by adding extra fields to their
// value types and updating those fields inside the rotation hook.
// =============================================================================

template <typename Key, typename Value>
class RBTree {
public:

    // ── Color ─────────────────────────────────────────────────────────────────
    enum class Color { RED, BLACK };

    // ── Node ──────────────────────────────────────────────────────────────────
    // Each tree node stores a key-value pair plus structural pointers.
    // The NIL sentinel is a single shared BLACK node used instead of nullptr,
    // so every leaf check is just (node == NIL) — no null-pointer branches.
    struct Node {
        Key    key;     // used for BST ordering
        Value  value;   // satellite data (student, event, book, waitlist entry …)
        Color  color;   // RED or BLACK — maintained by fix-up routines
        Node*  left;    // left child  (NIL if no left child)
        Node*  right;   // right child (NIL if no right child)
        Node*  parent;  // parent pointer (NIL->parent == NIL for the root)

        Node(const Key& k, const Value& v)
            : key(k), value(v), color(Color::RED),
              left(nullptr), right(nullptr), parent(nullptr) {}
    };

    // ── Construction / destruction ─────────────────────────────────────────────
    RBTree();   // initialise NIL sentinel; set root = NIL; nodeCount = 0
    ~RBTree();  // free all nodes via destroyTree(root), then delete NIL

    // ── Core operations ───────────────────────────────────────────────────────

    // Insert key-value pair.
    // Steps: standard BST insert → colour new node RED → call insertFixup.
    // O(log n)
    void insert(const Key& key, const Value& value);

    // Remove the node whose key matches exactly.
    // Steps: find node → splice out (using transplant) → call deleteFixup if needed.
    // Returns false if key not found.
    // O(log n)
    bool remove(const Key& key);

    // Return pointer to the node with exactly this key, or nullptr if absent.
    // Standard BST search — go left if key < node.key, right otherwise.
    // O(log n)
    Node* find(const Key& key) const;

    // ── Range / bound queries ─────────────────────────────────────────────────

    // First node whose key >= the given key  (like std::lower_bound).
    // Walk the tree keeping track of the last node where we went right.
    // Return nullptr if no such node exists.
    // O(log n)
    Node* lowerBound(const Key& key) const;

    // First node whose key > the given key  (like std::upper_bound).
    // Same idea as lowerBound but condition is strictly greater.
    // O(log n)
    Node* upperBound(const Key& key) const;

    // ── Min / max ─────────────────────────────────────────────────────────────

    // Leftmost node = minimum key.  Returns nullptr on empty tree.
    // Used by CourseRegistration::promoteFront to pop the highest-priority entry.
    // O(log n)
    Node* front() const;

    // Rightmost node = maximum key.  Returns nullptr on empty tree.
    // O(log n)
    Node* back() const;

    // ── Traversal ─────────────────────────────────────────────────────────────

    // Call visit(key, value) on every node in ascending key order.
    // Implemented as a recursive in-order traversal (left → node → right).
    // O(n)
    void inorder(std::function<void(const Key&, const Value&)> visit) const;

    // ── Size ──────────────────────────────────────────────────────────────────
    int  size()  const { return nodeCount; }
    bool empty() const { return nodeCount == 0; }

protected:
    // ── Internal state ────────────────────────────────────────────────────────
    Node* root;       // root of the tree; equals NIL when empty
    Node* NIL;        // sentinel leaf — always BLACK, shared by all null positions
    int   nodeCount;  // number of real (non-NIL) nodes

    // ── Rotation helpers ──────────────────────────────────────────────────────
    // Rotations preserve BST order and are the building block for fix-ups.
    // After rotating, call any augmentation update hook here so that
    // subtreeSize / maxEnd / subtreeBooks stay correct.

    // Left-rotate around x:
    //       x                y
    //      / \     →        / \
    //     A   y            x   C
    //        / \          / \
    //       B   C        A   B
    // x's right child (y) becomes the new subtree root.
    void leftRotate(Node* x);

    // Right-rotate around x (mirror of leftRotate).
    void rightRotate(Node* x);

    // ── Fix-up routines ───────────────────────────────────────────────────────

    // Restore RB invariants after insert.
    // z is the newly inserted RED node.
    // Three cases based on the colour of z's uncle (see CLRS §13.3):
    //   Case 1: uncle is RED   → recolour parent + uncle BLACK, grandparent RED, move z up
    //   Case 2: uncle is BLACK, z is inner child (triangle) → rotate to make it a line
    //   Case 3: uncle is BLACK, z is outer child (line)     → rotate + recolour
    void insertFixup(Node* z);

    // Restore RB invariants after delete.
    // x is the node that replaced the deleted node (may be NIL sentinel).
    // Four symmetric cases for x being a left/right child (see CLRS §13.4):
    //   Case 1: sibling w is RED                          → recolour + rotate
    //   Case 2: w is BLACK, both w's children BLACK       → recolour w RED, move x up
    //   Case 3: w is BLACK, w's far child BLACK (near RED)→ rotate w + recolour → Case 4
    //   Case 4: w is BLACK, w's far child RED             → rotate + recolour, done
    void deleteFixup(Node* x);

    // ── Utility helpers ───────────────────────────────────────────────────────

    // Replace subtree rooted at u with subtree rooted at v.
    // Updates u's parent to point to v; does NOT update v's children.
    void transplant(Node* u, Node* v);

    // Walk left pointers until hitting NIL → returns the minimum in subtree x.
    Node* minimum(Node* x) const;

    // Walk right pointers until hitting NIL → returns the maximum in subtree x.
    Node* maximum(Node* x) const;

    // Post-order recursive delete of every non-NIL node.
    void destroyTree(Node* x);
};

#include "../src/RBTree.tpp"
