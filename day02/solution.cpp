// Puzzle: https://adventofcode.com/2020/day/2

#include <fstream>
#include <iostream>
#include <vector>
#include <tuple>
#include <cassert>
using namespace std;

vector<tuple<int, int, char, string>> parse_input(string filename)
{
    ifstream input_file;
    input_file.open(filename);

    vector<tuple<int, int, char, string>> passwords;
    while (!input_file.eof())
    {
        int num1, num2;
        char dash, colon;
        char character;
        string password;

        input_file >> num1 >> dash >> num2 >> character >> colon >> password;
        passwords.push_back(make_tuple(num1, num2, character, password));
    }

    input_file.close();

    return passwords;
}

// Part I Solution
// Time complexity: O(n) where n is the total length of all passwords
int num_valid_passwords_count(vector<tuple<int, int, char, string>> &passwords)
{
    int result = 0;
    for (auto pass : passwords)
    {
        int low, high;
        char character;
        string password;

        tie(low, high, character, password) = pass;

        int count = 0;
        for (char c : password)
        {
            if (c == character)
            {
                count++;
            }
        }
        if (low <= count && count <= high)
        {
            result++;
        }
    }

    return result;
}

// Part II Solution
// Time complexity: O(n) where n is the number of passwords
int num_valid_passwords_pos(vector<tuple<int, int, char, string>> &passwords)
{
    int result = 0;
    for (auto pass : passwords)
    {
        int pos1, pos2;
        char character;
        string password;

        tie(pos1, pos2, character, password) = pass;

        if ((password[pos1 - 1] == character) != (password[pos2 - 1] == character))
        {
            result++;
        }
    }

    return result;
}

int main()
{
    auto example_passwords = parse_input("example.txt");
    auto passwords = parse_input("input.txt");

    assert(num_valid_passwords_count(example_passwords) == 2);
    cout << num_valid_passwords_count(passwords) << endl;

    assert(num_valid_passwords_pos(example_passwords) == 1);
    cout << num_valid_passwords_pos(passwords) << endl;

    return 0;
}