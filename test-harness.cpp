#include "merkle-tree.h"
#include <iostream>

using namespace std;

int main() {
  srand(137);
  MerkleTree<int> m;
  m.insert(5);
  m.insert(7);
  m.insert(3);
  m.insert(0);
  m.insert(-1);
  m.traverseTree(m._root);
}
