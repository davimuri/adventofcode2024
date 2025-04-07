#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include "utils.cpp"

using namespace std;

const int EMPTY = -1;

vector<int> decompress(string diskMap) {
    vector<int> disk;
    for (int i = 0; i < diskMap.length(); i++) {
        int current = diskMap[i] - '0';
        if (i % 2 == 0) {
            // block file
            for (int j = 1; j <= current; j++) disk.push_back(i/2);
        } else {
            // empty block
            for (int j = 1; j <= current; j++) disk.push_back(EMPTY);
        }
    }
    return disk;
}

vector<int> compactDisk(vector<int> disk) {
    vector<int> compacted = disk;
    int left = 0;
    int right = compacted.size() - 1;
    while (left < right) {
        if (compacted[left] != EMPTY) {
            left++;
            continue;
        }
        if (compacted[right] == EMPTY) {
            right--;
            continue;
        }
        compacted[left] = compacted[right];
        compacted[right] = EMPTY;
    }
    return compacted;
}

vector<int> compactDiskPart2(vector<int> disk) {
    vector<int> compacted = disk;
    int fileToMoveStart = compacted.size() - 1;
    int fileToMoveEnd = compacted.size() - 1;
    while (fileToMoveStart > 0 && fileToMoveEnd > 0) {
        // find start and end of file to move
        while (compacted[fileToMoveEnd] == EMPTY && fileToMoveEnd > 0) {
            fileToMoveEnd--;
        }
        if (fileToMoveEnd <= 0) break;

        int fileId = compacted[fileToMoveEnd];
        fileToMoveStart = fileToMoveEnd;
        while (compacted[fileToMoveStart] == fileId && fileToMoveStart > 0) {
            fileToMoveStart--;
        }
        if (fileToMoveStart <= 0) break;

        if (compacted[fileToMoveStart] != fileId) {
            fileToMoveStart++;
        }
        int fileSize = fileToMoveEnd - fileToMoveStart + 1;

        // find empty space
        int emptyBlockStart = 0;
        int emptyBlockEnd = -1;
        int emptyBlockSize = 0;
        while (emptyBlockSize < fileSize && emptyBlockEnd < fileToMoveStart) {
            emptyBlockStart = emptyBlockEnd + 1;
            while (compacted[emptyBlockStart] != EMPTY && emptyBlockStart < fileToMoveStart) {
                emptyBlockStart++;
            }
            if (emptyBlockStart >= fileToMoveStart) break;

            emptyBlockEnd = emptyBlockStart;
            while (compacted[emptyBlockEnd] == EMPTY && emptyBlockEnd < fileToMoveStart) {
                emptyBlockEnd++;
            }
            if (compacted[emptyBlockEnd] != EMPTY) {
                emptyBlockEnd--;
            }
            emptyBlockSize = emptyBlockEnd - emptyBlockStart + 1;
        }

        // move file
        if (emptyBlockSize >= fileSize && emptyBlockEnd < fileToMoveStart) {
            for (int i = emptyBlockStart; i < emptyBlockStart+fileSize; i++) {
                compacted[i] = fileId;
            }
            for (int i = fileToMoveStart; i <= fileToMoveEnd; i++) {
                compacted[i] = EMPTY;
            }
        }
        fileToMoveEnd = fileToMoveStart - 1;
        fileToMoveStart = fileToMoveEnd;
    }
    return compacted;
}

long checksum(vector<int> disk) {
    long result = 0;
    for (int i = 0; i < disk.size(); i++) {
        if (disk[i] != EMPTY) {
            result += disk[i] * i;
        }
    }
    return result;
}

int main() {
    vector<string> lines = readLines("data/input09.txt");
    auto disk = decompress(lines[0]);
    auto compacted = compactDisk(disk);
    long checksum1 = checksum(compacted);
    auto t1 = chrono::high_resolution_clock::now();
    auto compacted2 = compactDiskPart2(disk);
    long checksum2 = checksum(compacted2);
    auto t2 = chrono::high_resolution_clock::now();
    auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Duration (ms): " << ms_int.count() << endl;
    cout << "Part 1 -  checksum: " << checksum1 << endl;
    cout << "Part 2 -  checksum: " << checksum2 << endl;
    return 0;
}
