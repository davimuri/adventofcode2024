#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include "utils.cpp"

using namespace std;

struct Computer {
    long regA, regB, regC;
    vector<int> program;
    int ptr;
    vector<int> out;
};

long getRegister(string line) {
    return stol(line.substr(12));
}

vector<int> getProgram(string line) {
    vector<int> prog;
    auto tokens = tokenize(line.substr(9), ',');
    for (string t : tokens) {
        prog.push_back(stoi(t));
    }
    return prog;
}

int opCode(Computer* computer) {
    return computer->program[computer->ptr];
}

long comboOperand(Computer* computer) {
    long value = computer->program[computer->ptr + 1];
    if (value >= 0 && value <= 3) {
        return value;
    }
    if (value == 4) {
        return computer->regA;
    }
    if (value == 5) {
        return computer->regB;
    }
    if (value == 6) {
        return computer->regC;
    }
    return -1;
}

long literalOperand(Computer* computer) {
    return computer->program[computer->ptr+1];
}

void adv(Computer* computer) {
    computer->regA = computer->regA / pow(2, comboOperand(computer));
}

void bxl(Computer* computer) {
    computer->regB = computer->regB ^ literalOperand(computer);
}

void bst(Computer* computer) {
    computer->regB = comboOperand(computer) % 8;
}

bool jnz(Computer* computer) {
    if (computer->regA == 0) {
        return false;
    }
    computer->ptr = literalOperand(computer);
    return true;
}

void bxc(Computer* computer) {
    computer->regB = computer->regB ^ computer->regC;
}

void out(Computer* computer) {
    computer->out.push_back(comboOperand(computer) % 8);
}

void bdv(Computer* computer) {
    computer->regB = computer->regA / pow(2, comboOperand(computer));
}

void cdv(Computer* computer) {
    computer->regC = computer->regA / pow(2, comboOperand(computer));
}

string execute(Computer* computer) {
    while (computer->ptr < computer->program.size()) {
        switch(opCode(computer)) {
            case 0:
                adv(computer);
                computer->ptr += 2;
                break;
            case 1:
                bxl(computer);
                computer->ptr += 2;
                break;
            case 2:
                bst(computer);
                computer->ptr += 2;
                break;
            case 3:
                if (!jnz(computer)) {
                    computer->ptr += 2;
                }
                break;
            case 4:
                bxc(computer);
                computer->ptr += 2;
                break;
            case 5:
                out(computer);
                computer->ptr += 2;
                break;
            case 6:
                bdv(computer);
                computer->ptr += 2;
                break;
            case 7:
                cdv(computer);
                computer->ptr += 2;
                break;
        }
    }
    string output = "";
    for (int v : computer->out) {
        output += to_string(v) + ",";
    }
    output = output.substr(0, output.length()-1);
    return output;
}

// Based on: https://www.reddit.com/r/adventofcode/comments/1hhtc6g/2024_day_17_part_2_explanation_of_the_solution/
// 2,4, 1,1, 7,5, 4,4, 1,4, 0,3, 5,5, 3,0
// 2. B = A % 8
// 1. B = B ^ 1
// 7. C = A / pow(2,B)
// 4. B = B ^ C
// 1. B = B ^ 4
// 0. A = A / pow(2,3)
// 5. out B % 8
// 3. if A != 0 jump to 0
void testPart2(Computer* computer) {
    int d1 = 0;
    for (int i = 0; i < 8; i++) {
        computer->regA = i;
        computer->regB = 0;
        computer->regC = 0;
        computer->ptr = 0;
        computer->out.clear();
        if (stoi(execute(computer)) == computer->program.back()) {
            d1 = i;
            break;
        }
    }
    cout << "Found d1 " << d1 << endl;
    int d2 = 0;
    for (int i = 0; i < 8; i++) {
        computer->regA = d1 * 8 + i;
        computer->regB = 0;
        computer->regC = 0;
        computer->ptr = 0;
        computer->out.clear();
        if (stoi(execute(computer)) == computer->program[computer->program.size()-2]) {
            d2 = i;
            break;
        }
    }
    int d1d2 = d1 * 8 + d2;
    cout << "Found d2 " << d2 << " whole number " << d1d2 << endl;
    int d3 = -1;
    for (int i = 0; i < 8; i++) {
        computer->regA = d1d2 * 8 + i;
        computer->regB = 0;
        computer->regC = 0;
        computer->ptr = 0;
        computer->out.clear();
        if (stoi(execute(computer)) == computer->program[computer->program.size()-3]) {
            d3 = i;
            break;
        }
    }
    int d1d2d3 = d1d2 * 8 + d3;
    cout << "Found d3 " << d3 << " whole number " << d1d2d3 << endl;
    
}

bool outputSameThatProgram(Computer* computer, int programIdx) {
    for (int o : computer->out) {
        if (o != computer->program[programIdx]) {
            return false;
        }
        programIdx++;
    }
    return true;
}

long check(Computer* computer, long regA, int programIdx) {
    if (programIdx < 0) {
        return regA;
    }
    for (int i = 0; i < 8; i++) {
        long nextRegA = regA * 8 + i;
        computer->regA = nextRegA;
        computer->regB = 0;
        computer->regC = 0;
        computer->ptr = 0;
        computer->out.clear();
        execute(computer);
        if (outputSameThatProgram(computer, programIdx)) {
            long solution = check(computer, nextRegA, programIdx-1);
            if (solution != -1) {
                return solution;
            }
        }
    }
    return -1;
}

int main() {
    vector<string> lines = readLines("data/input17.txt");
    Computer computer;
    computer.regA = getRegister(lines[0]);
    computer.regB = getRegister(lines[1]);
    computer.regC = getRegister(lines[2]);
    computer.program = getProgram(lines[4]);
    computer.ptr = 0;
    cout << "A " << computer.regA << endl;
    cout << "B " << computer.regB << endl;
    cout << "C " << computer.regC << endl;
    cout << "Program ";
    for (int c : computer.program) {
        cout << c << ", ";
    }
    cout << endl;
    auto t1 = chrono::high_resolution_clock::now();
    string outputPart1 = execute(&computer);
    auto t2 = chrono::high_resolution_clock::now();
    auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 1 - output: " << outputPart1 << " Duration (ms): " << ms_int.count() << endl;

    t1 = chrono::high_resolution_clock::now();
    long solutionPart2 = check(&computer, 0, computer.program.size()-1);
    // solvePart2(&computer);
    t2 = chrono::high_resolution_clock::now();
    ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 2 - solution: " << solutionPart2 << " Duration (ms): " << ms_int.count() << endl;

    return 0;
}
