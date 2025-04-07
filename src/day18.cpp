#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include <deque>
#include "utils.cpp"

using namespace std;

const int ROWS = 71;
const int COLS = 71;
const int FALLEN_BYTES = 1024;
// const int ROWS = 7;
// const int COLS = 7;
// const int FALLEN_BYTES = 12;

const pair<int, int> DIRECTIONS[4] = {make_pair(1, 0), make_pair(0, 1), make_pair(-1, 0), make_pair(0, -1)};

vector<pair<int, int>> getCoordinates(vector<string> lines) {
    vector<pair<int, int>> coords;
    for (string l : lines) {
        auto tokens = tokenize(l, ',');
        coords.push_back(make_pair(stoi(tokens[0]), stoi(tokens[1])));
    }
    return coords;
}

int solvePart1(vector<pair<int, int>> coordinates) {
    char memory[ROWS][COLS];
    int steps[ROWS][COLS];
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            memory[i][j] = '.';
            steps[i][j] = 0;
        }
    }
    for (int i = 0; i < FALLEN_BYTES; i++) {
        auto pos = coordinates[i];
        memory[pos.second][pos.first] = '#';
    }
    deque<pair<int, int>> queue;
    queue.push_back(make_pair(0,0));
    int minSteps = -1;
    while (queue.size() > 0) {
        auto pos = queue.front();
        queue.pop_front();
        int currentSteps = steps[pos.second][pos.first];
        if (pos.first == COLS-1 && pos.second == ROWS-1) {
            minSteps = currentSteps;
            break;
        }
        for (int i = 0; i < 4; i++) {
            auto dir = DIRECTIONS[i];
            int nextRow = pos.second + dir.second;
            int nextCol = pos.first + dir.first;
            if (nextRow < 0 || nextRow >= ROWS || nextCol < 0 || nextCol >= COLS || memory[nextRow][nextCol] == '#') {
                continue;
            }
            if (steps[nextRow][nextCol] == 0 || steps[nextRow][nextCol] > currentSteps + 1) {
                queue.push_back(make_pair(nextCol, nextRow));
                steps[nextRow][nextCol] = currentSteps + 1;
                memory[nextRow][nextCol] = 'O';
            }
        }
    }
    return minSteps;
}

string solvePart2(vector<pair<int, int>> coordinates) {
    char memory[ROWS][COLS];
    int steps[ROWS][COLS];
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            memory[i][j] = '.';
            steps[i][j] = 0;
        }
    }
    for (int i = 0; i < FALLEN_BYTES; i++) {
        auto pos = coordinates[i];
        memory[pos.second][pos.first] = '#';
    }
    for (int b = FALLEN_BYTES; b < coordinates.size(); b++) {
        auto bytePos = coordinates[b];
        memory[bytePos.second][bytePos.first] = '#';
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                steps[i][j] = 0;
            }
        }

        deque<pair<int, int>> queue;
        queue.push_back(make_pair(0,0));
        int minSteps = -1;
        while (queue.size() > 0) {
            auto pos = queue.front();
            queue.pop_front();
            int currentSteps = steps[pos.second][pos.first];
            if (pos.first == COLS-1 && pos.second == ROWS-1) {
                minSteps = currentSteps;
                break;
            }
            for (int i = 0; i < 4; i++) {
                auto dir = DIRECTIONS[i];
                int nextRow = pos.second + dir.second;
                int nextCol = pos.first + dir.first;
                if (nextRow < 0 || nextRow >= ROWS || nextCol < 0 || nextCol >= COLS || memory[nextRow][nextCol] == '#') {
                    continue;
                }
                if (steps[nextRow][nextCol] == 0 || steps[nextRow][nextCol] > currentSteps + 1) {
                    queue.push_back(make_pair(nextCol, nextRow));
                    steps[nextRow][nextCol] = currentSteps + 1;
                    memory[nextRow][nextCol] = 'O';
                }
            }
        }
        queue.clear();
        if (minSteps == -1) {
            return to_string(bytePos.first) + "," + to_string(bytePos.second);
        }
    }
    return "not found";
}

int main() {
    vector<string> lines = readLines("data/input18.txt");
    vector<pair<int, int>> coordinates = getCoordinates(lines);
    auto t1 = chrono::high_resolution_clock::now();
    int steps = solvePart1(coordinates);
    auto t2 = chrono::high_resolution_clock::now();
    auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 1 - steps: " << steps << " Duration (ms): " << ms_int.count() << endl;

    t1 = chrono::high_resolution_clock::now();
    string solutionPart2 = solvePart2(coordinates);
    t2 = chrono::high_resolution_clock::now();
    ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 2 - solution: " << solutionPart2 << " Duration (ms): " << ms_int.count() << endl;

    return 0;
}
