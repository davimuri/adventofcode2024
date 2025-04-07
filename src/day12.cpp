#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include <deque>
#include "utils.cpp"

using namespace std;

const int ROWS = 140;
const int COLS = 140;
// const int ROWS = 4;
// const int COLS = 4;
// const int ROWS = 10;
// const int COLS = 10;

class Point {
    public:
        int x;
        int y;

        Point(int px, int py) {
            x = px;
            y = py;
        }

        Point operator + (const Point& p) const {
            return Point(x + p.x, y + p.y);
        }
};

enum BorderOrientation {
  HORIZONTAL,
  VERTICAL
};

enum Direction {
  LEFT,
  RIGHT,
  UP,
  DOWN
};

// const Point DIRECTIONS[] = {Point(-1, 0), Point(1, 0), Point(0, -1), Point(0, 1)};
const map<Direction, Point> DIRECTIONS = {
    {LEFT, Point(-1, 0)}, 
    {RIGHT, Point(1, 0)},
    {UP, Point(0, -1)},
    {DOWN, Point(0, 1)}
};

class Border {
    public:
        BorderOrientation orientation;
        int borderIdx;
        int posIdx;
        Direction regionDirection; // region cell position: up, down, right, left

        Border(BorderOrientation h, int bi, int pi, Direction rp) {
            orientation = h;
            borderIdx = bi;
            posIdx = pi;
            regionDirection = rp;
        }

        bool sameSide(const Border& b) {
            return orientation == b.orientation && borderIdx == b.borderIdx 
                && abs(posIdx-b.posIdx) == 1 && regionDirection == b.regionDirection;
        }

        bool operator < (const Border& b) const {
            if (orientation < b.orientation) return true;
            if (orientation == b.orientation && borderIdx < b.borderIdx) return true;
            if (orientation == b.orientation && borderIdx == b.borderIdx && posIdx < b.posIdx) return true;
            return false;
        }

        string toString() {
            return "orient: " + to_string(orientation) + " borderIdx: " + to_string(borderIdx)
                + " posIdx: " + to_string(posIdx) + " regionDir: " + to_string(regionDirection);
        }
};

long solvePart1Naive(vector<string> land, int xStart, int yStart, bool (&visited)[ROWS][COLS]) {
    char plant = land[yStart][xStart];
    long area = 1;
    long perimeter = 0;
    deque<Point> queue;
    queue.push_back(Point(xStart, yStart));
    visited[yStart][xStart] = true;
    while (!queue.empty()) {
        Point current = queue.front();
        queue.pop_front();
        for (auto const& entry : DIRECTIONS) {
            Point newP = current + entry.second;
            if (newP.x < 0 || newP.x >= COLS || newP.y < 0 || newP.y >= ROWS || land[newP.y][newP.x] != plant) {
                perimeter++;
            }else if (land[newP.y][newP.x] == plant && !(visited[newP.y][newP.x])) {
                area++;
                visited[newP.y][newP.x] = true;
                queue.push_back(newP);
            }
        }
    }
    // cout << "region " << plant << " (" << xStart << ", " << yStart << ") area " << area << " perimeter " << perimeter << endl; 
    return area * perimeter;
}

Direction inverse(Direction dir) {
    switch (dir) {
    case UP:
        return DOWN;
        break;
    case DOWN:
        return UP;
        break;
    case RIGHT:
        return LEFT;
        break;
    case LEFT:
        return RIGHT;
        break;
    default:
        break;
    }
}

Border getBorder(Point regionCell, Point adjacentCell, Direction dir) {
    Direction regionDir = inverse(dir);
    BorderOrientation orientation = regionDir == UP || regionDir == DOWN ? HORIZONTAL : VERTICAL;
    int borderIdx = orientation == HORIZONTAL ? max(regionCell.y, adjacentCell.y) : max(regionCell.x, adjacentCell.x);
    int posIdx = orientation == HORIZONTAL ? regionCell.x : regionCell.y;
    return Border(orientation, borderIdx, posIdx, regionDir);
}

int countSides(vector<Border> borders) {
    sort(borders.begin(), borders.end());
    int sides = 1;
    // cout << "Borders " << borders[0].toString() << endl;
    for (int i = 1; i < borders.size(); i++) {
        // cout << borders[i].toString() << endl;
        if (!borders[i-1].sameSide(borders[i])) {
            sides++;
        }
    }
    return sides;
}

long solvePart2Naive(vector<string> land, int xStart, int yStart, bool (&visited)[ROWS][COLS]) {
    char plant = land[yStart][xStart];
    long area = 1;
    long perimeter = 0;
    deque<Point> queue;
    queue.push_back(Point(xStart, yStart));
    visited[yStart][xStart] = true;
    vector<Border> borders;
    while (!queue.empty()) {
        Point current = queue.front();
        queue.pop_front();
        for (auto const& entry : DIRECTIONS) {
            Point newP = current + entry.second;
            if (newP.x < 0 || newP.x >= COLS || newP.y < 0 || newP.y >= ROWS || land[newP.y][newP.x] != plant) {
                borders.push_back(getBorder(current, newP, entry.first));
                perimeter++;
            }else if (land[newP.y][newP.x] == plant && !(visited[newP.y][newP.x])) {
                area++;
                visited[newP.y][newP.x] = true;
                queue.push_back(newP);
            }
        }
    }
    long sides = countSides(borders);
    // cout << "region " << plant << " (" << xStart << ", " << yStart << ") area " << area 
    //     << " perimeter " << perimeter << " Sides " << sides << endl; 
    return area * sides;
}

int main() {
    vector<string> lines = readLines("data/input12.txt");
    vector<string> stones = tokenize(lines[0], ' ');
    auto t1 = chrono::high_resolution_clock::now();
    bool visited[ROWS][COLS];
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            visited[i][j] = false;
        }
    }
    long price1 = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (!visited[i][j]) {
                price1 += solvePart1Naive(lines, j, i, visited);
            }
        }
    }
    auto t2 = chrono::high_resolution_clock::now();
    auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 1 naive - price: " << price1 << " Duration (ms): " << ms_int.count() << endl;

    t1 = chrono::high_resolution_clock::now();
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            visited[i][j] = false;
        }
    }
    long price2 = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (!visited[i][j]) {
                price2 += solvePart2Naive(lines, j, i, visited);
            }
        }
    }
    t2 = chrono::high_resolution_clock::now();
    ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 2 naive - price: " << price2 << " Duration (ms): " << ms_int.count() << endl;

    return 0;
}
