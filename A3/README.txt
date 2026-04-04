This project implements a Left-Leaning Red-Black (LLRB) Tree in C++. The purpose of this project is to understand how balanced trees work and how LLRB trees maintain balance using rotations and color properties.

Features
Insert operation for LLRB tree
Inorder traversal for displaying tree
Basic exception handling (duplicate insert, empty tree)
How it works

The LLRB tree keeps itself balanced using:

Left rotation: fixes right-leaning red links
Right rotation: fixes consecutive red links
Color flip: balances the tree

These rules ensure that all red links lean to the left and the tree remains balanced.