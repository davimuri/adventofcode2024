#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
using namespace std;

vector<int> getNumbers(string text, char delimiter) {
    stringstream ss(text);
    vector<int> v;
    string s;
    while (getline(ss, s, delimiter)) {
        if (s != "") {
            v.push_back(stoi(s));
        }
    }
    return v;
}

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

bool isPairSafe(vector<int> report, int left, int right, bool ascending) {
    int diff = report[left] - report[right];
    if ((ascending && diff >= 0) || (!ascending && diff <= 0)) {
        return false;
    }
    diff = abs(diff);
    if (diff < 1 || diff > 3) {
        return false;
    }
    return true;
}

bool isSafePart2(vector<int> report) {
    int indexes[4] = {-1, -1, -1, -1};
    int currentIdx = 0;
    int violations = 0;
    int ascendingCount = 0;
    int descendingCount = 0;
    for (int i = 1; i < report.size(); i++) {
        if (report[i-1] > report[i]) {
            descendingCount++;
        } else {
            ascendingCount++;
        }
    }
    bool assumeAscending = ascendingCount > descendingCount ? true : false;
    if ((assumeAscending && descendingCount >= 2) || (!assumeAscending && ascendingCount >= 2)) {
        return false;
    }
    for (int i = 1; i < report.size(); i++) {
        if (!isPairSafe(report, i-1, i, assumeAscending)) {
            violations++;
            if (currentIdx < 4) {
                indexes[currentIdx++] = i-1;
                indexes[currentIdx++] = i;
            }
        }
    }

    if (violations > 2) {
        return false;
    }
    if (violations == 2) {
        if (indexes[1] != indexes[2]) {
            // 2 violations no consecutives
            return false;
        }
        if (!isPairSafe(report, indexes[0], indexes[3], assumeAscending)) {
            return false;
        }
    }
    if (violations == 1) {
        // check removing indexes[0] 
        int left = indexes[0]-1;
        int right = indexes[1];
        bool safe = true;
        if (left >= 0) {
            if (!isPairSafe(report, left, right, assumeAscending)) {
                safe = false;
            }
        }
        if (!safe) {
            // check removing indexes[1]
            int left = indexes[0];
            int right = indexes[1]+1;
            if (right < report.size()) {
                if (!isPairSafe(report, left, right, assumeAscending)) {
                    return false;
                }
            }
        }
    }
    return true;
}

int main()
{
    vector<string> lines = readLines("data/input02.txt");
    vector< vector<int> > reports;
    for (string l : lines) {
        vector<int> nums = getNumbers(l, ' ');
        reports.push_back(nums);
    }
    int safeCount = 0;
    for (vector<int> report : reports) {
        bool ascendent = report[0] < report[1] ? true : false;
        bool safe = true;
        for (int i = 1; i < report.size() && safe; i++) {
            int diff = report[i-1] - report[i];
            if ((ascendent && diff >= 0) || (!ascendent && diff <= 0)) {
                safe = false;
                continue;
            }
            diff = abs(diff);
            if (diff < 1 || diff > 3) {
                safe = false;
            }
        }
        if (safe) {
            safeCount++;
        }
    }
    cout << "Part 1 - safe count: " << safeCount << endl;

    int safeCount2 = 0;
    for (vector<int> report : reports) {
        if (isSafePart2(report)) {
            safeCount2++;
        }
    }
    cout << "Part 2 - safe count: " << safeCount2 << endl;

    return 0;
}
