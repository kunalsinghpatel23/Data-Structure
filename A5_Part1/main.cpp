#include "binomial.h"

int main()
{
    BinomialHeap bh;

    try
    {
        bh.insert(10);
        bh.insert(20);
        bh.insert(5);
        bh.insert(30);
        bh.insert(1);
        bh.insert(14);
        bh.insert(27);
        bh.insert(8);
        bh.insert(3);
        bh.insert(12);

        cout << "Heap after insertions:\n";
        bh.printHeap();

        cout << "\nMinimum key: " << bh.findMin() << endl;

        cout << "\nDeleting minimum...\n";
        bh.deleteMin();
        bh.printHeap();

        cout << "\nDecreasing key 27 to 2...\n";
        bh.decreaseKey(27, 2);
        bh.printHeap();

        cout << "\nDeleting key 14...\n";
        bh.deleteKey(14);
        bh.printHeap();

        cout << "\nCurrent minimum key: " << bh.findMin() << endl;

        // we can Uncomment this to test exceptions
        // bh.decreaseKey(100, 2);
        // bh.decreaseKey(5, 50);
        // bh.deleteKey(999);
    }
    catch (const exception& e)
    {
        cout << "Exception: " << e.what() << endl;
    }

    return 0;
}