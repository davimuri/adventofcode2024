#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include <deque>
#include <tuple>
#include "utils.cpp"

using namespace std;

int solvePart1(vector<vector<int>> locks, vector<vector<int>> keys) {
    int count = 0;
    for (auto lock : locks) {
        for (auto key : keys) {
            bool fit = true;
            for (int i = 0; i < key.size() && fit; i++) {
                if (lock[i] + key[i] >= 6) {
                    fit = false;
                }
            }
            if (fit) {
                // cout << "fit lock " << lock[0] << " key " << key[0] << endl;
                count++;
            }
        }
    }
    return count;
}

int main() {
    vector<string> lines = readLines("data/input25.txt");
    vector<vector<int>> locks;
    vector<vector<int>> keys;
    bool processingLock = true;
    bool newElement = true;
    vector<int> current;
    for (string l : lines) {
        if (l == "") {
            if (processingLock) {
                locks.push_back(current);
            } else {
                keys.push_back(current);
            }
            current = vector<int>();
            newElement = true;
        } else if (newElement) {
            processingLock = l[0] == '.' ? false : true;
            int value = processingLock ? 0 : 5;
            for (int i = 0; i < l.length(); i++) {
                current.push_back(value);
            }
            newElement = false;
        } else {
            for (int i = 0; i < l.length(); i++) {
                if (processingLock && l[i] == '#') {
                    current[i]++;
                } else if (!processingLock && l[i] == '.'){
                    current[i]--;
                }
            }
        }
    }
    if (processingLock) {
        locks.push_back(current);
    } else {
        keys.push_back(current);
    }

    // cout << "locks: " << endl;
    // for (auto l : locks) {
    //     for (auto e : l) {
    //         cout << e;
    //     }
    //     cout << endl;
    // }
    // cout << "keys: " << endl;
    // for (auto l : keys) {
    //     for (auto e : l) {
    //         cout << e;
    //     }
    //     cout << endl;
    // }

    auto t1 = chrono::high_resolution_clock::now();
    auto output = solvePart1(locks, keys);
    auto t2 = chrono::high_resolution_clock::now();
    auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 1 - output: " << output << " Duration (ms): " << ms_int.count() << endl;

    return 0;
}
