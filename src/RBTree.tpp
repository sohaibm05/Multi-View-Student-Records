// Shared — RBTree template implementation
// Built collectively by all team members

template <typename Key, typename Value>
RBTree<Key, Value>::RBTree() {}

template <typename Key, typename Value>
RBTree<Key, Value>::~RBTree() {}

template <typename Key, typename Value>
void RBTree<Key, Value>::destroyTree(Node* x) {}

template <typename Key, typename Value>
void RBTree<Key, Value>::leftRotate(Node* x) {}

template <typename Key, typename Value>
void RBTree<Key, Value>::rightRotate(Node* x) {}

template <typename Key, typename Value>
void RBTree<Key, Value>::insertFixup(Node* z) {}

template <typename Key, typename Value>
void RBTree<Key, Value>::deleteFixup(Node* x) {}

template <typename Key, typename Value>
void RBTree<Key, Value>::transplant(Node* u, Node* v) {}

template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::minimum(Node* x) const { return nullptr; }

template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::maximum(Node* x) const { return nullptr; }

template <typename Key, typename Value>
void RBTree<Key, Value>::insert(const Key& key, const Value& value) {}

template <typename Key, typename Value>
bool RBTree<Key, Value>::remove(const Key& key) { return false; }

template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::find(const Key& key) const { return nullptr; }

template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::lowerBound(const Key& key) const { return nullptr; }

template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::upperBound(const Key& key) const { return nullptr; }

template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::front() const { return minimum(root); }

template <typename Key, typename Value>
typename RBTree<Key, Value>::Node* RBTree<Key, Value>::back() const { return maximum(root); }

template <typename Key, typename Value>
void RBTree<Key, Value>::inorder(std::function<void(const Key&, const Value&)> visit) const {}
