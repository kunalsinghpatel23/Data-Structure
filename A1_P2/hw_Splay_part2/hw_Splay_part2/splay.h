#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#include <iostream>
using namespace std;

class SplayTree {
private:
    struct Node {
        int key;
        int weight;     // used for weighted-splay (frequency)
        Node* left;
        Node* right;

        Node(int k) {
            key = k;
            weight = 1;
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root;

public:
    enum Mode {
        BOTTOM_UP,
        TOP_DOWN,
        SEMI_SPLAY_TOP_DOWN,
        WEIGHTED_TOP_DOWN
    };

private:
    Mode mode;
    int semiRotationLimit; // used for semi-splay option 1

    Node* rotateRight(Node* x);
    Node* rotateLeft(Node* x);

    // splay variants
    Node* splayBottomUp(Node* root, int key);
    Node* splayTopDown(Node* root, int key);
    Node* splayTopDownSemi(Node* root, int key, int k);
    Node* splayTopDownWeighted(Node* root, int key);

    // wrappers that pick a mode
    Node* splay(Node* root, int key);

    Node* insertNode(Node* root, int key);
    Node* deleteNode(Node* root, int key);

    void printTree(Node* root, int space);

public:
    SplayTree();

    void setMode(Mode m);
    void setSemiRotationLimit(int k);

    void insert(int key);
    void remove(int key);
    bool search(int key);
    void display();
};

#endif
