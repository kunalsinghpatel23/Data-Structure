#include "hash.h"
#include <ctime>

string methodName(CollisionHandling method) {
    switch (method) {
        case CHAINING_VECTOR: return "Chaining with Vector";
        case CHAINING_LIST: return "Chaining with List";
        case CHAINING_BST: return "Chaining with AVL BST";
        case LINEAR_PROBING: return "Linear Probing";
        case QUADRATIC_PROBING: return "Quadratic Probing";
        case DOUBLE_HASHING: return "Double Hashing";
        default: return "Unknown";
    }
}

int main() {
    srand(time(nullptr));

    vector<string> dataFiles = {
        "data/no_collision_data.txt",
        "data/low_collision_data.txt",
        "data/high_collision_data.txt"
    };

    CollisionHandling methods[] = {
        CHAINING_VECTOR,
        CHAINING_LIST,
        CHAINING_BST,
        LINEAR_PROBING,
        QUADRATIC_PROBING,
        DOUBLE_HASHING
    };

    for (const auto& file : dataFiles) {
        cout << "\n========================================" << endl;
        cout << "Benchmarking with data from: " << file << endl;
        cout << "========================================" << endl;

        auto data = readDataFromFile(file);
        int numSearch = data.size() / 2;
        int numDelete = 0;   // change this if you want delete benchmark too

        for (auto method : methods) {
            cout << "\n----------------------------------------" << endl;
            cout << "Running benchmark for method: " << methodName(method) << endl;
            cout << "----------------------------------------" << endl;

            HashTable table(100003, method);
            table.benchmarkHashTable(table, data, numSearch, numDelete);

            cout << "\nStatistics for method: " << methodName(method) << endl;
            table.displayStats();
        }

        cout << "\n----------------------------------------" << endl;
        cout << "Running benchmark for std::unordered_map" << endl;
        cout << "----------------------------------------" << endl;

        benchmarkStdUnorderedMap(data, numSearch, numDelete);
    }

    return 0;
}