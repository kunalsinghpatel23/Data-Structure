#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <unordered_map>
#include <fstream>
#include <chrono>
#include "AVL/avl.h"

using namespace std;

enum CollisionHandling {
    CHAINING_VECTOR,
    CHAINING_LIST,
    CHAINING_BST,
    LINEAR_PROBING,
    QUADRATIC_PROBING,
    DOUBLE_HASHING
};

class HashTable {
private:
    int tableSize;
    int elementCount;
    CollisionHandling method;

    // Storage for each variation
    vector<vector<pair<string, int>>> tableVector;
    vector<list<pair<string, int>>> tableList;
    vector<AVLTree> tableBST;
    vector<pair<string, int>> tableProbing;

    // Hash functions
    int hash1(const string& key) const;
    int hash2(const string& key) const;
    int probe(int index, int i) const;
    void rehash();

    // Helpers
    int findEmptySlot(const string& key);
    void resizeIfNeeded();

public:
    HashTable(int size, CollisionHandling variant);
    ~HashTable();

    void insert(const string& key, int value);
    bool search(const string& key, int& value);
    bool remove(const string& key);

    void benchmarkHashTable(HashTable& table, const vector<pair<string, int>>& data, int numSearch, int numDelete);
    void displayStats();
};

vector<pair<string, int>> readDataFromFile(const string& filename);
void benchmarkStdUnorderedMap(const vector<pair<string, int>>& data, int numSearch, int numDelete);

#endif