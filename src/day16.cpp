#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <deque>
#include <limits>
#include "utils.cpp"

using namespace std;

const int ROWS = 141;
const int COLS = 141;
// const int ROWS = 15;
// const int COLS = 15;
const pair<int, int> DIRECTIONS[4] = {make_pair(1, 0), make_pair(0, 1), make_pair(-1, 0), make_pair(0, -1)};
typedef char (&maze_type)[ROWS][COLS];

struct Node {
    int row;
    int col;
    long cost;
    int dirIdx;
    Node* previous;
};

void initMaze(maze_type maze, vector<string> lines) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            maze[i][j] = lines[i][j];
        }
    }
}

void printMaze(maze_type maze) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            cout << maze[i][j];
        }
        cout << endl;
    }
}

bool visited(Node* node, int row, int col) {
    if (node == NULL) {
        return false;
    }
    if (node->col == col && node->row == row) {
        return true;
    }
    return visited(node->previous, row, col);
}

long solvePart1(maze_type maze, pair<int, int> pos) {
    auto descending = [](Node* a, Node* b) {
        return a->cost > b->cost;
    };
    Node first;
    first.row = pos.second;
    first.col = pos.first;
    first.cost = 0;
    first.dirIdx = 0;
    first.previous = NULL;
    vector<Node*> nodes;
    nodes.push_back(&first);
    set<pair<int, int>> visited;
    long minCost = LONG_MAX;
    Node *endNode = NULL;
    while (nodes.size() > 0) {
        Node *current = nodes.back();
        nodes.pop_back();
        if (current->cost > minCost) {
            continue;
        }
        if (maze[current->row][current->col] == 'E') {
            minCost = current->cost;
            break;
        }
        visited.insert(make_pair(current->col, current->row));
        for (int i = 0; i < 4; i++) {
            int nextDirIdx = (current->dirIdx + i) % 4;
            auto dir = DIRECTIONS[nextDirIdx];
            int nextRow = current->row + dir.second;
            int nextCol = current->col + dir.first;
            pair<int, int> nextPos = make_pair(nextCol, nextRow);
            if (maze[nextRow][nextCol] == '#' || visited.count(nextPos) > 0 ) {
                continue;
            }
            long nextCost = current->cost;
            if (i == 0) {
                nextCost++;
            } else if (i == 1 || i == 3) {
                nextCost += 1001;
            } else if (i == 2) {
                nextCost += 2001;
            }
            Node *next = new Node;
            next->row = nextRow;
            next->col = nextCol;
            next->cost = nextCost;
            next->dirIdx = nextDirIdx;
            next->previous = current;
            nodes.push_back(next);
        }
        sort(nodes.begin(), nodes.end(), descending);
    }   
    return minCost;
}

long solvePart2(maze_type maze, pair<int, int> pos, long minCost) {
    long visitedCost[ROWS][COLS];
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            visitedCost[i][j] = LONG_MAX;
        }
    }
    auto descending = [](Node* a, Node* b) {
        return a->cost > b->cost;
    };
    Node first;
    first.row = pos.second;
    first.col = pos.first;
    first.cost = 0;
    first.dirIdx = 0;
    first.previous = NULL;
    deque<Node*> nodes;
    nodes.push_back(&first);
    Node *endNode = NULL;
    set<pair<int, int>> positionsInOptimalPaths;
    int optimalPaths = 0;
    while (nodes.size() > 0) {
        Node *current = nodes.front();
        nodes.pop_front();
        visitedCost[current->row][current->col] = current->cost;
        if (current->cost > minCost) {
            continue;
        }
        if (maze[current->row][current->col] == 'E' && minCost == current->cost) {
            optimalPaths++;
            endNode = current;
            while (endNode != NULL) {
                positionsInOptimalPaths.insert(make_pair(endNode->col, endNode->row));
                endNode = endNode->previous;
            }
            continue;
        }
        for (int i = 0; i < 4; i++) {
            int nextDirIdx = (current->dirIdx + i) % 4;
            auto dir = DIRECTIONS[nextDirIdx];
            int nextRow = current->row + dir.second;
            int nextCol = current->col + dir.first;
            long nextCost = current->cost;
            if (i == 0) {
                nextCost++;
            } else if (i == 1 || i == 3) {
                nextCost += 1001;
            } else if (i == 2) {
                continue;
            }
            if (maze[nextRow][nextCol] == '#' || nextCost > visitedCost[nextRow][nextCol]) {
                continue;
            }
            Node *next = new Node;
            next->row = nextRow;
            next->col = nextCol;
            next->cost = nextCost;
            next->dirIdx = nextDirIdx;
            next->previous = current;
            nodes.push_back(next);
        }
    }
    return positionsInOptimalPaths.size();
}

int main() {
    vector<string> lines = readLines("data/input16.txt");
    auto t1 = chrono::high_resolution_clock::now();
    char maze[ROWS][COLS];
    initMaze(maze, lines);
    // printMaze(maze);
    pair<int, int> start;
    pair<int, int> end;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (maze[i][j] == 'S') {
                start = make_pair(j, i);
            } else if (maze[i][j] == 'E') {
                end = make_pair(j, i);
            }
        }
    }
    long costPart1 = solvePart1(maze, start);
    auto t2 = chrono::high_resolution_clock::now();
    auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 1 - cost: " << costPart1 << " Duration (ms): " << ms_int.count() << endl;

    t1 = chrono::high_resolution_clock::now();
    long tiles = solvePart2(maze, start, costPart1);
    t2 = chrono::high_resolution_clock::now();
    ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 2 - tiles : " << tiles << " Duration (ms): " << ms_int.count() << endl;

    return 0;
}
