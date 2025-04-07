#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include <deque>
#include <limits>
#include "utils.cpp"

using namespace std;

const int ROWS = 50;
const int COLS = 50;

typedef char (&map_part_2_v2)[ROWS][COLS*2];

array<array<char, COLS>, ROWS> getMap(vector<string> lines) {
    array<array<char, COLS>, ROWS> map;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            map[i][j] = lines[i][j];
        }
    }
    return map;
}

pair<int, int> findRobotPosition(array<array<char, COLS>, ROWS> map) {
    int row = 0;
    int col = 0;    
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (map[i][j] == '@') {
                return make_pair(j, i);
            }
        }
    }
    return make_pair(0, 0);
}

pair<int, int> getDir(char dirChar) {
    switch (dirChar) {
    case '<':
        return make_pair(-1, 0);
        break;
    case '>':
        return make_pair(1, 0);
        break;
    case '^':
        return make_pair(0, -1);
        break;
    case 'v':
        return make_pair(0, 1);
        break;
    }
    return make_pair(0, 0);
}

array<array<char, COLS>, ROWS> move(array<array<char, COLS>, ROWS> map, vector<string> lines) {
    pair<int, int> pos = findRobotPosition(map);
    int row = pos.second;
    int col = pos.first;
    map[row][col] = '.';
    for (int i = ROWS+1; i < lines.size(); i++) {
        for (int j = 0; j < lines[i].length(); j++) {
            char dirChar = lines[i][j];
            auto dir = getDir(dirChar);
            switch (map[row+dir.second][col+dir.first]) {
                case '.':
                    row += dir.second;
                    col += dir.first;
                    break;
                case '#':
                    break;
                case 'O':
                    int tmpRow = row+dir.second;
                    int tmpCol = col+dir.first;
                    while (map[tmpRow][tmpCol] == 'O') {
                        tmpRow += dir.second;
                        tmpCol += dir.first;
                    }
                    if (map[tmpRow][tmpCol] == '.') {
                        map[tmpRow][tmpCol] = 'O';
                        row += dir.second;
                        col += dir.first;
                        map[row][col] = '.';
                    }
                    break;
            }
        }
    }
    return map;
}

long sumBoxesPositions(array<array<char, COLS>, ROWS> map) {
    long sum = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (map[i][j] == 'O') {
                sum += 100 * i + j;
            }
        }
    }
    return sum;
}

map_part_2_v2 getMapPart2(array<array<char, COLS>, ROWS> map) {
    static char newMap[ROWS][COLS*2];
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int newJ = j*2;
            switch (map[i][j]) {
            case '#':
                newMap[i][newJ] = '#';
                newMap[i][newJ+1] = '#';
                break;
            case 'O':
                newMap[i][newJ] = '[';
                newMap[i][newJ+1] = ']';
                break;
            case '.':
                newMap[i][newJ] = '.';
                newMap[i][newJ+1] = '.';
                break;
            case '@':
                newMap[i][newJ] = '@';
                newMap[i][newJ+1] = '.';
                break;
            }
        }
    }
    return newMap;
}

bool moveBoxesRight(map_part_2_v2 map, int row, int col) {
    int right = col+1;
    while (map[row][right] == '[' || map[row][right] == ']') {
        right++;
    }
    if (map[row][right] == '.') {
        for (int k = right-1; k >= col+1; k--) {
            map[row][k+1] = map[row][k];
        }
        map[row][col+1] = '.';
        return true;
    }
    return false;
}

bool moveBoxesLeft(map_part_2_v2 map, int row, int col) {
    int left = col-1;
    while (map[row][left] == '[' || map[row][left] == ']') {
        left--;
    }
    if (map[row][left] == '.') {
        for (int k = left+1; k <= col-1; k++) {
            map[row][k-1] = map[row][k];
        }
        map[row][col-1] = '.';
        return true;
    }
    return false;
}

bool moveBoxesUp(map_part_2_v2 map, int row, int col) {
    int startRow = row - 1;
    int left = col;
    int right = col+1;
    if (map[startRow][col] == ']') {
        left = col - 1;
        right = col;
    }
    deque<pair<int, int>> queue;
    queue.push_back(make_pair(left, startRow));
    queue.push_back(make_pair(right, startRow));
    bool foundWall = false;
    vector<pair<int, int>> toMove;
    while (!queue.empty() && !foundWall) {
        auto pos = queue.front();
        toMove.push_back(pos);
        queue.pop_front();
        if (map[pos.second-1][pos.first] == '#') {
            foundWall = true;
        } else if (map[pos.second-1][pos.first] == ']') {
            queue.push_back(make_pair(pos.first, pos.second-1));
            queue.push_back(make_pair(pos.first-1, pos.second-1));
        } else if (map[pos.second-1][pos.first] == '[') {
            queue.push_back(make_pair(pos.first, pos.second-1));
            queue.push_back(make_pair(pos.first+1, pos.second-1));
        }
    }
    if (queue.empty() && !foundWall) {
        // cout << "Moving Boxes up...\n";
        auto ascending = [](pair<int, int> a, pair<int,int> b) {
          return a.second < b.second || a.first < b.first;
        };
        auto equal = [](pair<int, int> a, pair<int,int> b) {
          return a.first == b.first && a.second == b.second;
        };
        sort(toMove.begin(), toMove.end(), ascending);
        toMove.erase(unique(toMove.begin(), toMove.end(), equal), toMove.end());
        for (auto pos : toMove) {
            // cout << "Moving down " << pos.first << ", " << pos.second << endl;
            map[pos.second-1][pos.first] = map[pos.second][pos.first];
            map[pos.second][pos.first] = '.';
        }
        return true;
    }
    return false;
}

