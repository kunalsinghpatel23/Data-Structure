#include "splay.h"

SplayTree::SplayTree() {
    root = nullptr;
    mode = TOP_DOWN;           // default
    semiRotationLimit = 2;     // default semi limit (can change)
}

// aka Zig
SplayTree::Node* SplayTree::rotateRight(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

// aka Zag
SplayTree::Node* SplayTree::rotateLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

void SplayTree::setMode(Mode m) {
    mode = m;
}

void SplayTree::setSemiRotationLimit(int k) {
    if (k < 0) k = 0;
    semiRotationLimit = k;
}

//Bottom-up (recursive) splay

SplayTree::Node* SplayTree::splayBottomUp(Node* r, int key) {
    if (r == nullptr || r->key == key) return r;

    if (key < r->key) {
        if (r->left == nullptr) return r;

        // Zig-Zig
        if (key < r->left->key) {
            r->left->left = splayBottomUp(r->left->left, key);
            r = rotateRight(r);
        }
        // Zig-Zag
        else if (key > r->left->key) {
            r->left->right = splayBottomUp(r->left->right, key);
            if (r->left->right != nullptr) {
                r->left = rotateLeft(r->left);
            }
        }

        return (r->left == nullptr) ? r : rotateRight(r);
    }
    else {
        if (r->right == nullptr) return r;

        // Zag-Zag
        if (key > r->right->key) {
            r->right->right = splayBottomUp(r->right->right, key);
            r = rotateLeft(r);
        }
        // Zag-Zig
        else if (key < r->right->key) {
            r->right->left = splayBottomUp(r->right->left, key);
            if (r->right->left != nullptr) {
                r->right = rotateRight(r->right);
            }
        }

        return (r->right == nullptr) ? r : rotateLeft(r);
    }
}


//Top-down (iterative) splay (full splay)

SplayTree::Node* SplayTree::splayTopDown(Node* r, int key) {
    if (r == nullptr) return nullptr;

    Node header(0); // temporary tree
    header.left = header.right = nullptr;

    Node* leftMax = &header;   // right spine of left tree
    Node* rightMin = &header;  // left spine of right tree

    while (true) {
        if (key < r->key) {
            if (r->left == nullptr) break;

            if (key < r->left->key) {
                r = rotateRight(r);
                if (r->left == nullptr) break;
            }

            // link to right tree
            rightMin->left = r;
            rightMin = r;
            r = r->left;
        }
        else if (key > r->key) {
            if (r->right == nullptr) break;

            if (key > r->right->key) {
                r = rotateLeft(r);
                if (r->right == nullptr) break;
            }

            // link to left tree
            leftMax->right = r;
            leftMax = r;
            r = r->right;
        }
        else {
            break;
        }
    }

    // reassemble
    leftMax->right = r->left;
    rightMin->left = r->right;
    r->left = header.right;
    r->right = header.left;

    return r;
}


/*  Top - down semi - splay(OPTION 1) : stop after k rotations
    still does the top-down split/link, just limits rotate calls. */

SplayTree::Node* SplayTree::splayTopDownSemi(Node* r, int key, int k) {
    if (r == nullptr) return nullptr;
    if (k <= 0) return r;

    Node header(0);
    header.left = header.right = nullptr;

    Node* leftMax = &header;
    Node* rightMin = &header;

    int rotations = 0;

    while (true) {
        if (key < r->key) {
            if (r->left == nullptr) break;

            if (key < r->left->key && rotations < k) {
                r = rotateRight(r);
                rotations++;
                if (r->left == nullptr) break;
            }

            rightMin->left = r;
            rightMin = r;
            r = r->left;
        }
        else if (key > r->key) {
            if (r->right == nullptr) break;

            if (key > r->right->key && rotations < k) {
                r = rotateLeft(r);
                rotations++;
                if (r->right == nullptr) break;
            }

            leftMax->right = r;
            leftMax = r;
            r = r->right;
        }
        else {
            break;
        }

// if we already used all rotations, stop early (half wayish behavior)
        if (rotations >= k) break;
    }

    leftMax->right = r->left;
    rightMin->left = r->right;
    r->left = header.right;
    r->right = header.left;

    return r;
}

/*  Weighted top - down splay(OPTION 1) :
    Rotate only if child.weight >= parent.weight */

SplayTree::Node* SplayTree::splayTopDownWeighted(Node* r, int key) {
    if (r == nullptr) return nullptr;

    auto canRotate = [](Node* parent, Node* child) -> bool {
        if (parent == nullptr || child == nullptr) return false;
        return child->weight >= parent->weight;
        };

    Node header(0);
    header.left = header.right = nullptr;

    Node* leftMax = &header;
    Node* rightMin = &header;

    while (true) {
        if (key < r->key) {
            if (r->left == nullptr) break;

            // only rotate if weight rule allows it
            if (key < r->left->key && canRotate(r, r->left)) {
                r = rotateRight(r);
                if (r->left == nullptr) break;
            }

            rightMin->left = r;
            rightMin = r;
            r = r->left;
        }
        else if (key > r->key) {
            if (r->right == nullptr) break;

            if (key > r->right->key && canRotate(r, r->right)) {
                r = rotateLeft(r);
                if (r->right == nullptr) break;
            }

            leftMax->right = r;
            leftMax = r;
            r = r->right;
        }
        else {
            break;
        }
    }

    leftMax->right = r->left;
    rightMin->left = r->right;
    r->left = header.right;
    r->right = header.left;

    return r;
}


 // Mode wrapper

SplayTree::Node* SplayTree::splay(Node* r, int key) {
    if (mode == BOTTOM_UP) {
        return splayBottomUp(r, key);
    }
    if (mode == SEMI_SPLAY_TOP_DOWN) {
        return splayTopDownSemi(r, key, semiRotationLimit);
    }
    if (mode == WEIGHTED_TOP_DOWN) {
        return splayTopDownWeighted(r, key);
    }
    // default
    return splayTopDown(r, key);
}

SplayTree::Node* SplayTree::insertNode(Node* r, int key) {
    if (r == nullptr) {
        return new Node(key);
    }

    r = splay(r, key);

    // already exists
    if (r->key == key) {
        return r;
    }

    Node* n = new Node(key);

    if (key < r->key) {
        n->right = r;
        n->left = r->left;
        r->left = nullptr;
    }
    else {
        n->left = r;
        n->right = r->right;
        r->right = nullptr;
    }

    return n;
}

SplayTree::Node* SplayTree::deleteNode(Node* r, int key) {
    if (r == nullptr) return nullptr;

    r = splay(r, key);

    if (r == nullptr || r->key != key) {
        return r; 
    }

    Node* toDelete = r;

    if (r->left == nullptr) {
        r = r->right;
        delete toDelete;
        return r;
    }

    // keep right subtree aside
    Node* rightSub = r->right;

    // bring the max of left subtree to root
    // (splaying with the same key works because key > any key in left subtree)
    r = splay(r->left, key);

    r->right = rightSub;

    delete toDelete;
    return r;
}

void SplayTree::insert(int key) {
    root = insertNode(root, key);
}

void SplayTree::remove(int key) {
    root = deleteNode(root, key);
}

bool SplayTree::search(int key) {
    root = splay(root, key);

    bool found = (root != nullptr && root->key == key);

    // weighted rule: increment only on successful search
    if (found && mode == WEIGHTED_TOP_DOWN) {
        root->weight++;
    }

    return found;
}

void SplayTree::printTree(Node* r, int space) {
    const int COUNT = 10;

    if (r == nullptr) return;

    space += COUNT;

    printTree(r->right, space);

    for (int i = COUNT; i < space; i++) {
        cout << " ";
    }

    // show key (and weight if weighted mode)
    if (mode == WEIGHTED_TOP_DOWN) {
        cout << r->key << "(" << r->weight << ")" << endl;
    }
    else {
        cout << r->key << endl;
    }

    printTree(r->left, space);
}

void SplayTree::display() {
    printTree(root, 0);
    cout << endl;
}