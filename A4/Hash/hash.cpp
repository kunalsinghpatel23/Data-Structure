#include "hash.h"

// Constructor
HashTable::HashTable(int size, CollisionHandling variant) {
    tableSize = size;
    elementCount = 0;
    method = variant;

    switch (method) {
        case CHAINING_VECTOR:
            tableVector.resize(tableSize);
            break;
        case CHAINING_LIST:
            tableList.resize(tableSize);
            break;
        case CHAINING_BST:
            tableBST.resize(tableSize);
            break;
        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING:
            tableProbing.resize(tableSize, {"", 0});
            break;
    }
}

HashTable::~HashTable() {}

int HashTable::hash1(const string& key) const {
    unsigned long hash = 0;
    for (char c : key) {
        hash = (hash * 31 + c);
    }
    return hash % tableSize;
}

int HashTable::hash2(const string& key) const {
    unsigned long hash = 5381;
    for (char c : key) {
        hash = ((hash << 5) + hash) + c;
    }
    return 1 + (hash % (tableSize - 1));
}

// Collision probing function
int HashTable::probe(int index, int i) const {
    switch (method) {
        case LINEAR_PROBING:
            return (index + i) % tableSize;

        case QUADRATIC_PROBING:
            return (index + i * i) % tableSize;

        case DOUBLE_HASHING:
            return (index + i * hash2(to_string(index))) % tableSize;

        default:
            return index;
    }
}

void HashTable::insert(const string& key, int value) {
    resizeIfNeeded();
    int index = hash1(key);

    switch (method) {
        case CHAINING_VECTOR: {
            for (auto& p : tableVector[index]) {
                if (p.first == key) {
                    p.second = value;
                    return;
                }
            }
            tableVector[index].push_back({key, value});
            elementCount++;
            break;
        }

        case CHAINING_LIST: {
            for (auto& p : tableList[index]) {
                if (p.first == key) {
                    p.second = value;
                    return;
                }
            }
            tableList[index].push_back({key, value});
            elementCount++;
            break;
        }

        case CHAINING_BST: {
            int dummy;
            if (!tableBST[index].search(key, dummy)) {
                tableBST[index].insert(key, value);
                elementCount++;
            } else {
                tableBST[index].insert(key, value); // update existing
            }
            break;
        }

        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING: {
            int baseIndex = hash1(key);

            // For double hashing, use the real second hash from the key
            for (int i = 0; i < tableSize; i++) {
                int pos;
                if (method == DOUBLE_HASHING) {
                    pos = (baseIndex + i * hash2(key)) % tableSize;
                } else {
                    pos = probe(baseIndex, i);
                }

                // Empty slot -> insert
                if (tableProbing[pos].first.empty()) {
                    tableProbing[pos] = {key, value};
                    elementCount++;
                    return;
                }

                // Key already exists -> update
                if (tableProbing[pos].first == key) {
                    tableProbing[pos].second = value;
                    return;
                }
            }
            break;
        }
    }
}

bool HashTable::search(const string& key, int& value) {
    int index = hash1(key);

    switch (method) {
        case CHAINING_VECTOR: {
            for (const auto& p : tableVector[index]) {
                if (p.first == key) {
                    value = p.second;
                    return true;
                }
            }
            return false;
        }

        case CHAINING_LIST: {
            for (const auto& p : tableList[index]) {
                if (p.first == key) {
                    value = p.second;
                    return true;
                }
            }
            return false;
        }

        case CHAINING_BST: {
            return tableBST[index].search(key, value);
        }

        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING: {
            int baseIndex = hash1(key);

            for (int i = 0; i < tableSize; i++) {
                int pos;
                if (method == DOUBLE_HASHING) {
                    pos = (baseIndex + i * hash2(key)) % tableSize;
                } else {
                    pos = probe(baseIndex, i);
                }

                // here Empty slot means key not found
                if (tableProbing[pos].first.empty()) {
                    return false;
                }

                if (tableProbing[pos].first == key) {
                    value = tableProbing[pos].second;
                    return true;
                }
            }
            return false;
        }
    }

    return false;
}

