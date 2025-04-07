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

unsigned long long solvePart1(map<string, int> wires, vector<vector<string>> operations) {
    int zOutputs = 0;
    for (auto op : operations) {
        string out = op[4];
        if (out[0] == 'z') {
            zOutputs++;
        }
    }
    int zOutputsDone = 0;
    unsigned long long result = 0;
    while (zOutputsDone < zOutputs) {
        for (auto op : operations) {
            string in1 = op[0];
            string in2 = op[2];
            string out = op[4];
            if (wires.count(out) == 0 && wires.count(in1) > 0 && wires.count(in2) > 0) {
                string gate = op[1];
                int outValue = 0;
                if (gate == "AND") {
                    outValue = wires[in1] & wires[in2];
                } else if (gate == "OR") {
                    outValue = wires[in1] | wires[in2];
                } else {
                    outValue = wires[in1] ^ wires[in2];
                }
                wires[out] = outValue;
                if (out[0] == 'z') {
                    zOutputsDone++;
                    // z001
                    int pos = stoi(out.substr(1, out.length()-1));
                    if (outValue == 1) {
                        unsigned long long mask = ((unsigned long long)1) << pos;
                        result = result | mask;
                    }
                }
            }
        }
    }
    return result;
}

string solvePart2(map<string, int> wires, vector<vector<string>> operations) {
    unsigned long long x = 0;
    unsigned long long y = 0;
    int maxPosX = 0;
    int maxPosY = 0;
    for (const auto &entry : wires) {
        int pos = stoi(entry.first.substr(1, entry.first.length()-1));
        unsigned long long mask = 1;
        if (entry.first[0] == 'x') {
            if (entry.second == 1) {
                mask = mask << pos;
                x = x | mask;
            } else {
                mask = ~(mask << pos);
                x = x & mask;
            }
            maxPosX = max(pos, maxPosX);
        } else if (entry.first[0] == 'y') {
            if (entry.second == 1) {
                mask = mask << pos;
                y = y | mask;
            } else {
                mask = ~(mask << pos);
                y = y & mask;
            }
            maxPosY = max(pos, maxPosY);
        }
    }

    cout << "Testing sums with x inputs: " << endl;
    unsigned long long expectedSum = 1;
    for (int xIdx = 0; xIdx <= maxPosX; xIdx++) {
        map<string, int> inputs;
        for (int i = 0; i <= maxPosX; i++) {
            string wireName = i < 10 ? "x0" + to_string(i) : "x" + to_string(i);
            inputs[wireName] = 0;
        }
        string wireName = xIdx < 10 ? "x0" + to_string(xIdx) : "x" + to_string(xIdx);
        inputs[wireName] = 1;
        for (int i = 0; i <= maxPosY; i++) {
            string wireName = i < 10 ? "y0" + to_string(i) : "y" + to_string(i);
            inputs[wireName] = 0;
        }
        unsigned long long actualSum = solvePart1(inputs, operations);
        if (expectedSum != actualSum) {
            // cout << "x: " << x << " y: " << y << " x+y: " << x+y << " maxPosX: " << maxPosX << " maxPosY: " << maxPosY << endl;
            unsigned long long diff = actualSum ^ expectedSum;
            cout << "expected sum " << expectedSum << " actual sum " << actualSum << endl;
            for (int i = 0; i <= maxPosX; i++) {
                unsigned long long mask = 1;
                mask = mask << i;
                if ((diff & mask) != 0) {
                    cout << "z" << i << endl;
                }
            }
        }
        expectedSum *= 2;
    }
    expectedSum = 1;
    cout << "Testing sums with y inputs: " << endl;
    for (int xIdx = 0; xIdx <= maxPosX; xIdx++) {
        map<string, int> inputs;
        for (int i = 0; i <= maxPosX; i++) {
            string wireName = i < 10 ? "x0" + to_string(i) : "x" + to_string(i);
            inputs[wireName] = 0;
        }
        for (int i = 0; i <= maxPosY; i++) {
            string wireName = i < 10 ? "y0" + to_string(i) : "y" + to_string(i);
            inputs[wireName] = 0;
        }
        string wireName = xIdx < 10 ? "y0" + to_string(xIdx) : "y" + to_string(xIdx);
        inputs[wireName] = 1;

        unsigned long long actualSum = solvePart1(inputs, operations);
        if (expectedSum != actualSum) {
            // cout << "x: " << x << " y: " << y << " x+y: " << x+y << " maxPosX: " << maxPosX << " maxPosY: " << maxPosY << endl;
            unsigned long long diff = actualSum ^ expectedSum;
            cout << "expected sum " << expectedSum << " actual sum " << actualSum << endl;
            for (int i = 0; i <= maxPosX; i++) {
                unsigned long long mask = 1;
                mask = mask << i;
                if ((diff & mask) != 0) {
                    cout << "z" << i << endl;
                }
            }
        }
        expectedSum *= 2;
    }

    return "";
}

int main() {
    vector<string> lines = readLines("data/input24.txt");
    map<string, int> wires;
    vector<vector<string>> operations;
    bool parsingWires = true;
    for (string l : lines) {
        if (l == "") {
            parsingWires = false;
            continue;
        }
        if (parsingWires) {
            int idx = l.find(":");
            wires[l.substr(0, idx)] = stoi(l.substr(idx+2, l.length() - idx+2));
        } else {
            auto tokens = tokenize(l, ' ');
            operations.push_back(tokens);
        }
    }

    auto t1 = chrono::high_resolution_clock::now();
    auto output = solvePart1(wires, operations);
    auto t2 = chrono::high_resolution_clock::now();
    auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 1 - output: " << output << " Duration (ms): " << ms_int.count() << endl;
    t1 = chrono::high_resolution_clock::now();
    string solution = solvePart2(wires, operations);
    t2 = chrono::high_resolution_clock::now();
    ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 2 - solution: " << solution << " Duration (ms): " << ms_int.count() << endl;

    return 0;
}
