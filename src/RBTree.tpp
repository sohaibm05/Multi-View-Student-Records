// =============================================================================
// RBTree.tpp  —  Template implementation
// Included at the bottom of RBTree.hpp so the compiler sees the full
// definition whenever the header is included.
// =============================================================================

// ── Constructor ───────────────────────────────────────────────────────────────
// 1. Allocate the NIL sentinel with a default key/value.
// 2. Set its colour to BLACK (invariant: leaves are always BLACK).
// 3. Point all its own pointers to itself (or nullptr — both work since we
//    never dereference NIL's children).
// 4. Set root = NIL (empty tree).
// 5. nodeCount = 0.
template <typename Key, typename Value>
RBTree<Key, Value>::RBTree() {
    NIL = new Node(Key{}, Value{});  // sentinel leaf — shared by all null positions
    NIL->color  = Color::BLACK;      // invariant 3: every leaf (NIL) is BLACK
    NIL->left   = NIL;               // NIL points to itself
    NIL->right  = NIL;
    NIL->parent = NIL;
    root      = NIL;                 // empty tree: root IS the NIL sentinel
    nodeCount = 0;
}

// ── Destructor ────────────────────────────────────────────────────────────────
// Call destroyTree(root) to free every real node, then delete the NIL sentinel.
template <typename Key, typename Value>
RBTree<Key, Value>::~RBTree() {
    destroyTree(root);
    delete NIL;
}

// ── destroyTree ───────────────────────────────────────────────────────────────
// Post-order traversal: destroy left subtree, destroy right subtree, delete x.
// Stop when x == NIL (base case — do NOT delete the sentinel here).
template <typename Key, typename Value>
void RBTree<Key, Value>::destroyTree(Node* x) {
    if (x == NIL) return;
    destroyTree(x->left);
    destroyTree(x->right);
    delete x;
}

// ── leftRotate ────────────────────────────────────────────────────────────────
// Standard left rotation around node x (see diagram in RBTree.hpp).
// Steps (CLRS §13.2 LEFT-ROTATE):
//   1. y = x->right
//   2. x->right = y->left       (move y's left subtree to x's right)
//   3. if y->left != NIL, set y->left->parent = x
//   4. y->parent = x->parent    (link y to x's old parent)
//   5. update x->parent's child pointer to y (or root = y if x was root)
//   6. y->left = x              (x becomes y's left child)
//   7. x->parent = y
// After rotating: if you have augmentation fields (subtreeSize, maxEnd, etc.)
// update x first (it's now lower), then y (it's now higher).
template <typename Key, typename Value>
void RBTree<Key, Value>::leftRotate(Node* x) {}

// ── rightRotate ───────────────────────────────────────────────────────────────
// Mirror of leftRotate.  Replace every "left↔right" in the steps above.
template <typename Key, typename Value>
void RBTree<Key, Value>::rightRotate(Node* x) {}

// ── insert ────────────────────────────────────────────────────────────────────
// 1. Allocate a new Node(key, value) coloured RED.
// 2. Standard BST walk to find the insertion point (track parent with a
//    trailing pointer).
// 3. Link the new node into the tree (set parent, and update parent's
//    left or right child).
// 4. Set new node's left = NIL, right = NIL.
// 5. Increment nodeCount.
// 6. Call insertFixup(newNode) to restore RB invariants.
template <typename Key, typename Value>
void RBTree<Key, Value>::insert(const Key& key, const Value& value) {}

// ── insertFixup ───────────────────────────────────────────────────────────────
// Loop while z's parent is RED (violation of invariant 4).
// Determine whether z is in the left or right subtree of its grandparent,
// then apply the three cases symmetrically.
// After the loop, force root to BLACK (invariant 2).
// Reference: CLRS 4th ed. §13.3, RB-INSERT-FIXUP pseudocode.
template <typename Key, typename Value>
void RBTree<Key, Value>::insertFixup(Node* z) {}

