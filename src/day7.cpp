#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
using namespace std;

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

vector<long> getNumbers(string line) {
    vector<long> nums;
    string testValue = "";
    int i = 0;
    while (line[i] != ':') {
        testValue += line[i];
        i++;
    }
    nums.push_back(stol(testValue));
    i += 2;
    string numStr = "";
    while (line[i] != '\0') {
        if (line[i] == ' ') {
            nums.push_back(stol(numStr));
            numStr = "";
        } else {
            numStr += line[i];
        }
        i++;
    }
    if (numStr != "") {
        nums.push_back(stol(numStr));
    }
    return nums;
}

bool equationTrue(vector<long> nums, long currentValue, int currentIndex) {
    if (nums[0] == currentValue && currentIndex == nums.size()) {
        return true;
    }
    if (currentIndex >= nums.size()) {
        return false;
    }
    if (equationTrue(nums, currentValue * nums[currentIndex], currentIndex+1)) {
        return true;
    }
    if (equationTrue(nums, currentValue + nums[currentIndex], currentIndex+1)) {
        return true;
    }
    return false;
}

bool equationTruePart2(vector<long> nums, long currentValue, int currentIndex) {
    if (nums[0] == currentValue && currentIndex == nums.size()) {
        return true;
    }
    if (currentIndex >= nums.size()) {
        return false;
    }
    if (equationTruePart2(nums, currentValue * nums[currentIndex], currentIndex+1)) {
        return true;
    }
    if (equationTruePart2(nums, currentValue + nums[currentIndex], currentIndex+1)) {
        return true;
    }
    long newVal = stol(to_string(currentValue) + to_string(nums[currentIndex]));
    if (equationTruePart2(nums, newVal, currentIndex+1)) {
        return true;
    }
    return false;
}

int main() {
    vector<string> lines = readLines("data/input07.txt");
    long sum1 = 0;
    long sum2 = 0;
    for (string line : lines) {
        vector<long> nums = getNumbers(line);
        if (equationTrue(nums, nums[1], 2)) {
            sum1 += nums[0];
        }
        if (equationTruePart2(nums, nums[1], 2)) {
            sum2 += nums[0];
        }
    }
    cout << "Part 1 - sum: " << sum1 << endl;
    cout << "Part 2 - sum: " << sum2 << endl;
    return 0;
}
