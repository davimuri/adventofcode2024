#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
using namespace std;

const int ROWS = 50;
const int COLS = 50;

struct Point {
    int x;
    int y;

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

map<char, vector<Point>> getAntennas(vector<string> lines) {
    map<char, vector<Point>> antennas;
    for (int i = 0; i < lines.size(); i++) {
        for (int j = 0; j < lines[i].length(); j++) {
            if (lines[i][j] != '.') {
                if (antennas.count(lines[i][j]) == 0) {
                    antennas[lines[i][j]] = vector<Point>();
                }
                Point loc;
                loc.x = j;
                loc.y = i;
                antennas[lines[i][j]].push_back(loc);
            }
        }
    }
    return antennas;
}

Point calculateAntinode(Point p1, Point p2) {
    int dx1 = p1.x - p2.x;
    int dy1 = p1.y - p2.y;
    Point antinode1;
    antinode1.x = p1.x + dx1;
    antinode1.y = p1.y + dy1;
    return antinode1;
}

vector<Point> calculateAntinodes(Point p1, Point p2, bool justOne) {
    int dx1 = p1.x - p2.x;
    int dy1 = p1.y - p2.y;
    int newX = p1.x + dx1;
    int newY = p1.y + dy1;
    vector<Point> antinodes;
    while (newX >= 0 && newX < COLS && newY >= 0 && newY < ROWS) {
        Point antinode;
        antinode.x = newX;
        antinode.y = newY;
        antinodes.push_back(antinode);
        if (justOne) {
            return antinodes;
        }
        newX += dx1;
        newY += dy1;
    }
    if (!justOne) {
        antinodes.push_back(p1);
    }
    return antinodes;
}

int main() {
    vector<string> lines = readLines("data/input08.txt");
    map<char, vector<Point>> antennas = getAntennas(lines);
    set<Point> allAntinodes;
    set<Point> allAntinodesPart2;

    for (const auto& entry : antennas) {
        // cout << entry.first << " --- " << endl;
        auto locations = entry.second;
        for (int i = 0; i < locations.size(); i++) {
            for (int j = i+1; j < locations.size(); j++) {
                // cout << "(" <<locations[i].x << ", " << locations[i].y << ")";
                // cout << "(" <<locations[j].x << ", " << locations[j].y << ")" << endl;
                vector<Point> currentAntinodes = calculateAntinodes(locations[i], locations[j], true);
                for (Point p : currentAntinodes) {
                    allAntinodes.insert(p);
                }
                currentAntinodes = calculateAntinodes(locations[j], locations[i], true);
                for (Point p : currentAntinodes) {
                    allAntinodes.insert(p);
                }

                currentAntinodes = calculateAntinodes(locations[i], locations[j], false);
                for (Point p : currentAntinodes) {
                    allAntinodesPart2.insert(p);
                }
                currentAntinodes = calculateAntinodes(locations[j], locations[i], false);
                for (Point p : currentAntinodes) {
                    allAntinodesPart2.insert(p);
                }

            }
        }
        
    }
    // char endStatus[ROWS][COLS];
    // for (int i = 0; i < ROWS; i++) {
    //     for (int j = 0; j < COLS; j++) {
    //         endStatus[i][j] = lines[i][j];
    //     }
    // }
    // for (Point p : allAntinodesPart2) {
    //     endStatus[p.y][p.x] = '#';
    // }
    // for (int i = 0; i < ROWS; i++) {
    //     for (int j = 0; j < COLS; j++) {
    //         cout << endStatus[i][j];
    //     }
    //     cout << endl;
    // }

    cout << "Part 1 - antinodes: " << allAntinodes.size() << endl;
    cout << "Part 2 - antinodes: " << allAntinodesPart2.size() << endl;
    return 0;
}
