#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
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

int countXmas(vector<string> matrix, int row, int col) {
    int directions[8][2] = {{0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}};
    char target[4] = {'X', 'M', 'A', 'S'};
    int count = 0;
    for (int k = 0; k < 8; k++) {
        int i = row;
        int j = col;
        int targetIndex = 0;
        while (i >= 0 && i < matrix.size() && j >= 0 && j < matrix[i].length() && targetIndex < 4 && matrix[i][j] == target[targetIndex]) {
            i += directions[k][1];
            j += directions[k][0];
            targetIndex++;
        }
        if (targetIndex == 4) {
            count++;
        }
    }
    return count;
}

bool isXmasPart2(vector<string> matrix, int row, int col) {
    int count = 0;
    if (row == 0 || row == matrix.size()-1 || col == 0 || col == matrix[row].length()-1) {
        return false;
    }
    if ((matrix[row-1][col-1] == 'M' && matrix[row+1][col+1] == 'S')
        || (matrix[row-1][col-1] == 'S' && matrix[row+1][col+1] == 'M')) {
        count++;
    }
    if ((matrix[row+1][col-1] == 'M' && matrix[row-1][col+1] == 'S')
        || (matrix[row+1][col-1] == 'S' && matrix[row-1][col+1] == 'M')) {
        count++;
    }
    return count == 2;
}

int main()
{
    vector<string> lines = readLines("data/input04.txt");
    int count = 0;
    int count2 = 0;
    for (int i = 0; i < lines.size(); i++) {
        for (int j = 0; j < lines[j].length(); j++) {
            if (lines[i][j] == 'X') {
                count += countXmas(lines, i, j);
            } else if (lines[i][j] == 'A' && isXmasPart2(lines, i, j)) {
                count2++;
            }
        }
    }

    cout << "Part 1 - count: " << count << endl;
    cout << "Part 2 - count2: " << count2 << endl;
}
