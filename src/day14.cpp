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

const int ROWS = 103;
const int COLS = 101;
// const int ROWS = 7;
// const int COLS = 11;


array<int, 4> parseLine(string line) {
    int commaPos = line.find(",");
    int posX = stoi(line.substr(2, commaPos-2));
    int whitespacePos = line.find(" ");
    int posY = stoi(line.substr(commaPos+1, whitespacePos-commaPos+1));
    int velocityPos = line.find("v=");
    commaPos = line.find(",", commaPos+1);
    int velX = stoi(line.substr(velocityPos+2, commaPos-velocityPos+2));
    int velY = stoi(line.substr(commaPos+1));
    
    return {posX, posY, velX, velY};
}

pair<int, int> move(array<int, 4> robot, int iterations, int rows, int cols) {
    int x = (robot[2] * iterations) % cols;
    if (robot[2] > 0) {
        x = (x + robot[0]) % cols;
    } else {
        // cout << "vel X negative" << endl;
        int posConverted = cols - 1 - robot[0];
        // cout << "x before offset " << x << endl;
        x = (abs(x) + posConverted) % cols;
        // cout << "x after offset " << x << endl;
        x = cols - 1 - x;
        // cout << "x converted " << x << endl;
    }
    int y = (robot[3] * iterations) % rows;
    if (robot[3] > 0) {
        y = (y + robot[1]) % rows;
    } else {
        int posConverted = rows - 1 - robot[1];
        y = (abs(y) + posConverted) % rows;
        y = rows - 1 - y;
    }
    return make_pair(x, y);
}

int quadrant(pair<int, int> pos, int rows, int cols) {
    if (pos.first < cols / 2) {
        if (pos.second < rows / 2) {
            return 0;
        } else if (pos.second > rows / 2) {
            return 2;
        }
    } else if (pos.first > cols / 2) {
        if (pos.second < rows / 2) {
            return 1;
        } else if (pos.second > rows / 2) {
            return 3;
        }
    }
    return -1;
}

long solvePart1(vector<string> lines) {
    int iterations = 100;
    int quadrants[4] = {0};
    for (string l : lines) {
        auto robot = parseLine(l);
        // cout << "x " << robot[0] << " y " << robot[1] << " velX " << robot[2] << " velY " << robot[3] << endl;
        auto endPos = move(robot, 100, ROWS, COLS);
        int idx = quadrant(endPos, ROWS, COLS);
        if (idx != -1) {
            quadrants[idx]++;
        }
    }
    long safetyFactor = 1;
    for (int i = 0; i < 4; i++) {
        safetyFactor *= quadrants[i];
    }
    return safetyFactor;
}
// https://courses.luxembourgishwithanne.lu/courses/1450288/lectures/33823985/hotmart_lecture_video_download_link/63621143
// lecture-attachment-63621143
// https://cdn.fs.teachablecdn.com/kCk3oz6BSpmy2CQBwSRZ
int metricSlope(char matrix[ROWS][COLS]) {
    vector<pair<int,int>> leftPositions;
    vector<pair<int, int>> rightPositions;
    for (int i = 0; i < ROWS; i++) {
        int left = 0;
        while (left < COLS && matrix[i][left] == '.') {
            left++;
        }
        if (left < COLS) {
            leftPositions.push_back(make_pair(left, i));
            int right = COLS - 1;
            while (right > left && matrix[i][right] == '.') {
                right--;
            }
            if (right >= left) {
                rightPositions.push_back(make_pair(right, i));
            }
        }
    }
    int count = 0;
    int sum = 0;
    for (int i = 1; i < leftPositions.size(); i++) {
        int dxLeft = leftPositions[i].first - leftPositions[i-1].first;
        int dxRight = rightPositions[i].first - rightPositions[i-1].first;
        sum += abs(-1 * dxLeft - dxRight);
        count++;
    }
    return sum / count;
}

int metricTogether(char matrix[ROWS][COLS]) {
    int countSpaces = 0;
    for (int i = 0; i < ROWS; i++) {
        int left = 0;
        while (left < COLS && matrix[i][left] == '.') {
            left++;
        }
        if (left < COLS) {
            int right = COLS - 1;
            while (right > left && matrix[i][right] == '.') {
                right--;
            }
            if (right > left) {
                for (int j = left; j < right; j++) {
                    if (matrix[i][j] == '.') {
                        countSpaces++;
                    }
                }
            }
        }
    }
    return countSpaces;
}

void solvePart2(vector<string> lines) {
    int iterations = 20000;
    vector<array<int, 4>> robots;
    for (string l : lines) {
        robots.push_back(parseLine(l));
    }
    int minMetric = INT_MAX;
    for (int i = 1; i <= iterations; i++) {
        char matrix[ROWS][COLS];
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                matrix[r][c] = '.';
            }
        }
        for (array<int, 4> r : robots) {
            auto endPos = move(r, i, ROWS, COLS);
            matrix[endPos.second][endPos.first] = '#';
        }
        minMetric = min(metricTogether(matrix), minMetric);
        if (minMetric < (COLS/3)*ROWS) {
            for (int r = 0; r < ROWS; r++) {
                for (int c = 0; c < COLS; c++) {
                    cout << matrix[r][c];
                }
                cout << endl;
            }
            cout << "Iteration " << i << endl;
            return;
        }
    }
}

int main() {
    vector<string> lines = readLines("data/input14.txt");
    auto t1 = chrono::high_resolution_clock::now();
    long safetyFactor = solvePart1(lines);
    auto t2 = chrono::high_resolution_clock::now();
    auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 1 naive - safetyFactor: " << safetyFactor << " Duration (ms): " << ms_int.count() << endl;
    solvePart2(lines);
    return 0;
}
