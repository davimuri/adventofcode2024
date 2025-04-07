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

const int ROWS = 141;
const int COLS = 141;
// const int ROWS = 15;
// const int COLS = 15;
const pair<int, int> DIRECTIONS[4] = {make_pair(1, 0), make_pair(0, 1), make_pair(-1, 0), make_pair(0, -1)};
typedef char (&maze_type)[ROWS][COLS];
enum CheatStatus { UNUSED, ACTIVE, DONE }; 

struct Node {
    int row;
    int col;
    int cost;
    CheatStatus cheatSatus;
    int cheatJumps;
    Node* previous;

    Node(int r, int c, int co) {
        row = r;
        col = c;
        cost = co;
        cheatSatus = UNUSED;
        cheatJumps = 0;
        previous = NULL;
    }
};

void initMaze(maze_type maze, vector<string> lines) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            maze[i][j] = lines[i][j];
        }
    }
}

Node* optimalCost(maze_type maze, int startRow, int startCol) {
    bool visited[ROWS][COLS];
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            visited[i][j] = false;
        }
    }
    deque<Node*> queue;
    Node* start = new Node(startRow, startCol, 0);
    queue.push_back(start);
    while (queue.size() > 0) {
        Node* current = queue.front();
        queue.pop_front();
        if (maze[current->row][current->col] == 'E') {
            // Node* lastNode = current;
            // while (lastNode != NULL) {
            //     cout << lastNode->row << ", " << lastNode->col << endl;
            //     lastNode = lastNode->previous;
            // }
            return current;
        }
        visited[current->row][current->col] = true;
        for (int i = 0; i < 4; i++) {
            int nextRow = current->row + DIRECTIONS[i].second;
            int nextCol = current->col + DIRECTIONS[i].first;
            if (maze[nextRow][nextCol] != '#' && !visited[nextRow][nextCol]) {
                Node* nextNode = new Node(nextRow, nextCol, current->cost+1);
                nextNode->previous = current;
                queue.push_back(nextNode);
            }
        }
    }
    return NULL;
}

int cheat(maze_type maze, int startRow, int startCol, Node* endNode) {
    map<pair<int, int>, Node*> optimalPathNodes;
    Node* current = endNode;
    while (current != NULL) {
        optimalPathNodes[make_pair(current->col, current->row)] = current;
        current = current->previous;
    }
    map<int, int> cheatsCount;
    for (auto& entry : optimalPathNodes) {
        current = entry.second;
        // cout << "(" << entry.second->col << ", " << entry.second->row << ", " << entry.second->cost << ") ";
        for (int i = 0; i < 4; i++) {
            int nextRow = current->row + DIRECTIONS[i].second;
            int nextCol = current->col + DIRECTIONS[i].first;
            int nextNextRow = nextRow + DIRECTIONS[i].second;
            int nextNextCol = nextCol + DIRECTIONS[i].first;
            pair<int, int> jumpPos = make_pair(nextNextCol, nextNextRow);
            if (optimalPathNodes.find(jumpPos) != optimalPathNodes.end() && maze[nextRow][nextCol] == '#') {
                Node* jumpNode = optimalPathNodes[jumpPos];
                if (jumpNode->cost > current->cost) {
                    int save = jumpNode->cost - current->cost - 2;
                    if (cheatsCount.find(save) == cheatsCount.end()) {
                        cheatsCount[save] = 1;
                    } else {
                        cheatsCount.at(save) = cheatsCount.at(save) + 1;
                    }
                }
            }
        }
    }
    int atLeast100saved = 0;
    for (auto& entry : cheatsCount) {
        // cout << "There are " << entry.second << " cheats that save " << entry.first << " picoseconds" << endl;
        if (entry.first >= 100) {
            atLeast100saved += entry.second;
        }
    }
    return atLeast100saved;
}

int cheatPart2(maze_type maze, int startRow, int startCol, Node* endNode) {
    vector<Node*> path;
    Node* current = endNode;
    while (current != NULL) {
        path.push_back(current);
        current = current->previous;
    }
    path = vector<Node*>(path.rbegin(), path.rend());
    map<int, int> cheatsCount;
    for (int i = 0; i < path.size(); i++) {
        for (int j = i + 1; j < path.size(); j++) {
            int actualCost = path[j]->cost - path[i]->cost;
            int distance = abs(path[j]->col - path[i]->col) + abs(path[j]->row - path[i]->row);
            if (distance > 20) {
                continue;
            }
            int saving = actualCost - distance;
            if (cheatsCount.find(saving) == cheatsCount.end()) {
                cheatsCount[saving] = 1;
            } else {
                cheatsCount.at(saving) = cheatsCount.at(saving) + 1;
            }
        }
    }
    int atLeast100saved = 0;
    for (auto& entry : cheatsCount) {
        // cout << "There are " << entry.second << " cheats that save " << entry.first << " picoseconds" << endl;
        if (entry.first >= 100) {
            atLeast100saved += entry.second;
        }
    }
    return atLeast100saved;
}


int main() {
    vector<string> lines = readLines("data/input20.txt");
    char maze[ROWS][COLS];
    initMaze(maze, lines);
    int startRow = -1;
    int startCol = -1;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (maze[i][j] == 'S') {
                startRow = i;
                startCol = j;
            }
        }
    }

    auto t1 = chrono::high_resolution_clock::now();
    Node* endNode = optimalCost(maze, startRow, startCol);
    int cheatsSavingAtLeast100 = cheat(maze, startRow, startCol, endNode);
    auto t2 = chrono::high_resolution_clock::now();
    auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 1 - cheats that save at least 100 picoseconds: " << cheatsSavingAtLeast100 << " Duration (ms): " << ms_int.count() << endl;

    t1 = chrono::high_resolution_clock::now();
    cheatsSavingAtLeast100 = cheatPart2(maze, startRow, startCol, endNode);
    t2 = chrono::high_resolution_clock::now();
    ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 2 - cheats that save at least 100 picoseconds: " << cheatsSavingAtLeast100 << " Duration (ms): " << ms_int.count() << endl;
    return 0;
}
