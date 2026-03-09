#include "dsw.h"

// ----------------- PRIVATE ----------------------------------

// rotate right
void BST::rotateRight(Node*& node)
{
    if (node == nullptr || node->left == nullptr)
        return;

    Node* leftChild = node->left;
    node->left = leftChild->right;
    leftChild->right = node;
    node = leftChild;
}

// rotate left
void BST::rotateLeft(Node*& node)
{
    if (node == nullptr || node->right == nullptr)
        return;

    Node* rightChild = node->right;
    node->right = rightChild->left;
    rightChild->left = node;
    node = rightChild;
}

// check if subtree has size <= 2
bool BST::isSmallSubtree(Node* node)
{
    if (node == nullptr)
        return true; // size 0

    // if node has no children, size is 1
    if (node->left == nullptr && node->right == nullptr)
        return true;

    // if both children exist, size is at least 3
    if (node->left != nullptr && node->right != nullptr)
        return false;

    // only left child exists
    if (node->left != nullptr)
    {
        if (node->left->left == nullptr && node->left->right == nullptr)
            return true; // size 2
        else
            return false;
    }

    // only right child exists
    if (node->right != nullptr)
    {
        if (node->right->left == nullptr && node->right->right == nullptr)
            return true; // size 2
        else
            return false;
    }

    return false;
}

// count all nodes in tree using Morris traversal
int BST::countNodes()
{
    int count = 0;
    Node* curr = root;

    while (curr != nullptr)
    {
        if (curr->left == nullptr)
        {
            count++;
            curr = curr->right;
        }
        else
        {
            Node* pred = curr->left;
            while (pred->right != nullptr && pred->right != curr)
                pred = pred->right;

            if (pred->right == nullptr)
            {
                pred->right = curr;
                curr = curr->left;
            }
            else
            {
                pred->right = nullptr;
                count++;
                curr = curr->right;
            }
        }
    }

    return count;
}

// Phase 1
// create a partially left-skewed vine
// rotate left when there is a right child
// skip rotation if the right subtree is very small
void BST::createVine()
{
    Node dummy(0);
    dummy.left = root;

    Node* current = &dummy;

    while (current->left != nullptr)
    {
        if (current->left->right != nullptr &&
            !isSmallSubtree(current->left->right))
        {
            rotateLeft(current->left);
        }
        else
        {
            current = current->left;
        }
    }

    root = dummy.left;
}

// Phase 2
// do right rotations to reduce height
// do not aim for perfect balancing
void BST::rebuildTree(int size)
{
    if (size <= 1)
        return;

    int targetHeight = (int)ceil(2 * log2(size));
    if (targetHeight < 1)
        targetHeight = 1;

    int passes = 0;
    int estimatedHeight = size;

    while (estimatedHeight > targetHeight)
    {
        estimatedHeight = (estimatedHeight + 1) / 2;
        passes++;
    }

    int rotationCount = size / 2;

    for (int i = 0; i < passes && rotationCount > 0; i++)
    {
        performRotation(rotationCount);
        rotationCount /= 2;
    }
}

// do right rotation on every second node
void BST::performRotation(int count)
{
    Node dummy(0);
    dummy.left = root;

    Node* scanner = &dummy;

    for (int i = 0; i < count; i++)
    {
        if (scanner->left == nullptr || scanner->left->left == nullptr)
            break;

        rotateRight(scanner->left);

        // move forward
        if (scanner->left != nullptr)
            scanner = scanner->left->right;
        else
            break;

        if (scanner == nullptr)
            break;
    }

    root = dummy.left;
}

void BST::printTree(Node* root, int space)
{
    const int COUNT = 10;

    if (root == nullptr)
        return;

    space += COUNT;

    printTree(root->right, space);

    for (int i = COUNT; i < space; i++)
        cout << " ";

    cout << root->data << endl;

    printTree(root->left, space);
}

// --------------------- PUBLIC ------------------

BST::BST()
{
    root = nullptr;
}

BST::~BST()
{
    deleteTree(root);
}

void BST::deleteTree(Node*& node)
{
    if (node == nullptr)
        return;

    deleteTree(node->left);
    deleteTree(node->right);

    delete node;
    node = nullptr;
}

void BST::insert(int val)
{
    Node* newNode = new Node(val);

    if (root == nullptr)
    {
        root = newNode;
        return;
    }

    Node* curr = root;
    Node* parent = nullptr;

    while (curr != nullptr)
    {
        parent = curr;

        if (val == curr->data)
        {
            delete newNode;
            throw invalid_argument("Duplicate values are not allowed in this BST.");
        }
        else if (val < curr->data)
        {
            curr = curr->left;
        }
        else
        {
            curr = curr->right;
        }
    }

    if (val < parent->data)
        parent->left = newNode;
    else
        parent->right = newNode;
}

void BST::dswBalance()
{
    if (root == nullptr)
        throw runtime_error("Cannot balance an empty tree.");

    int size = countNodes();

    if (size <= 1)
        return;

    createVine();
    rebuildTree(size);
}

void BST::display()
{
    if (root == nullptr)
        throw runtime_error("Tree is empty. Nothing to display.");

    cout << endl;
    printTree(root, 0);
    cout << endl;
}  