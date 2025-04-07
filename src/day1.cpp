#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
using namespace std;

//g++ -std=c++11 -o build/day1 src/day1.cpp
int main() {
    fstream InputFile("data/input01.txt", ios_base::in);

    vector<int> l1, l2;
    int a = 0;
    int count = 0;
    while (InputFile >> a) {
        if (count % 2 == 0) {
            l1.push_back(a);
        } else {
            l2.push_back(a);
        }
        count++;
    }

    InputFile.close();
    cout << "count: " << count << " size: " << l1.size() + l2.size() << "\n";
    
    sort(l1.begin(), l1.end());
    sort(l2.begin(), l2.end());

    int sum = 0;
    for (auto it1 = l1.begin(), it2 = l2.begin() ; it1 != l1.end() && it2 != l2.end(); ++it1, ++it2) {
        sum += abs(*it1 - *it2);
    }
    
    cout << "Part 1 - Sum: " << sum << "\n";

    map<int, int> l2count;
    for (auto it2 = l2.begin() ; it2 != l2.end(); ++it2) {
        if (l2count.count(*it2) > 0) {
            l2count.at(*it2) = l2count.at(*it2) + 1;
        } else {
            l2count[*it2] = 1;
        }
    }
    int sum2 = 0;
    for (int val : l1) {
        if (l2count.count(val) > 0) {
            sum2 += val * l2count.at(val);
        }
    }

    cout << "Part 2 - Sum: " << sum2 << "\n";

    return 0;
}
