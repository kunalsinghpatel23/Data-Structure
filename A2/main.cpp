#include "dsw.h"

int main()
{
    BST bst;

    try
    {
        bst.insert(2);
        bst.insert(3);
        bst.insert(19);
        bst.insert(27);
        bst.insert(20);
        bst.insert(36);
        bst.insert(30);
        bst.insert(59);
        bst.insert(43);
        bst.insert(40);

        cout << "Original BST:";
        bst.display();

        bst.dswBalance();

        cout << "DSW balanced BST:";
        bst.display();
    }
    catch (const exception& e)
    {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}