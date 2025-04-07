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

vector<string> getNumbers(string text, char delimiter) {
    stringstream ss(text);
    vector<string> v;
    string s;
    while (getline(ss, s, delimiter)) {
        if (s != "") {
            v.push_back(s);
        }
    }
    return v;
}

bool isUpdateCorrect(vector<string> nums, set<string> rules) {
    for (int i = 0; i < nums.size(); i++) {
        for (int j = i+1; j < nums.size(); j++) {
            string newOrder = nums[j] + "|" + nums[i];
            if (rules.find(newOrder) != rules.end()){
                return false;
            }
        }
    }
    return true;
}

void fixUpdate(vector<string> &nums, set<string> rules) {
    for (int i = 0; i < nums.size(); i++) {
        for (int j = i+1; j < nums.size(); j++) {
            string newOrder = nums[j] + "|" + nums[i];
            if (rules.find(newOrder) != rules.end()){
                string tmp = nums[i];
                nums[i] = nums[j];
                nums[j] = tmp;
                j = i+1;
            }
        }
    }
}

int main()
{
    vector<string> lines = readLines("data/input05.txt");
    set<string> rules = {};
    int linesIndex = 0;
    while (linesIndex < lines.size()) {
        string l = lines[linesIndex++];
        if (l == "") {
            break;
        }
        rules.insert(l);
    }
    cout << "Rules: " << rules.size() << endl;
    int sum = 0;
    int sum2 = 0;
    while (linesIndex < lines.size()) {
        string l = lines[linesIndex++];
        vector<string> nums = getNumbers(l, ',');
        if (isUpdateCorrect(nums, rules)) {
            sum += stoi(nums[nums.size() / 2]);
        } else {
            fixUpdate(nums, rules);
            sum2 += stoi(nums[nums.size() / 2]);
        }
    }
    cout << "Part 1 - sum: " << sum << endl;
    cout << "Part 2 - sum2: " << sum2 << endl;
    return 0;
}