// ── remove ────────────────────────────────────────────────────────────────────
// 1. Find the node y to delete (may be the target or its in-order successor).
// 2. Track x (the node that will move into y's position) and its original colour.
// 3. Use transplant to splice out y.
// 4. Decrement nodeCount.
// 5. If the removed colour was BLACK, call deleteFixup(x) — removing a BLACK
//    node reduces black-height on one path, which must be fixed.
// Returns false if key is not in the tree.
// Reference: CLRS 4th ed. §13.4, RB-DELETE pseudocode.
template <typename Key, typename Value>
bool RBTree<Key, Value>::remove(const Key& key) { return false; }

// ── deleteFixup ───────────────────────────────────────────────────────────────
// Loop while x != root and x is BLACK (x has a "double-black" problem).
// At each step find x's sibling w and apply one of four cases:
//   Case 1 → recolour + rotate, converts to case 2/3/4
//   Case 2 → recolour w RED, move x up (shortens double-black path)
//   Case 3 → rotate + recolour, converts to case 4
//   Case 4 → rotate + recolour, terminates the loop
// After loop: set x->colour = BLACK to absorb the extra black.
// Reference: CLRS 4th ed. §13.4, RB-DELETE-FIXUP pseudocode.
template <typename Key, typename Value>
void RBTree<Key, Value>::deleteFixup(Node* x) {}

// ── transplant ────────────────────────────────────────────────────────────────
// Replace the subtree rooted at u with the subtree rooted at v:
//   if u is the root → root = v
//   else update u->parent's left or right pointer to v
//   then v->parent = u->parent
// Note: does NOT update v->left or v->right — caller handles that.
template <typename Key, typename Value>
void RBTree<Key, Value>::transplant(Node* u, Node* v) {}

// ── minimum ───────────────────────────────────────────────────────────────────
// Walk x->left until hitting NIL.  The last real node is the minimum.
// Used internally by remove (to find in-order successor) and by front().
template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::minimum(Node* x) const {
    if (x == NIL) return nullptr;
    while (x->left != NIL)
        x = x->left;
    return x;
}

// ── maximum ───────────────────────────────────────────────────────────────────
// Walk x->right until hitting NIL.  Mirror of minimum.
template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::maximum(Node* x) const {
    if (x == NIL) return nullptr;
    while (x->right != NIL)
        x = x->right;
    return x;
}

// ── find ──────────────────────────────────────────────────────────────────────
// Standard BST search:
//   start at root; while current != NIL:
//     if key == current->key → return current
//     if key  < current->key → go left
//     else                   → go right
// Return nullptr if not found.
template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::find(const Key& key) const {
    Node* current = root;
    while (current != NIL) {
        if (key == current->key)
            return current;
        else if (key < current->key)
            current = current->left;
        else
            current = current->right;
    }
    return nullptr;
}

// ── lowerBound ────────────────────────────────────────────────────────────────
// Find the first node with key >= the argument (leftmost node that could match).
// Algorithm: BST walk; whenever you go right, remember the current node as a
// candidate.  When you fall off the tree, return the last candidate (or nullptr).
template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::lowerBound(const Key& key) const { return nullptr; }

// ── upperBound ────────────────────────────────────────────────────────────────
// Same as lowerBound but returns the first node with key > the argument.
// Only difference: candidate is updated when node->key > key (not >=).
template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::upperBound(const Key& key) const { return nullptr; }

// ── front ─────────────────────────────────────────────────────────────────────
// Return minimum(root).  Return nullptr when the tree is empty (root == NIL).
template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::front() const { return minimum(root); }

// ── back ──────────────────────────────────────────────────────────────────────
// Return maximum(root).  Return nullptr when the tree is empty (root == NIL).
template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::back() const { return maximum(root); }

// ── inorder ───────────────────────────────────────────────────────────────────
// Recursive in-order traversal: left subtree → visit current → right subtree.
// Skip NIL nodes.  Produces keys in ascending order.
// Helper signature (private, optional): inorderHelper(Node* x, visit)
template <typename Key, typename Value>
void RBTree<Key, Value>::inorder(std::function<void(const Key&, const Value&)> visit) const {}
