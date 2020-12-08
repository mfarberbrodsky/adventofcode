#include <iostream>
#include <fstream>
#include <regex>
#include <cassert>
#include <vector>
#include <unordered_set>

using namespace std;

typedef struct
{
    string operation;
    int argument;
} instruction;

vector<instruction> parse_input(string filename)
{
    vector<instruction> program;

    regex instruction_regex("(acc|jmp|nop) ([+-]\\d+)");

    ifstream input_file;
    input_file.open(filename);

    string line;
    while (getline(input_file, line))
    {
        smatch match;
        regex_match(line, match, instruction_regex);
        program.push_back({match[1], stoi(match[2])});
    }

    input_file.close();

    return program;
}

typedef struct
{
    bool terminated;
    int acc;
} program_result;

// Run program, return termination status (true if terminated, false if loop) and accumulator value at termination or before repeating a line
program_result run(vector<instruction> &program)
{
    int acc = 0, ip = 0;
    unordered_set<int> lines_seen;

    while (lines_seen.find(ip) == lines_seen.end())
    {
        lines_seen.insert(ip);
        if (ip == program.size())
        {
            return {true, acc};
        }
        instruction inst = program[ip];
        if (inst.operation == "acc")
        {
            acc += inst.argument;
            ip += 1;
        }
        if (inst.operation == "jmp")
        {
            ip += inst.argument;
        }
        if (inst.operation == "nop")
        {
            ip += 1;
        }
    }

    return {false, acc};
}

// Part I Solution
int acc_before_loop(vector<instruction> &program)
{
    program_result result = run(program);
    return result.acc;
}

// Part II Solution
int acc_after_correction(vector<instruction> &program)
{
    for (int line = 0; line < program.size(); line++)
    {
        if (program[line].operation == "nop")
        {
            program[line].operation = "jmp";
        }
        else if (program[line].operation == "jmp")
        {
            program[line].operation = "nop";
        }
        else
        {
            continue;
        }

        program_result result = run(program);
        if (result.terminated)
        {
            return result.acc;
        }

        if (program[line].operation == "nop")
        {
            program[line].operation = "jmp";
        }
        else if (program[line].operation == "jmp")
        {
            program[line].operation = "nop";
        }
    }

    return -1;
}

int main()
{
    vector<instruction> example_program = parse_input("example.txt");
    assert(acc_before_loop(example_program) == 5);
    assert(acc_after_correction(example_program) == 8);

    vector<instruction> program = parse_input("input.txt");
    cout << acc_before_loop(program) << endl;
    cout << acc_after_correction(program) << endl;

    return 0;
}