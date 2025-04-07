#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include "utils.cpp"

using namespace std;

const int ROWS = 43;
const int COLS = 43;
// const int ROWS = 8;
// const int COLS = 8;

struct Point {
    int x;
    int y;

    Point(int px, int py) {
        x = px;
        y = py;
    }
    
    bool operator < (const Point &other) const { return y*COLS + x < other.y*COLS + other.x; }
};

const Point DIRECTIONS[] = {Point(-1, 0), Point(1, 0), Point(0, -1), Point(0, 1)};

int calculateScore(vector<string> topoMap, Point trailhead) {
    set<Point> visited;
    deque<Point> queue;
    queue.push_back(trailhead);
    int score = 0;
    while (!queue.empty()) {
        Point current = queue.front();
        queue.pop_front();
        if (topoMap[current.y][current.x] == '9' && visited.find(current) == visited.end()) {
            score++;
            visited.insert(current);
            continue;
        }
        visited.insert(current);
        for (Point dir : DIRECTIONS) {
            int newX = current.x + dir.x;
            int newY = current.y + dir.y;
            Point newPoint = Point(newX, newY);
            if (visited.find(newPoint) == visited.end()
                && (topoMap[current.y][current.x] - '0') + 1 == (topoMap[newY][newX] - '0')) {
                queue.push_back(newPoint);
            }
        }
    }
    return score;
}

int calculateScorePart2(vector<string> topoMap, Point trailhead) {
    deque<Point> queue;
    queue.push_back(trailhead);
    int score = 0;
    while (!queue.empty()) {
        Point current = queue.front();
        queue.pop_front();
        if (topoMap[current.y][current.x] == '9') {
            score++;
            continue;
        }
        for (Point dir : DIRECTIONS) {
            int newX = current.x + dir.x;
            int newY = current.y + dir.y;
            Point newPoint = Point(newX, newY);
            if ((topoMap[current.y][current.x] - '0') + 1 == (topoMap[newY][newX] - '0')) {
                queue.push_back(newPoint);
            }
        }
    }
    return score;
}

int main() {
    vector<string> lines = readLines("data/input10.txt");
    auto t1 = chrono::high_resolution_clock::now();
    int scoreSum = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (lines[i][j] == '0') {
                int score = calculateScore(lines, Point(j, i));
                scoreSum += score;
            }
        }
    }
    auto t2 = chrono::high_resolution_clock::now();
    auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 1 - score sum: " << scoreSum << " Duration (ms): " << ms_int.count() << endl;

    t1 = chrono::high_resolution_clock::now();
    int scoreSum2 = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (lines[i][j] == '0') {
                int score = calculateScorePart2(lines, Point(j, i));
                scoreSum2 += score;
            }
        }
    }
    t2 = chrono::high_resolution_clock::now();
    ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 2 - score sum: " << scoreSum2 << " Duration (ms): " << ms_int.count() << endl;

    return 0;
}
