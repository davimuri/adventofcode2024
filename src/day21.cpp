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

const map<char, pair<int, int> > NUMERIC_KEYPAD = {
    {'7', make_pair(0, 0)}, {'8', make_pair(1, 0)}, {'9', make_pair(2, 0)},
    {'4', make_pair(0, 1)}, {'5', make_pair(1, 1)}, {'6', make_pair(2, 1)},
    {'1', make_pair(0, 2)}, {'2', make_pair(1, 2)}, {'3', make_pair(2, 2)},
                            {'0', make_pair(1, 3)}, {'A', make_pair(2, 3)},
};
char NUMERIC_MATRIX[4][3] = {
    {'7', '8', '9'},
    {'4', '5', '6'},
    {'1', '2', '3'},
    {'#', '0', 'A'},
};

const map<char, pair<int, int> > DIRECTIONAL_KEYPAD = {
                            {'^', make_pair(1, 0)}, {'A', make_pair(2, 0)},
    {'<', make_pair(0, 1)}, {'v', make_pair(1, 1)}, {'>', make_pair(2, 1)},
};

char DIRECTIONAL_MATRIX[2][3] = {
    {'#', '^', 'A'},
    {'<', 'v', '>'}
};

const map<char, pair<int, int> > DIRECTIONS_MAP = {
    {'>', make_pair(1, 0)},
    {'<', make_pair(-1, 0)},
    {'^', make_pair(0, -1)},
    {'v', make_pair(0, 1)},
};

string forwardNumeric(string sequence) {
    // A position in matrix
    int row = 3;
    int col = 2;
    string output = "";
    for (char c: sequence) {
        if (c == 'A') {
            output += NUMERIC_MATRIX[row][col];
        } else {
            auto dir = DIRECTIONS_MAP.at(c);
            row += dir.second;
            col += dir.first;
        }
    }
    return output;
}

string forwardDirectional(string sequence) {
    // A position in matrix
    int row = 0;
    int col = 2;
    string output = "";
    for (char c: sequence) {
        if (c == 'A') {
            output += DIRECTIONAL_MATRIX[row][col];
        } else {
            auto dir = DIRECTIONS_MAP.at(c);
            row += dir.second;
            col += dir.first;
        }
    }
    return output;
}

bool validateSolutionPart1(string targetCode, string sequence) {
    string directionalSeq = sequence;
    for (int i = 0; i < 2; i++) {
        directionalSeq = forwardDirectional(directionalSeq);
        cout << "Directional " << i << ": " << directionalSeq << endl;
    }
    string finalCode = forwardNumeric(directionalSeq);
    cout << "Final code " << finalCode << endl;
    return finalCode == targetCode;
}


struct Node {
    pair<int, int> pos;
    string seq;
    Node(pair<int, int> p, string s) {
        pos = p;
        seq = s;
    }
};

vector<string> getOptimalPaths(pair<int, int> start, pair<int, int> end, char matrix[][3], int rows, int cols) {
    vector<string> paths;
    if (start.first == end.first && start.second == end.second) {
        paths.push_back("A");
        return paths;
    }
    deque<Node> queue;
    queue.push_back(Node(start, ""));
    int minLength = INT_MAX;
    while (queue.size() > 0) {
        Node current = queue.front();
        queue.pop_front();
        if (current.seq.length() > minLength) {
            continue;
        }
        if (current.pos.first == end.first && current.pos.second == end.second) {
            if (current.seq.length() < minLength) {
                minLength = current.seq.length();
                paths.clear();
            }
            paths.push_back(current.seq + "A");
            continue;
        }
        for (const auto &dir : DIRECTIONS_MAP) {
            pair<int, int> nextPos = make_pair(current.pos.first + dir.second.first, current.pos.second + dir.second.second);
            if (nextPos.first < 0 || nextPos.first >= cols || nextPos.second < 0 || nextPos.second >= rows
                    || matrix[nextPos.second][nextPos.first] == '#') {
                continue;
            }
            string nextSeq = current.seq + dir.first;
            queue.push_back(Node(nextPos, nextSeq));
        }
    }
    return paths;
}

map<pair<char, char>, vector<string>> buildPaths(map<char, pair<int, int>> keypad, char matrix[][3], int rows, int cols) {
    map<pair<char, char>, vector<string>> allPaths;
    for (const auto &entry1 : keypad) {
        auto startPos = entry1.second;
        for (const auto &entry2 : keypad) {
            auto endPos = entry2.second;
            pair<char, char> key = make_pair(entry1.first, entry2.first);
            allPaths[key] = getOptimalPaths(startPos, endPos, matrix, rows, cols);
        }
    }
    return allPaths;
}

