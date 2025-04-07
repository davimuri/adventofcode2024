#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include <deque>
#include "utils.cpp"

using namespace std;

vector<string> getTowels(string line) {
    vector<string> tokens = tokenize(" " + line, ',');
    vector<string> towels;
    for (string t : tokens) {
        towels.push_back(t.substr(1, t.length()-1));
    }
    return towels;
}

int matchTowelInPattern(string pattern, string towel, int idx) {
    if (towel.length() > pattern.length() - idx) {
        return -1;
    }
    for (int i = idx, j = 0; i < pattern.length() && j < towel.length(); i++, j++) {
        if (pattern[i] != towel[j]) {
            return -1;
        }
    }
    return idx + towel.length();
}

bool isPatternPossibleRecursive(string pattern, vector<string> towels, set<string> *memo) {
    if (pattern.length() == 0) {
        return true;
    }
    if (memo->count(pattern) > 0) {
        return true;
    }
    for (string t : towels) {
        int nextIdx = matchTowelInPattern(pattern, t, 0);
        if (nextIdx != -1) {
            string subpattern = pattern.substr(nextIdx, pattern.length() - nextIdx);
            bool found = isPatternPossibleRecursive(subpattern, towels, memo);
            if (found) {
                memo->insert(subpattern);
                return true;
            }
        }
    }
    return false;
}

int solvePart1(vector<string> towels, vector<string> lines) {
    int count = 0;
    set<string> memo;
    for (int i = 2; i < lines.size(); i++) {
        if (isPatternPossibleRecursive(lines[i], towels, &memo)) {
            count++;
        }
    }

    return count;
}

long isPatternPossibleRecursivePart2(string pattern, vector<string> towels, map<string, long> *memo) {
    if (pattern.length() == 0) {
        return 1;
    }
    if (memo->find(pattern) != memo->end()) {
        return memo->at(pattern);
    }
    long solutions = 0;
    for (string t : towels) {
        int nextIdx = matchTowelInPattern(pattern, t, 0);
        if (nextIdx != -1) {
            string subpattern = pattern.substr(nextIdx, pattern.length() - nextIdx);
            solutions += isPatternPossibleRecursivePart2(subpattern, towels, memo);
        }
    }
    memo->insert({pattern, solutions});
    return solutions;
}

long solvePart2(vector<string> towels, vector<string> lines) {
    long count = 0;
    map<string, long> memo;
    for (int i = 2; i < lines.size(); i++) {
        count += isPatternPossibleRecursivePart2(lines[i], towels, &memo);
    }
    return count;
}

int main() {
    vector<string> lines = readLines("data/input19.txt");
    vector<string> towels = getTowels(lines[0]);
    auto t1 = chrono::high_resolution_clock::now();
    int count = solvePart1(towels, lines);
    auto t2 = chrono::high_resolution_clock::now();
    auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 1 - count: " << count << " Duration (ms): " << ms_int.count() << endl;

    t1 = chrono::high_resolution_clock::now();
    long solutionPart2 = solvePart2(towels, lines);
    t2 = chrono::high_resolution_clock::now();
    ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 2 - solution: " << solutionPart2 << " Duration (ms): " << ms_int.count() << endl;
    return 0;
}
