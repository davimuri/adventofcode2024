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

vector<string> tokenize(string text, char delimiter) {
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

void print(vector<int> nums) {
    for (int n : nums) {
        cout << n << " ";
    }
    cout << endl;
}