vector<string> combine(vector<vector<string>> possiblePaths) {
    vector<string> sequences;
    int minLength = INT_MAX;
    deque<pair<string, int>> queue;
    queue.push_back(make_pair("", 0));
    while (queue.size() > 0) {
        auto current = queue.front();
        queue.pop_front();
        if (current.second >= possiblePaths.size()) {
            if (current.first.length() < minLength) {
                minLength = current.first.length();
                sequences.clear();
            }
            sequences.push_back(current.first);
            continue;
        }
        for (string s : possiblePaths[current.second]) {
            queue.push_back(make_pair(current.first + s, current.second + 1));
        }
    }
    return sequences;
}

long solvePart1(vector<string> lines) {
    auto allPathsNumeric = buildPaths(NUMERIC_KEYPAD, NUMERIC_MATRIX, 4, 3);
    auto allPathsDirectional = buildPaths(DIRECTIONAL_KEYPAD, DIRECTIONAL_MATRIX, 2, 3);
    long complexity = 0;
    for (string code : lines) {
        vector<vector<string>> possiblePaths;
        char previous = 'A';
        for (char next : code) {
            possiblePaths.push_back(allPathsNumeric.at(make_pair(previous, next)));
            previous = next;
        }
        vector<string> numericSeqs = combine(possiblePaths);

        vector<string> shortestSeqs;
        for (int i = 0; i < 2; i++) {
            int minLength = INT_MAX;
            for (string seq : numericSeqs) {
                previous = 'A';
                possiblePaths.clear();
                for (char next : seq) {
                    possiblePaths.push_back(allPathsDirectional.at(make_pair(previous, next)));
                    previous = next;
                }
                vector<string> directionalSeqs = combine(possiblePaths);
                for (string s : directionalSeqs) {
                    if (s.length() < minLength) {
                        shortestSeqs.clear();
                        minLength = s.length();
                    }
                    if (s.length() == minLength) {
                        shortestSeqs.push_back(s);
                    }
                }
            }
            numericSeqs = shortestSeqs;
        }
        cout << shortestSeqs[0].length() << " * " << stol(code.substr(0, code.length()-1)) << endl;
        complexity += shortestSeqs[0].length() * stol(code.substr(0, code.length()-1));
    }    
    return complexity;
}

long processSequence(string inputSeq, int currentRobot, int totalRobots,
    map<pair<char, char>, vector<string>> allPathsDirectional, map<pair<int, string>, long> *memo) {
    
    pair<int, string> memoKey = make_pair(currentRobot, inputSeq);
    if (memo->count(memoKey) > 0) {
        return memo->at(memoKey);
    }
    if (currentRobot + 1 == totalRobots) {
        char previous = 'A';
        long sumLength = 0;
        for (char next : inputSeq) {
            sumLength += allPathsDirectional.at(make_pair(previous, next))[0].length();
            previous = next;
        }
        memo->emplace(memoKey, sumLength);
        return sumLength;
    }
    char previous = 'A';
    long sumLength = 0;
    for (char next : inputSeq) {
        vector<string> subSequences = allPathsDirectional.at(make_pair(previous, next));
        long minLength = LONG_MAX;
        for (string subSeq : subSequences) {
            long currentLength = processSequence(subSeq, currentRobot + 1, totalRobots, allPathsDirectional, memo);
            minLength = min(minLength, currentLength);
        }
        previous = next;
        sumLength += minLength;
    }
    memo->emplace(memoKey, sumLength);
    return sumLength;
}

long solvePart2(vector<string> lines) {
    auto allPathsNumeric = buildPaths(NUMERIC_KEYPAD, NUMERIC_MATRIX, 4, 3);
    auto allPathsDirectional = buildPaths(DIRECTIONAL_KEYPAD, DIRECTIONAL_MATRIX, 2, 3);
    long complexity = 0;
    for (string code : lines) {
        vector<vector<string>> possiblePaths;
        char previous = 'A';
        for (char next : code) {
            possiblePaths.push_back(allPathsNumeric.at(make_pair(previous, next)));
            previous = next;
        }
        vector<string> numericSeqs = combine(possiblePaths);

        long minLength = LONG_MAX;
        map<pair<int, string>, long> memo;
        for (string s : numericSeqs) {
            long currentLength = processSequence(s, 0, 25, allPathsDirectional, &memo);
            minLength = min(minLength, currentLength);
        }
        cout << minLength << " * " << stol(code.substr(0, code.length()-1)) << endl;
        complexity += minLength * stol(code.substr(0, code.length()-1));
    }    
    return complexity;
}

int main() {
    vector<string> lines = readLines("data/input21.txt");

    auto t1 = chrono::high_resolution_clock::now();
    long complexity = solvePart1(lines);
    auto t2 = chrono::high_resolution_clock::now();
    auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 1 - complexity: " << complexity << " Duration (ms): " << ms_int.count() << endl;
    t1 = chrono::high_resolution_clock::now();
    complexity = solvePart2(lines);
    t2 = chrono::high_resolution_clock::now();
    ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 2 - complexity: " << complexity << " Duration (ms): " << ms_int.count() << endl;
    return 0;
}
