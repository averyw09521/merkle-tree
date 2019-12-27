//#ifdef MERKLETREE_INCLUDED
//#define MERKLETREE_INCLUDED

#include <functional>
#include <string>
#include <iostream>
#include <sstream>
#include "sha256.h"
#include <cstdlib>

template <typename T>
class MerkleTree {

public:
  struct MerkleNode {
    MerkleNode *left, *right;
    MerkleNode *prev;
    std::string hash;
    T* data;

    MerkleNode(MerkleNode* left = nullptr, MerkleNode* right = nullptr) : 
      left(nullptr), right(nullptr), prev(nullptr), data(nullptr) {
        hash = sha256(toString(rand()));
      }

    MerkleNode(std::string hash, MerkleNode* left = nullptr, MerkleNode* right = nullptr) :
      left(left), right(right), prev(nullptr), hash(hash), data(nullptr) {}

  };

  

  MerkleTree();
  ~MerkleTree();

  void insert(const T& element);
  void traverseTree(MerkleNode* curr);
  MerkleNode *_root;

private:
  std::size_t _size;
  std::size_t _layers;
  MerkleNode *_end;

  void growLayer();
  MerkleNode* createEmptyTree(int layers);
  void updateHashesUpward(MerkleNode* curr);
  MerkleNode* findNextNode(MerkleNode* curr);
  static std::string toString(const T& val);
  MerkleNode* findLeftMostInRightSubtree(MerkleNode* curr);
};

template <typename T>
MerkleTree<T>::MerkleTree() : _root(nullptr), _size(0), _layers(0), _end(nullptr) {}

template <typename T>
MerkleTree<T>::~MerkleTree() {}

template <typename T>
std::string MerkleTree<T>::toString(const T& val) {
  std::ostringstream oss;
  oss << val;
  return oss.str();
}

template <typename T>
void MerkleTree<T>::insert(const T& element) {
  if (_size == 0 || _size == (1 << (_layers-1))) growLayer();
  _end->hash = sha256(toString(element));
  _end->data = new T(element);
  ++_size;
  updateHashesUpward(_end);
  _end = findNextNode(_end);
}

template <typename T>
typename MerkleTree<T>::MerkleNode* MerkleTree<T>::findNextNode(MerkleNode* curr) {
  MerkleNode *last = curr;
  // step 1: go upwards until the subtree we were is not the right  subtree of the
  // current node, or we reach the root.
  curr = curr->prev;
  while (curr != nullptr && curr == curr->right) {
    last = curr;
    curr = curr->prev;
  }
  // if we reached the root, that means we need to grow a new layer. Return default root.
  if (curr == nullptr) return _root;

  // Step 2: go to the right subtree, and find the left-most node of that subtree
  curr = findLeftMostInRightSubtree(curr);

  return curr;
}

template <typename T>
void MerkleTree<T>::growLayer() {
  if (_root == nullptr) {
    _root = new MerkleNode();
    _layers = 1;
    _end = _root;
    return;
  }
  auto leftTree = _root;
  auto rightTree = createEmptyTree(_layers++);
  _root = new MerkleNode(sha256(leftTree->hash + rightTree->hash), leftTree, rightTree);
  rightTree->prev = _root;
  leftTree->prev = _root;
  _end = findLeftMostInRightSubtree(_root);
}


template <typename T>
typename MerkleTree<T>::MerkleNode* MerkleTree<T>::findLeftMostInRightSubtree(MerkleNode* curr) {
  curr = curr->right;
  while (curr->left != nullptr) {
    curr = curr->left;
  }
  return curr;
}

template <typename T>
typename MerkleTree<T>::MerkleNode* MerkleTree<T>::createEmptyTree(int layers) {
  if (layers == 1) return new MerkleNode();

  auto leftTree = createEmptyTree(layers-1);
  auto rightTree = createEmptyTree(layers-1);
  auto result = new MerkleNode(sha256(leftTree->hash + rightTree->hash), leftTree, rightTree);
  leftTree->prev = result;
  rightTree->prev = result;
  return result;
}

template <typename T>
void MerkleTree<T>::traverseTree(MerkleNode* curr) {
  if (curr == nullptr) return;
  std::cout << curr << std::endl;
  std::cout << curr->hash << std::endl;
  traverseTree(curr->left);
  traverseTree(curr->right);
}

template <typename T>
void MerkleTree<T>::updateHashesUpward(MerkleNode* curr) {
  while (curr != nullptr) {
    if (curr->left != nullptr) {
      curr->hash = sha256(curr->left->hash + curr->right->hash);
    }
    curr = curr->prev;
  }
}

template <typename T>


//#endif