bool moveBoxesDown(map_part_2_v2 map, int row, int col) {
    int startRow = row + 1;
    int left = col;
    int right = col+1;
    if (map[startRow][col] == ']') {
        left = col - 1;
        right = col;
    }
    deque<pair<int, int>> queue;
    queue.push_back(make_pair(left, startRow));
    queue.push_back(make_pair(right, startRow));
    bool foundWall = false;
    vector<pair<int, int>> toMove;
    while (!queue.empty() && !foundWall) {
        auto pos = queue.front();
        toMove.push_back(pos);
        queue.pop_front();
        if (map[pos.second+1][pos.first] == '#') {
            foundWall = true;
        } else if (map[pos.second+1][pos.first] == ']') {
            auto p1 = make_pair(pos.first, pos.second+1);
            auto p2 = make_pair(pos.first-1, pos.second+1);
            queue.push_back(p1);
            queue.push_back(p2);
        } else if (map[pos.second+1][pos.first] == '[') {
            auto p1 = make_pair(pos.first, pos.second+1);
            auto p2 = make_pair(pos.first+1, pos.second+1);
            queue.push_back(p1);
            queue.push_back(p2);
        }
    }
    if (queue.empty() && !foundWall) {
        // cout << "Moving Boxes down...\n";
        auto greater = [](pair<int, int> a, pair<int,int> b) {
          return a.second > b.second || a.first > b.first;
        };
        auto equal = [](pair<int, int> a, pair<int,int> b) {
          return a.first == b.first && a.second == b.second;
        };
        sort(toMove.begin(), toMove.end(), greater);
        toMove.erase(unique(toMove.begin(), toMove.end(), equal), toMove.end());
        for (auto pos : toMove) {
            // cout << "Moving down " << pos.first << ", " << pos.second << endl;
            map[pos.second+1][pos.first] = map[pos.second][pos.first];
            map[pos.second][pos.first] = '.';
        }
        return true;
    }
    return false;
}

long movePart2(map_part_2_v2 map, vector<string> lines) {
    int row = 0;
    int col = 0;    
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < 2*COLS; j++) {
            if (map[i][j] == '@') {
                row = i;
                col = j;
            }
        }
    }
    cout << "Part2 - Initial position: " << row << ", " << col << endl;
    map[row][col] = '.';
    for (int i = ROWS+1; i < lines.size(); i++) {
        for (int j = 0; j < lines[i].length(); j++) {
            char dirChar = lines[i][j];
            auto dir = getDir(dirChar);
            switch (map[row+dir.second][col+dir.first]) {
            case '.':
                row += dir.second;
                col += dir.first;
                break;
            case '#':
                break;
            case '[':
            case ']':
                if (dirChar == '>') {
                    if (moveBoxesRight(map, row, col)) {
                        row += dir.second;
                        col += dir.first;
                    }
                } else if (dirChar == '<') {
                    if (moveBoxesLeft(map, row, col)) {
                        row += dir.second;
                        col += dir.first;
                    }
                } else if (dirChar == '^') {
                    if (moveBoxesUp(map, row, col)) {
                        row += dir.second;
                        col += dir.first;
                    }
                } else if (dirChar == 'v') {
                    if (moveBoxesDown(map, row, col)) {
                        row += dir.second;
                        col += dir.first;
                    }
                }
                break;
            }
        }
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS*2; j++) {
            cout << map[i][j];
        }
        cout << endl;
    }

    long sum = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS*2; j++) {
            if (map[i][j] == '[') {
                sum += 100 * i + j;
            }
        }
    }
    return sum;
}

//       []
//     []  []
//      [][]
//       []
void testMoveUpDownPart2() {
    char mapTest[ROWS][COLS*2] = {
        {'.', '.', '[', ']', '.', '.'},
        {'[', ']', '.', '.', '[', ']'},
        {'.', '[', ']', '[', ']', '.'},
        {'.', '.', '[', ']', '.', '.'},
        {'.', '.', '.', '.', '.', '.'},
    };
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS*2; j++) {
            cout << mapTest[i][j];
        }
        cout << endl;
    }
    cout << endl;
    moveBoxesUp(mapTest, 4, 2);
    // moveBoxesDown(mapTest, 0, 4);
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS*2; j++) {
            cout << mapTest[i][j];
        }
        cout << endl;
    }
}


int main() {
    vector<string> lines = readLines("data/input15.txt");
    auto t1 = chrono::high_resolution_clock::now();
    array<array<char, COLS>, ROWS> map = getMap(lines);
    map = move(map, lines);
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            cout << map[i][j];
        }
        cout << endl;
    }
    long sumPart1 = sumBoxesPositions(map);
    auto t2 = chrono::high_resolution_clock::now();
    auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 1 - sum of coordinates: " << sumPart1 << " Duration (ms): " << ms_int.count() << endl;

    t1 = chrono::high_resolution_clock::now();
    map = getMap(lines);
    map_part_2_v2 mapPart2 = getMapPart2(map);
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS*2; j++) {
            cout << mapPart2[i][j];
        }
        cout << endl;
    }
    long sumPart2 = movePart2(mapPart2, lines);
    t2 = chrono::high_resolution_clock::now();
    ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 2 - sum of coordinates: " << sumPart2 << " Duration (ms): " << ms_int.count() << endl;

    return 0;
}
