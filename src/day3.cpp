#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <regex>
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

int doMultiplications(string text) {
    /*regex r("(mul)");
    smatch match;
    if (regex_search(text, match, r)) {
        cout << "Prefix: " << match.prefix() << endl;
        cout << "Suffix: " << match.suffix() << endl;
        for(auto v: match) {
            cout << v << endl;
        }
    }*/

    regex mul_regex("(mul\\([0-9]+,[0-9]+\\))");
    auto it_begin = sregex_iterator(text.begin(), text.end(), mul_regex);
    auto it_end = sregex_iterator();
 
    cout << "Found " << distance(it_begin, it_end) << " mul:\n";
 
    int sum = 0;
    for (sregex_iterator i = it_begin; i != it_end; ++i) {
        smatch match = *i;
        string match_str = match.str();
        cout << match_str << endl;
        string nums = match_str.substr(4, match_str.length()-5);
        auto numbers = getNumbers(nums, ',');
        sum += numbers[0] * numbers[1];
        //cout << numbers[0] << " * " << numbers[1] << " = " << numbers[0] * numbers[1] << endl;
    }
    return sum;
}

int doMultiplicationsPart2(string text) {
    regex mul_regex("(do\\(\\))|(don't\\(\\))|(mul\\([0-9]+,[0-9]+\\))");
    auto it_begin = sregex_iterator(text.begin(), text.end(), mul_regex);
    auto it_end = sregex_iterator();
 
    cout << "Found " << distance(it_begin, it_end) << " mul:\n";
 
    int sum = 0;
    bool doMul = true;
    for (sregex_iterator i = it_begin; i != it_end; ++i) {
        smatch match = *i;
        string match_str = match.str();
        if (match_str == "do()") {
            doMul = true;
            cout << match_str << endl;
        } else if (match_str == "don't()") {
            doMul = false;
            cout << match_str << endl;
        } else if (doMul) {
            string nums = match_str.substr(4, match_str.length()-5);
            auto numbers = getNumbers(nums, ',');
            sum += numbers[0] * numbers[1];
            cout << match_str << " Y " << endl;
        } else {
            cout << match_str << " N " << endl;
        }
        //cout << numbers[0] << " * " << numbers[1] << " = " << numbers[0] * numbers[1] << endl;
    }
    return sum;
}

int main()
{
    vector<string> lines = readLines("data/input03.txt");
    int sum = 0;
    string concatenated = "";
    for (string l : lines) {
        //cout << l << endl;
        sum += doMultiplications(l);
        concatenated += l;        
    }
    cout << "Part 1 - sum: " << sum << endl;
    int sum2 = doMultiplicationsPart2(concatenated);
    cout << "Part 2 - sum2: " << sum2 << endl;
}
