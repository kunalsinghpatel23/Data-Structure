#include "splay.h"

int main() {
    SplayTree tree;

    // we can this to test different tasks:
    // tree.setMode(SplayTree::BOTTOM_UP);
    tree.setMode(SplayTree::TOP_DOWN);

    // For semi-splay:
    // tree.setMode(SplayTree::SEMI_SPLAY_TOP_DOWN);
    // tree.setSemiRotationLimit(2);
    // For weighted-splay:
    // tree.setMode(SplayTree::WEIGHTED_TOP_DOWN);

    tree.insert(57);
    tree.insert(31);
    tree.insert(72);
    tree.insert(44);
    tree.insert(69);
    tree.insert(83);

    cout << "Tree after insertions:\n";
    tree.display();

    tree.search(44);
    cout << "Tree after searching/splaying 44:\n";
    tree.display();

    tree.remove(31);
    cout << "Tree after deleting 31:\n";
    tree.display();

    return 0;
}