bool HashTable::remove(const string& key) {
    int index = hash1(key);

    switch (method) {
        case CHAINING_VECTOR: {
            auto& bucket = tableVector[index];
            for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                if (it->first == key) {
                    bucket.erase(it);
                    elementCount--;
                    return true;
                }
            }
            return false;
        }

        case CHAINING_LIST: {
            auto& bucket = tableList[index];
            for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                if (it->first == key) {
                    bucket.erase(it);
                    elementCount--;
                    return true;
                }
            }
            return false;
        }

        case CHAINING_BST: {
            if (tableBST[index].remove(key)) {
                elementCount--;
                return true;
            }
            return false;
        }

        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING: {
            int baseIndex = hash1(key);

            for (int i = 0; i < tableSize; i++) {
                int pos;
                if (method == DOUBLE_HASHING) {
                    pos = (baseIndex + i * hash2(key)) % tableSize;
                } else {
                    pos = probe(baseIndex, i);
                }

                if (tableProbing[pos].first.empty()) {
                    return false;
                }

                if (tableProbing[pos].first == key) {
                    // Remove this entry
                    tableProbing[pos] = {"", 0};
                    elementCount--;

                    // Reinsert following cluster so search still works correctly
                    int next = (pos + 1) % tableSize;
                    while (!tableProbing[next].first.empty()) {
                        pair<string, int> temp = tableProbing[next];
                        tableProbing[next] = {"", 0};
                        elementCount--;
                        insert(temp.first, temp.second);
                        next = (next + 1) % tableSize;
                    }

                    return true;
                }
            }
            return false;
        }
    }

    return false;
}

// Read data from file
vector<pair<string, int>> readDataFromFile(const string& filename) {
    vector<pair<string, int>> data;
    ifstream file(filename);
    string key;
    int value;

    if (!file) {
        cerr << "Error reading file: " << filename << endl;
        return data;
    }

    while (file >> key >> value) {
        data.push_back({key, value});
    }

    file.close();
    return data;
}

// Benchmark custom hash table
void HashTable::benchmarkHashTable(HashTable& table, const vector<pair<string, int>>& data, int numSearch, int numDelete) {
    auto start = chrono::high_resolution_clock::now();

    // Insert
    for (const auto& p : data) {
        table.insert(p.first, p.second);
    }

    auto insertEnd = chrono::high_resolution_clock::now();

    // Search
    int value;
    for (int i = 0; i < numSearch; ++i) {
        table.search(data[rand() % data.size()].first, value);
    }

    auto searchEnd = chrono::high_resolution_clock::now();

    // Delete
    for (int i = 0; i < numDelete; ++i) {
        table.remove(data[rand() % data.size()].first);
    }

    auto end = chrono::high_resolution_clock::now();

    auto insertTime = chrono::duration_cast<chrono::microseconds>(insertEnd - start);
    auto searchTime = chrono::duration_cast<chrono::microseconds>(searchEnd - insertEnd);
    auto deleteTime = chrono::duration_cast<chrono::microseconds>(end - searchEnd);

    cout << "Insertion time: " << insertTime.count() << " microseconds" << endl;
    cout << "Search time: " << searchTime.count() << " microseconds" << endl;
    cout << "Deletion time: " << deleteTime.count() << " microseconds" << endl;
}

void HashTable::displayStats() {
    int totalElements = 0;
    int maxChainLength = 0;
    int emptyBuckets = 0;

    switch (method) {
        case CHAINING_VECTOR:
            for (const auto& bucket : tableVector) {
                if (bucket.empty()) emptyBuckets++;
                maxChainLength = max(maxChainLength, (int)bucket.size());
                totalElements += bucket.size();
            }
            break;

        case CHAINING_LIST:
            for (const auto& bucket : tableList) {
                if (bucket.empty()) emptyBuckets++;
                maxChainLength = max(maxChainLength, (int)bucket.size());
                totalElements += bucket.size();
            }
            break;

        case CHAINING_BST:
            for (const auto& bucket : tableBST) {
                vector<pair<string, int>> elements = bucket.inOrderTraversal();
                if (elements.empty()) {
                    emptyBuckets++;
                } else {
                    maxChainLength = max(maxChainLength, (int)elements.size());
                    totalElements += elements.size();
                }
            }
            break;

        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING:
            for (const auto& entry : tableProbing) {
                if (!entry.first.empty()) totalElements++;
                else emptyBuckets++;
            }
            break;
    }

    cout << "Total elements: " << totalElements << endl;
    cout << "Load factor: " << (double)totalElements / tableSize << endl;
    cout << "Empty buckets: " << emptyBuckets << endl;

    if (method == CHAINING_VECTOR || method == CHAINING_LIST || method == CHAINING_BST) {
        cout << "Max chain length: " << maxChainLength << endl;
    }
}

