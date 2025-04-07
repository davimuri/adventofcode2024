#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include <deque>
#include "utils.cpp"

using namespace std;

struct StoneCount {
    long stone;
    string stoneStr;
    long count;

    StoneCount(string st, long c) {
        stoneStr = st;
        stone = stol(st);
        count = c;
    }
};

long blinkStonesSmarter(vector<string> stones, int blinks) {
    deque<StoneCount> queue;
    map<string, long> stoneCount;
    for (string st: stones) {
        long prevCount = stoneCount.count(st) == 0 ? 0 : stoneCount[st];
        stoneCount[st] = prevCount + 1;
    }
    for (const auto& entry : stoneCount) {
        queue.push_back(StoneCount(entry.first, entry.second));
    }
    stoneCount.clear();
    int currentBlink = 1;
    while (!queue.empty() && currentBlink <= blinks) {
        StoneCount currentStone = queue.front();
        queue.pop_front();

        if (currentStone.stone == 0) {
            long prevCount = stoneCount.count("1") == 0 ? 0 : stoneCount["1"];
            stoneCount["1"] = prevCount + currentStone.count;
        } else if (currentStone.stoneStr.length() % 2 == 0) {
            int middle = currentStone.stoneStr.length() / 2;
            string halfStone1 = currentStone.stoneStr.substr(0, middle);
            long prevCount = stoneCount.count(halfStone1) == 0 ? 0 : stoneCount[halfStone1];
            stoneCount[halfStone1] = prevCount + currentStone.count;
            string halfStone2 = currentStone.stoneStr.substr(middle, middle);
            halfStone2 = halfStone2.erase(0, min(halfStone2.find_first_not_of('0'), halfStone2.size()-1));
            prevCount = stoneCount.count(halfStone2) == 0 ? 0 : stoneCount[halfStone2];
            stoneCount[halfStone2] = prevCount + currentStone.count;
        } else {
            long newVal = currentStone.stone * 2024;
            string stone2024 = to_string(newVal);
            long prevCount = stoneCount.count(stone2024) == 0 ? 0 : stoneCount[stone2024];
            stoneCount[stone2024] = prevCount + currentStone.count;
        }

        if (queue.empty()) {
            for (const auto& entry : stoneCount) {
                queue.push_back(StoneCount(entry.first, entry.second));
            }
            stoneCount.clear();
            currentBlink++;
        }

    }
    long finalStoneCount = 0;
    while (!queue.empty()) {
        StoneCount currentStone = queue.front();
        queue.pop_front();
        finalStoneCount += currentStone.count;
    }
    return finalStoneCount;
}

vector<string> blinkStone(string stone, int blinks, map<string, map<int, vector<string>>> *cache) {
    if (blinks == 0) {
        vector<string> empty;
        empty.push_back(stone);
        return empty;
    }
    int blinksToDo = blinks;
    vector<string> stonesToDo;
    if ((*cache).count(stone) > 0) {
        int blinksFound = blinks;
        while (blinksFound > 0 && (*cache)[stone].count(blinksFound) == 0) {
            blinksFound--;
        }
        if (blinksFound > 0) {
            blinksToDo = blinks - blinksFound;
            stonesToDo = (*cache)[stone][blinksFound];
        } else {
            vector<string> sameStoneVec;
            sameStoneVec.push_back(stone);
            stonesToDo = move(sameStoneVec);
        }
    } else {
        vector<string> sameStoneVec;
        sameStoneVec.push_back(stone);
        stonesToDo = move(sameStoneVec);
    }
    if (blinksToDo == 0) {
        return stonesToDo;
    }
    vector<string> newStones;
    for (string st : stonesToDo) {
        if (st == "0") {
            newStones.push_back("1");
        } else if (st.length() % 2 == 0) {
            int middle = st.length() / 2;
            string halfStone1 = st.substr(0, middle);
            newStones.push_back(halfStone1);
            long val = stol(st.substr(middle, middle));
            string halfStone2 = to_string(val);
            newStones.push_back(halfStone2);
        } else {
            long newVal = stol(st) * 2024;
            string stone2024 = to_string(newVal);
            newStones.push_back(stone2024);
        }
    }
    vector<string> result;
    for (string newSt : newStones) {
        vector<string> resStones = blinkStone(newSt, blinksToDo-1, cache);
        for (string e : resStones) {
            result.push_back(e);
        }
    }
    if ((*cache).count(stone) == 0) {
        map<int, vector<string>> entry;
        (*cache)[stone] = entry;
    }
    (*cache)[stone][blinks] = result;
    return result;
}

int solvePart1Naive(vector<string> stones, int blinks) {
    map<string, int> frequency;
    vector<string> *currentStones = &stones;
    for (int i = 1; i <= blinks; i++) {
        vector<string> newStones;
        for (string stone : *currentStones) {
            if (stone == "0") {
                newStones.push_back("1");
                if (frequency.count("1") == 0) {
                    frequency["1"] = 1;
                } else {
                    frequency["1"] = frequency["1"] + 1;
                }
            } else if (stone.length() % 2 == 0) {
                int middle = stone.length() / 2;
                string halfStone1 = stone.substr(0, middle);
                newStones.push_back(halfStone1);
                long val = stol(stone.substr(middle, middle));
                string halfStone2 = to_string(val);
                newStones.push_back(halfStone2);

                if (frequency.count(halfStone1) == 0) {
                    frequency[halfStone1] = 1;
                } else {
                    frequency[halfStone1] = frequency[halfStone1] + 1;
                }
                if (frequency.count(halfStone2) == 0) {
                    frequency[halfStone2] = 1;
                } else {
                    frequency[halfStone2] = frequency[halfStone2] + 1;
                }

            } else {
                long newVal = stol(stone) * 2024;
                string stone2024 = to_string(newVal);
                newStones.push_back(stone2024);
                if (frequency.count(stone2024) == 0) {
                    frequency[stone2024] = 1;
                } else {
                    frequency[stone2024] = frequency[stone2024] + 1;
                }
            }
        }
        *currentStones = newStones;
    }

    return (*currentStones).size();
}

int main() {
    vector<string> lines = readLines("data/input11.txt");
    vector<string> stones = tokenize(lines[0], ' ');
    auto t1 = chrono::high_resolution_clock::now();
    int stonesCount = solvePart1Naive(stones, 25);
    auto t2 = chrono::high_resolution_clock::now();
    auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 1 naive - stones: " << stonesCount << " Duration (ms): " << ms_int.count() << endl;

    t1 = chrono::high_resolution_clock::now();
    stonesCount = 0;
    map<string, map<int, vector<string>>> cache;
    for (string st : stones) {
        stonesCount += blinkStone(st, 25, &cache).size();
    }
    t2 = chrono::high_resolution_clock::now();
    ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 1 dfs with memo - stones: " << stonesCount << " Duration (ms): " << ms_int.count() << endl;

    t1 = chrono::high_resolution_clock::now();
    long stonesCount1c = blinkStonesSmarter(stones, 25);
    t2 = chrono::high_resolution_clock::now();
    ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 1 smarter - stones: " << stonesCount1c << " Duration (ms): " << ms_int.count() << endl;

    t1 = chrono::high_resolution_clock::now();
    long stonesCount2 = blinkStonesSmarter(stones, 75);
    t2 = chrono::high_resolution_clock::now();
    ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 2 smarter - stones: " << stonesCount2 << " Duration (ms): " << ms_int.count() << endl;

    return 0;
}
