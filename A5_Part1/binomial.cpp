#include "binomial.h"

// ---------------------- NODE CONSTRUCTOR ------------------------------
binomialNode::binomialNode(int k)
{
    key = k;
    degree = 0;
    sibling = nullptr;
    child = nullptr;
    parent = nullptr;
}

// ---------------------- PRIVATE FUNCTIONS ------------------------------

// Merges two root lists in sorted order by degree.
// This does NOT combine trees of same degree yet.
binomialNode* BinomialHeap::unionHeap(binomialNode* heap1, binomialNode* heap2)
{
    if (heap1 == nullptr)
        return heap2;
    if (heap2 == nullptr)
        return heap1;

    binomialNode* mergedHead = nullptr;
    binomialNode* tail = nullptr;

    while (heap1 != nullptr && heap2 != nullptr)
    {
        binomialNode* temp = nullptr;

        if (heap1->degree <= heap2->degree)
        {
            temp = heap1;
            heap1 = heap1->sibling;
        }
        else
        {
            temp = heap2;
            heap2 = heap2->sibling;
        }

        if (mergedHead == nullptr)
        {
            mergedHead = temp;
            tail = temp;
        }
        else
        {
            tail->sibling = temp;
            tail = temp;
        }
    }

    if (heap1 != nullptr)
        tail->sibling = heap1;
    else
        tail->sibling = heap2;

    return mergedHead;
}


// Combines two binomial trees of the same degree.
// The smaller root stays as parent.
binomialNode* BinomialHeap::mergeTrees(binomialNode* tree1, binomialNode* tree2)
{
    if (tree1->key > tree2->key)
        swap(tree1, tree2);

    tree2->parent = tree1;
    tree2->sibling = tree1->child;
    tree1->child = tree2;
    tree1->degree++;

    return tree1;
}


// Helper used while cleaning up the root list after union.
// It merges current and next when they have the same degree.
void BinomialHeap::linkTrees(binomialNode*& prev, binomialNode*& cur, binomialNode*& next)
{
    cur = mergeTrees(cur, next);
    next = cur->sibling;

    if (prev == nullptr)
        head = cur;
    else
        prev->sibling = cur;
}


// Searches for a key anywhere in the heap.
binomialNode* BinomialHeap::findNode(binomialNode* root, int key)
{
    if (root == nullptr)
        return nullptr;

    if (root->key == key)
        return root;

    binomialNode* foundInChild = findNode(root->child, key);
    if (foundInChild != nullptr)
        return foundInChild;

    return findNode(root->sibling, key);
}


// Reverses the linked list of children so they can become a valid heap root list.
// Also clearing parent pointers because these nodes become roots.
binomialNode* BinomialHeap::reverseChildren(binomialNode* node)
{
    binomialNode* prev = nullptr;
    binomialNode* current = node;

    while (current != nullptr)
    {
        binomialNode* next = current->sibling;
        current->sibling = prev;
        current->parent = nullptr;
        prev = current;
        current = next;
    }

    return prev;
}

// ---------------- PUBLIC FUNCTIONS ------------------------------
BinomialHeap::BinomialHeap()
{
    head = nullptr;
}


// Insert is done by creating a one-node heap and merging it.
void BinomialHeap::insert(int key)
{
    BinomialHeap tempHeap;
    tempHeap.head = new binomialNode(key);
    merge(tempHeap);
}


// Merge another heap into the current heap.
void BinomialHeap::merge(BinomialHeap& other)
{
    head = unionHeap(head, other.head);

    if (head == nullptr)
    {
        other.head = nullptr;
        return;
    }

    binomialNode* prev = nullptr;
    binomialNode* cur = head;
    binomialNode* next = cur->sibling;

    while (next != nullptr)
    {
        // Case 1: degrees are different
        // Case 2: three consecutive roots with same degree -> move ahead first
        if ((cur->degree != next->degree) ||
            (next->sibling != nullptr && next->sibling->degree == cur->degree))
        {
            prev = cur;
            cur = next;
        }
        else
        {
            // Same degree, merge them
            if (cur->key <= next->key)
            {
                cur->sibling = next->sibling;
                cur = mergeTrees(cur, next);

                if (prev == nullptr)
                    head = cur;
                else
                    prev->sibling = cur;
            }
            else
            {
                if (prev == nullptr)
                    head = next;
                else
                    prev->sibling = next;

                next = mergeTrees(next, cur);
                cur = next;
            }
        }

        next = cur->sibling;
    }

    other.head = nullptr;
}


// Returns the minimum key among the roots.
int BinomialHeap::findMin()
{
    if (head == nullptr)
        throw runtime_error("Heap is empty. Cannot find minimum.");

    int minValue = head->key;
    binomialNode* current = head->sibling;

    while (current != nullptr)
    {
        if (current->key < minValue)
            minValue = current->key;
        current = current->sibling;
    }

    return minValue;
}


// Removes the root with smallest key.
void BinomialHeap::deleteMin()
{
    if (head == nullptr)
        throw runtime_error("Heap is empty. Cannot delete minimum.");

    binomialNode* minNode = head;
    binomialNode* minPrev = nullptr;

    binomialNode* prev = nullptr;
    binomialNode* current = head;

    while (current != nullptr)
    {
        if (current->key < minNode->key)
        {
            minNode = current;
            minPrev = prev;
        }

        prev = current;
        current = current->sibling;
    }

    // Remove min root from root list
    if (minPrev == nullptr)
        head = minNode->sibling;
    else
        minPrev->sibling = minNode->sibling;

    // Reverse its children and merge back
    binomialNode* childHeapHead = reverseChildren(minNode->child);

    BinomialHeap tempHeap;
    tempHeap.head = childHeapHead;

    delete minNode;
    merge(tempHeap);
}


// Decreases a key and bubbles it upward to maintain heap order.
void BinomialHeap::decreaseKey(int oldKey, int newKey)
{
    if (head == nullptr)
        throw runtime_error("Heap is empty. Cannot decrease key.");

    if (newKey > oldKey)
        throw invalid_argument("New key must be smaller than or equal to old key.");

    binomialNode* target = findNode(head, oldKey);

    if (target == nullptr)
        throw runtime_error("Key not found in heap.");

    target->key = newKey;

    binomialNode* current = target;
    binomialNode* parentNode = current->parent;

    while (parentNode != nullptr && current->key < parentNode->key)
    {
        swap(current->key, parentNode->key);
        current = parentNode;
        parentNode = current->parent;
    }
}


// Deletes a key by decreasing it to negative infinity and then deleting min.
void BinomialHeap::deleteKey(int key)
{
    if (head == nullptr)
        throw runtime_error("Heap is empty. Cannot delete key.");

    decreaseKey(key, numeric_limits<int>::min());
    deleteMin();
}


void BinomialHeap::printHeap() {
    cout << "Binomial Heap:\n";
    binomialNode* current = head;

    while (current != nullptr) {
        cout << endl << "Tree of degree " << current->degree << endl;
        printTree(current, 0);
        current = current->sibling;
    }
}

void BinomialHeap::printTree(binomialNode* node, int space)
{
    if (node == nullptr)
        return;

    cout << setw(space * 2) << node->key << endl;

    binomialNode* child = node->child;
    while (child) {
        printTree(child, space + 3);
        child = child->sibling;
    }
}