// Rebuild table with larger size
void HashTable::rehash() {
    vector<pair<string, int>> allData;

    // Collect all existing elements
    switch (method) {
        case CHAINING_VECTOR:
            for (const auto& bucket : tableVector) {
                for (const auto& p : bucket) {
                    allData.push_back(p);
                }
            }
            break;

        case CHAINING_LIST:
            for (const auto& bucket : tableList) {
                for (const auto& p : bucket) {
                    allData.push_back(p);
                }
            }
            break;

        case CHAINING_BST:
            for (const auto& tree : tableBST) {
                vector<pair<string, int>> elements = tree.inOrderTraversal();
                for (const auto& p : elements) {
                    allData.push_back(p);
                }
            }
            break;

        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING:
            for (const auto& p : tableProbing) {
                if (!p.first.empty()) {
                    allData.push_back(p);
                }
            }
            break;
    }

    // Increase size
    tableSize = tableSize * 2 + 1;
    elementCount = 0;

    // Clear old structure and recreate
    tableVector.clear();
    tableList.clear();
    tableBST.clear();
    tableProbing.clear();

    switch (method) {
        case CHAINING_VECTOR:
            tableVector.resize(tableSize);
            break;
        case CHAINING_LIST:
            tableList.resize(tableSize);
            break;
        case CHAINING_BST:
            tableBST.resize(tableSize);
            break;
        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING:
            tableProbing.resize(tableSize, {"", 0});
            break;
    }

    // Reinsert all data
    for (const auto& p : allData) {
        insert(p.first, p.second);
    }
}

// Find empty slot for probing methods
int HashTable::findEmptySlot(const string& key) {
    int baseIndex = hash1(key);

    for (int i = 0; i < tableSize; i++) {
        int pos;
        if (method == DOUBLE_HASHING) {
            pos = (baseIndex + i * hash2(key)) % tableSize;
        } else {
            pos = probe(baseIndex, i);
        }

        if (tableProbing[pos].first.empty() || tableProbing[pos].first == key) {
            return pos;
        }
    }

    return -1;
}

// Resize when load factor becomes too high
void HashTable::resizeIfNeeded() {
    double loadFactor = (double)elementCount / tableSize;

    switch (method) {
        case CHAINING_VECTOR:
        case CHAINING_LIST:
        case CHAINING_BST:
            // Chaining can handle larger load factor
            if (loadFactor > 1.0) {
                rehash();
            }
            break;

        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING:
            // Probing degrades faster, so resize earlier
            if (loadFactor > 0.6) {
                rehash();
            }
            break;
    }
}

// Benchmark std::unordered_map
void benchmarkStdUnorderedMap(const vector<pair<string, int>>& data, int numSearch, int numDelete) {
    unordered_map<string, int> stdMap;

    auto start = chrono::high_resolution_clock::now();

    // Insert
    for (const auto& p : data) {
        stdMap[p.first] = p.second;
    }

    auto insertEnd = chrono::high_resolution_clock::now();

    // Search
    int value;
    for (int i = 0; i < numSearch; ++i) {
        auto it = stdMap.find(data[rand() % data.size()].first);
        if (it != stdMap.end()) {
            value = it->second;
        }
    }

    auto searchEnd = chrono::high_resolution_clock::now();

    // Delete
    for (int i = 0; i < numDelete; ++i) {
        stdMap.erase(data[rand() % data.size()].first);
    }

    auto end = chrono::high_resolution_clock::now();

    auto insertTime = chrono::duration_cast<chrono::microseconds>(insertEnd - start);
    auto searchTime = chrono::duration_cast<chrono::microseconds>(searchEnd - insertEnd);
    auto deleteTime = chrono::duration_cast<chrono::microseconds>(end - searchEnd);

    cout << "std::unordered_map Benchmark Results:" << endl;
    cout << "Insertion time: " << insertTime.count() << " microseconds" << endl;
    cout << "Search time: " << searchTime.count() << " microseconds" << endl;
    cout << "Deletion time: " << deleteTime.count() << " microseconds" << endl;
}