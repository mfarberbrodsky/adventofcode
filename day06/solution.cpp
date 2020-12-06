#include <iostream>
#include <cassert>
#include <fstream>
#include <vector>

using namespace std;

vector<vector<string>> parse_input(string filename)
{
    vector<vector<string>> groups;

    ifstream input_file;
    input_file.open(filename);

    string line;
    vector<string> group;
    while (getline(input_file, line))
    {
        if (line.length() == 0)
        {
            groups.push_back(group);
            group.clear();
        }
        else
        {
            group.push_back(line);
        }
    }
    groups.push_back(group);

    input_file.close();

    return groups;
}

// Part I Solution
int sum_any_questions(vector<vector<string>> groups)
{
    int sum = 0;
    for (vector<string> group : groups)
    {
        // questions[i] is true iff anyone answered yes on the question with the ith letter
        bool questions[26] = {};
        for (string person : group)
        {
            for (char ch : person)
            {
                questions[ch - 'a'] = true;
            }
        }

        for (bool question : questions)
        {
            if (question)
            {
                sum++;
            }
        }
    }
    return sum;
}

// Part II Solution
int sum_all_questions(vector<vector<string>> groups)
{
    int sum = 0;
    for (vector<string> group : groups)
    {
        // questions[i] is the number of people who answered yes on the question with the ith letter
        int questions[26] = {};
        for (string person : group)
        {
            for (char ch : person)
            {
                questions[ch - 'a']++;
            }
        }

        for (int question : questions)
        {
            if (question == group.size())
            {
                sum++;
            }
        }
    }
    return sum;
}

int main()
{
    auto example_groups = parse_input("example.txt");
    auto groups = parse_input("input.txt");

    assert(sum_any_questions(example_groups) == 11);
    cout << sum_any_questions(groups) << endl;

    assert(sum_all_questions(example_groups) == 6);
    cout << sum_all_questions(groups) << endl;
}