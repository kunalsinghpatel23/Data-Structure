Splay Tree Variants Implementation (C++)
 
Overview
This project implements different variants of the Splay Tree data structure in C++. A Splay Tree is a self-adjusting Binary Search Tree (BST) where recently accessed elements are moved closer to the root using rotations, improving performance for frequently accessed elements.

The following variants are implemented:
• Bottom-Up Splay
• Top-Down Splay
• Semi-Splay (Top-Down with rotation limit)
• Weighted Top-Down Splay
 
Files Included
1. splay.h – Contains class definition, Node structure, mode enumeration, and declarations.
2. splay.cpp – Contains implementation of splay variants, rotations, insert, delete, search, and display.
3. mainsplay.cpp – Driver file to test the tree operations.
4. README.docx – Project documentation.
 
Splay Variants Implemented
Bottom-Up Splay:
- Recursive implementation
- Uses Zig-Zig, Zig-Zag, Zag-Zag, Zag-Zig rotations

Top-Down Splay:
- Iterative implementation
- Uses split and reassemble technique

Semi-Splay:
- Limits number of rotations to k
- Stops early after k rotations

Weighted Top-Down Splay:
- Each node maintains a frequency (weight)
- Rotation occurs only if child.weight >= parent.weight
- Weight increases after successful search
 
Supported Operations
Insert – Inserts a key and splays it.
Search – Splays the key to root and returns true if found.
Delete – Splays the key, removes it, and reassembles the tree.
Display – Prints the tree sideways (shows weight in weighted mode).
  
Design Notes
- Implemented as a Binary Search Tree.
- Splay operation automatically called during insert, delete, and search.
- Mode wrapper selects appropriate splay variant.
- Semi-splay limits rotations.
- Weighted mode updates frequency after successful searches.
 
