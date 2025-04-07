#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
using namespace std;

const int DIRECTIONS[4][2] =  {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
const int ROWS = 130;
const int COLS = 130;

struct Point {
    int x;
    int y;
    int dirIndex;

    bool operator < (const Point &other) const { return y*COLS + x < other.y*COLS + other.x; }
};

vector<string> readLines(string filename) {
    fstream InputFile(filename, ios_base::in);
    vector<string> lines;
    string text;
    while (getline(InputFile, text)) {
        lines.push_back(text);
    }
    InputFile.close();
    return lines;
}

array<int, 2> findStartPosition(vector<string> matrix) {
    array<int, 2> pos = {0, 0};
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].length(); j++) {
            if (matrix[i][j] != '.' && matrix[i][j] != '#') {
                pos[0] = j;
                pos[1] = i;
                return pos;
            }
        }
    }
    return pos;
}

int getDirectionIndex(char start) {
    switch (start)
    {
        case '^': return 0;
        case '>': return 1;
        case 'V': 
        case 'v': return 2;
        case '<': return 3;
    }
    return -1;
}

char dirIndexToChar(int dirIndex) {
    switch (dirIndex)
    {
        case 0: return '^';
        case 1: return '>';
        case 2: return 'v';
        case 3: return '<';
    }
    return '&';
}



vector<Point> getPath(vector<string> lines, int xStart, int yStart, int dirIndex) {
    vector<Point> path;
    int x = xStart, y = yStart;
    while (x >= 0 && x < COLS && y >= 0 && y < ROWS) {
        if (lines[y][x] == '#') {
            x -= DIRECTIONS[dirIndex][0];
            y -= DIRECTIONS[dirIndex][1];
            dirIndex = (dirIndex + 1) % 4;
        } else {
            Point point;
            point.x = x;
            point.y = y;
            point.dirIndex = dirIndex;
            path.push_back(point);
        }
        x += DIRECTIONS[dirIndex][0];
        y += DIRECTIONS[dirIndex][1];
    }
    return path;
}

vector<Point> getPathIfCycle(vector<string> lines, int xStart, int yStart, int dirIndex) {
    vector<Point> path;
    int visited[ROWS][COLS][4];
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            for (int k = 0; k < 4; k++) {
                visited[i][j][k] = -1;
            }
        }
    }
    int x = xStart, y = yStart;
    while (x >= 0 && x < COLS && y >= 0 && y < ROWS) {
        if (lines[y][x] == '#') {
            x -= DIRECTIONS[dirIndex][0];
            y -= DIRECTIONS[dirIndex][1];
            dirIndex = (dirIndex + 1) % 4;
        } else {
            Point point;
            point.x = x;
            point.y = y;
            point.dirIndex = dirIndex;
            path.push_back(point);
            bool visitedAsigned = false;
            for (int i = 0; i < 4 && !visitedAsigned; i++) {
                if (visited[y][x][i] == dirIndex) {
                    // cout << "(" << x << ", " << y << ") visited " << i << ": " << visited[y][x][i] << endl;
                    return path;
                } else if (visited[y][x][i] == -1) {
                    visited[y][x][i] = dirIndex;
                    visitedAsigned = true;
                    // cout << "(" << x << ", " << y << ") visited asigned " << i << ": " << visited[y][x][i] << endl;
                }
            }
        }
        x += DIRECTIONS[dirIndex][0];
        y += DIRECTIONS[dirIndex][1];
    }
    path.clear();
    return path;
}

array<array<char, COLS>, ROWS> applyPath(vector<string> lines, vector<Point> path) {
    array<array<char, COLS>, ROWS> newMap = {'.'};
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            newMap[i][j] = lines[i][j];
        }
    }
    for (Point p : path) {
        newMap[p.y][p.x] = dirIndexToChar(p.dirIndex);
    }
    return newMap;
}

void printMap(array<array<char, COLS>, ROWS> myMap) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            cout << myMap[i][j];
        }
        cout << endl;
    }
}

set<Point> getDifferentPoints(vector<Point> path) {
    set<Point> differentPoints;
    for (Point p : path) {
        differentPoints.insert(p);
    }
    return differentPoints;
}

void solvePart1and2(vector<string> lines, int xStart, int yStart, int dirIndex) {
    vector<Point> path = getPath(lines, xStart, yStart, dirIndex);
    set<Point> differentPoints = getDifferentPoints(path);
    cout << "Part 1 count: " << differentPoints.size() << endl;
    int cycles = 0;
    differentPoints.erase(path[0]);
    for (Point p : differentPoints) {
        lines[p.y][p.x] = '#';
        vector<Point> cyclePath = getPathIfCycle(lines, xStart, yStart, dirIndex);
        lines[p.y][p.x] = '.';
        if (!cyclePath.empty()) {
            cycles++;
        }
    }
    cout << "Part 2 - count: " << cycles << endl;
}

int main() {
    vector<string> lines = readLines("data/input06.txt");
    if (lines.size() != ROWS || lines[0].length() != COLS) {
        cout << "Check input size" << endl;
        return 0;
    }
    array<int, 2> startPosition = findStartPosition(lines);
    cout << "x: " << startPosition[0] << " y: " << startPosition[1] << endl;
    char start = lines[startPosition[1]][startPosition[0]];
    int dirIndex = getDirectionIndex(start);
    cout << "start: " << start << " direction index: " << dirIndex << endl;
    solvePart1and2(lines, startPosition[0], startPosition[1], dirIndex);
    return 0;
}
