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

pair<long, long> parseButtonLine(string line) {
    string button = line.substr(7, 1);
    int commaPos = line.find(",");
    string x = line.substr(12, commaPos-12);
    int yPos = line.find("Y+")+2;
    string y = line.substr(yPos, line.length()-yPos);
    return make_pair(stol(x), stol(y));
}

pair<long, long> parsePrizeLine(string line, bool part2) {
    int xPos = line.find("X=")+2;
    int commaPos = line.find(",");
    long x = stol(line.substr(xPos, commaPos-xPos));
    int yPos = line.find("Y=")+2;
    long y = stol(line.substr(yPos, line.length()-yPos));
    if (part2) {
        x += 10000000000000;
        y += 10000000000000;
    }
    return make_pair(x, y);
}

long minimizeCost(pair<long, long> buttonA, pair<long, long> buttonB, pair<long, long> prize) {
    long minCost = LONG_MAX;
    int tries = 200;
    for (int i = 0; i < tries; i++) {
        for (int j = 0; j < tries; j++) {
            if (i + j < tries && buttonA.first * i + buttonB.first * j == prize.first
                && buttonA.second * i + buttonB.second * j == prize.second) {
                long cost = 3 * i + j;
                minCost = min(minCost, cost);
            }
        }
    }
    return minCost;
}

// https://www.reddit.com/r/adventofcode/comments/1hd7irq/2024_day_13_an_explanation_of_the_mathematics/
long minimizeCostSmarter(pair<long, long> buttonA, pair<long, long> buttonB, pair<long, long> prize) {
    // A*a_x + B*b_x = p_x
    // A*a_y + B*b_y = p_y

    // A = (p_x - B*b_x) / a_x
    // A = (p_y - B*b_y) / a_y
    // (p_x - B*b_x) / a_x = (p_y - B*b_y) / a_y
    // (p_x - B*b_x) * a_y = (p_y - B*b_y) * a_x
    // p_x*a_y - B*b_x*a_y = p_y*a_x - B*b_y*a_x
    // B = (p_y*a_x - p_x*a_y) / (b_y*a_x - b_x*a_y)
    long buttonBCount = (prize.second * buttonA.first - prize.first * buttonA.second) /
        (buttonB.second * buttonA.first - buttonB.first * buttonA.second);

    // A = (p_x - B*b_x) / a_x
    long buttonACount = (prize.first - buttonBCount * buttonB.first) / buttonA.first;

    long endPosX = buttonACount * buttonA.first + buttonBCount * buttonB.first;
    long endPosY = buttonACount * buttonA.second + buttonBCount * buttonB.second;
    long diffX = prize.first - endPosX;
    long diffY = prize.second - endPosY;
    if (diffX == 0 && diffY == 0) {
        return 3 * buttonACount + buttonBCount;
    }
    return LONG_MAX;
}

int main() {
    vector<string> lines = readLines("data/input13.txt");
    auto t1 = chrono::high_resolution_clock::now();
    long totalCost = 0;
    for (int i = 0; i < lines.size(); i+=4) {
        auto buttonA = parseButtonLine(lines[i]);
        auto buttonB = parseButtonLine(lines[i+1]);
        auto prize = parsePrizeLine(lines[i+2], false);
        long currentCost = minimizeCost(buttonA, buttonB, prize);
        if (currentCost < LONG_MAX) {
            totalCost += currentCost;
        }
    }
    auto t2 = chrono::high_resolution_clock::now();
    auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 1 naive - cost: " << totalCost << " Duration (ms): " << ms_int.count() << endl;

    t1 = chrono::high_resolution_clock::now();
    totalCost = 0;
    for (int i = 0; i < lines.size(); i+=4) {
        auto buttonA = parseButtonLine(lines[i]);
        auto buttonB = parseButtonLine(lines[i+1]);
        auto prize2 = parsePrizeLine(lines[i+2], false);
        long currentCost = minimizeCostSmarter(buttonA, buttonB, prize2);
        if (currentCost < LONG_MAX) {
            totalCost += currentCost;
        }
    }
    t2 = chrono::high_resolution_clock::now();
    ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 1 smarter - cost: " << totalCost << " Duration (ms): " << ms_int.count() << endl;

    t1 = chrono::high_resolution_clock::now();
    long totalCost2 = 0;
    for (int i = 0; i < lines.size(); i+=4) {
        auto buttonA = parseButtonLine(lines[i]);
        auto buttonB = parseButtonLine(lines[i+1]);
        auto prize2 = parsePrizeLine(lines[i+2], true);
        long currentCost = minimizeCostSmarter(buttonA, buttonB, prize2);
        if (currentCost < LONG_MAX) {
            totalCost2 += currentCost;
        }
    }
    t2 = chrono::high_resolution_clock::now();
    ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 2 smarter - cost: " << totalCost2 << " Duration (ms): " << ms_int.count() << endl;

    return 0;
}
