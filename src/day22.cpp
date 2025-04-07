#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include <deque>
#include <tuple>
#include "utils.cpp"

using namespace std;

long nextSecret(long secret) {
    secret = ((secret * 64) ^ secret) % 16777216;
    secret = ((secret / 32) ^ secret) % 16777216;
    secret = ((secret * 2048) ^ secret) % 16777216;
    return secret;
}

long solvePart1(vector<string> lines) {
    long sum = 0;
    for (string l : lines) {
        long secret = stol(l);
        for (int i = 0; i < 2000; i++) {
            secret = nextSecret(secret);
        }
        cout << secret << endl;
        sum += secret;
    }
    return sum;
}

void fillPriceChanges(long secret, map<tuple<int, int, int, int>, int> priceChangesToBananas) {

}

long solvePart2(vector<string> lines) {
    map<tuple<int, int, int, int>, int> priceChangesToBananas;
    for (string l : lines) {
        long secret = stol(l);
        int price = secret % 10;
        deque<int> lastPrices;
        lastPrices.push_back(price);
        set<tuple<int, int, int, int>> priceChangesSeen;
        for (int i = 0; i < 2000; i++) {
            secret = nextSecret(secret);
            int price = secret % 10;
            lastPrices.push_back(price);
            if (lastPrices.size() == 5) {
                tuple<int, int, int, int> priceChanges = make_tuple(
                    lastPrices[1]-lastPrices[0], lastPrices[2]-lastPrices[1],
                    lastPrices[3]-lastPrices[2], lastPrices[4]-lastPrices[3]);
                lastPrices.pop_front();
                if (priceChangesSeen.count(priceChanges) > 0) {
                    continue;
                }
                priceChangesSeen.insert(priceChanges);
                if (priceChangesToBananas[priceChanges] == 0) {
                    priceChangesToBananas[priceChanges] = lastPrices.back();
                } else {
                    priceChangesToBananas[priceChanges] += lastPrices.back();
                }
            }
        }
    }
    int maxBananas = -1;
    for (const auto &entry : priceChangesToBananas) {
        maxBananas = max(maxBananas, entry.second);
    }
    return maxBananas;
}

int main() {
    vector<string> lines = readLines("data/input22.txt");
    auto t1 = chrono::high_resolution_clock::now();
    long sum = solvePart1(lines);
    auto t2 = chrono::high_resolution_clock::now();
    auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 1 - sum: " << sum << " Duration (ms): " << ms_int.count() << endl;
    t1 = chrono::high_resolution_clock::now();
    sum = solvePart2(lines);
    t2 = chrono::high_resolution_clock::now();
    ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 2 - maxBananas: " << sum << " Duration (ms): " << ms_int.count() << endl;
    return 0;
}
