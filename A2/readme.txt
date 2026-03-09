Student: Kunal Singh Patel

Project Description

This project implements a variation of the Day-Stout-Warren (DSW) algorithm used to rebalance a Binary Search Tree (BST). The goal of the program is to improve the structure of an unbalanced BST while keeping the space complexity constant (O(1)).

The program builds a BST using the provided insert function and then applies a modified version of the DSW balancing algorithm. After balancing, the tree structure is displayed so the difference between the original and balanced tree can be observed.


Files Included

main.cpp
Contains the main function. It creates a BST, inserts values, calls the DSW balancing function, and prints the tree before and after balancing.

dsw.h
Header file containing the Node structure and BST class declarations.

dsw.cpp
Contains the implementation of all BST operations including rotations, vine creation, and the modified DSW balancing algorithm.

Makefile
Automates the compilation and running of the program using the make command.

README.txt
Provides information about the project, files, and instructions for running the program.

Task5.pdf
Contains written answers explaining the algorithm design choices and differences from the traditional DSW algorithm.

screenshot.png
Screenshot showing the program running and displaying both the original BST and the balanced BST.


Program Features

• Binary Search Tree insertion
• Tree visualization using a sideways tree display
• Modified DSW balancing algorithm
• Exception handling for invalid operations
• Constant space complexity (O(1)) for balancing
• Automated compilation using a Makefile


Modified DSW Implementation

Phase 1 – Partial Vine Creation
The algorithm creates a partially left-skewed vine by rotating left whenever a node has a right child. However, rotations are skipped when the right subtree contains two or fewer nodes. This avoids unnecessary restructuring of very small subtrees.

Phase 2 – Tree Rebalancing
The number of nodes in the vine is calculated and several right rotations are performed to reduce the height of the tree. Instead of enforcing perfect balancing, the algorithm only attempts to keep the height close to 2 log2(N).


Exception Handling

The program includes exception handling for the following cases:

• Inserting duplicate values into the BST
• Attempting to balance an empty tree
• Attempting to display an empty tree


Output

The program prints two trees:

1. The original unbalanced BST
2. The BST after applying the modified DSW balancing algorithm

This allows the structural improvement of the tree to be observed visually.
