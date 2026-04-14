Hash Table Benchmark

---

Description:

In this project, I implemented different types of hash tables and compared how they perform. The main idea is to see how different collision handling methods behave when there are no collisions, few collisions, and many collisions.

The methods I used are:

* Chaining using vector
* Chaining using list
* Chaining using AVL tree
* Linear probing
* Quadratic probing
* Double hashing

---

Data Files:

There are 3 datasets used in this program:

* no_collision_data.txt
* low_collision_data.txt
* high_collision_data.txt

These datasets help test the hash tables under different collision conditions.

---

How to Run:

First compile the program using:

g++ mainhash.cpp hash.cpp AVL/avl.cpp -o hashbench

Then run it using:

./hashbench

(For Windows, use: .\hashbench.exe)

---

What the Program Does:

For each dataset and each method:

* Inserts all the data into the hash table
* Performs random search operations
* (Delete is included but not heavily tested)

After that, it prints:

* Time taken for insertion
* Time taken for searching
* Time taken for deletion
* Total elements
* Load factor
* Number of empty buckets
* Max chain length (for chaining)

---

Observations:

From running the program, I noticed a few things:

* Chaining methods worked better when there were a lot of collisions.
* Vector and list chaining were simple and worked well, but if too many elements go into one bucket, it can slow down.
* AVL tree chaining helped keep things balanced, so searching was better in heavy collision cases, but it added extra work during insert.
* Linear probing was fast when there were no collisions, but got slower when collisions increased because of clustering.
* Quadratic probing was slightly better than linear probing but still had some issues when the table got full.
* Double hashing performed the best among probing methods because it spreads keys more evenly.
* Probing methods use less memory, while chaining methods use extra memory for lists or trees.

---

Notes:

* The table resizes automatically when it gets too full.
* In probing methods, elements are reinserted after deletion so that searching still works correctly.

---

Author:
Kunal Singh Patel

---
