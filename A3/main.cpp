#include <iostream>
#include <stdexcept>
using namespace std;

class LLRBTree
{
private:
    static const bool RED = true;
    static const bool BLACK = false;

    struct Node
    {
        int key;
        bool color;
        Node *left;
        Node *right;

        Node(int k) : key(k), color(RED), left(nullptr), right(nullptr) {}
    };

    Node *root = nullptr;

    bool isRed(Node *x)
    {
        return x != nullptr && x->color == RED;
    }

    Node *rotateLeft(Node *h)
    {
        Node *x = h->right;
        h->right = x->left;
        x->left = h;
        x->color = h->color;
        h->color = RED;
        return x;
    }

    Node *rotateRight(Node *h)
    {
        Node *x = h->left;
        h->left = x->right;
        x->right = h;
        x->color = h->color;
        h->color = RED;
        return x;
    }

    void flipColors(Node *h)
    {
        h->color = !h->color;
        if (h->left)
            h->left->color = !h->left->color;
        if (h->right)
            h->right->color = !h->right->color;
    }

    Node *insert(Node *h, int key)
    {
        if (h == nullptr)
        {
            return new Node(key);
        }

        if (key < h->key)
        {
            h->left = insert(h->left, key);
        }
        else if (key > h->key)
        {
            h->right = insert(h->right, key);
        }
        else
        {
            throw runtime_error("Duplicate key insertion is not allowed.");
        }

        if (isRed(h->right) && !isRed(h->left))
            h = rotateLeft(h);

        if (isRed(h->left) && isRed(h->left->left))
            h = rotateRight(h);

        if (isRed(h->left) && isRed(h->right))
            flipColors(h);

        return h;
    }

    void inorder(Node *h)
    {
        if (!h)
            return;
        inorder(h->left);
        cout << h->key << (h->color == RED ? "(R) " : "(B) ");
        inorder(h->right);
    }

    void clear(Node *h)
    {
        if (!h)
            return;
        clear(h->left);
        clear(h->right);
        delete h;
    }

public:
    ~LLRBTree()
    {
        clear(root);
    }

    void insert(int key)
    {
        root = insert(root, key);
        if (root)
            root->color = BLACK;
    }

    void printInOrder()
    {
        if (!root)
        {
            throw runtime_error("Tree is empty.");
        }
        inorder(root);
        cout << endl;
    }
};

int main()
{
    LLRBTree tree;

    try
    {
        tree.insert(10);
        tree.insert(20);
        tree.insert(30);
        tree.insert(15);
        tree.insert(5);

        cout << "Inorder traversal of LLRB tree: ";
        tree.printInOrder();
    }
    catch (const exception &e)
    {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}