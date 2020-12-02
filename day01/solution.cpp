// Puzzle: https://adventofcode.com/2020/day/1

#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <cassert>
using namespace std;

vector<int> parse_input(string filename)
{
    ifstream input_file;
    input_file.open(filename);

    vector<int> report;
    while (!input_file.eof())
    {
        int num;
        input_file >> num;
        report.push_back(num);
    }

    input_file.close();

    return report;
}

// Part I Solution
// Time complexity: O(n) on average, where n is the number of entries
int product_of_2020_sum_pair(vector<int> &report)
{
    unordered_set<int> seen;
    for (int entry : report)
    {
        if (seen.count(2020 - entry))
        {
            return entry * (2020 - entry);
        }
        seen.insert(entry);
    }

    return -1;
}

// Part II Solution
// Time complexity: O(n^2) on average, where n is the number of entries
int product_of_2020_sum_triplet(vector<int> &report)
{
    for (int entry1 : report)
    {
        int remaining_sum = 2020 - entry1;

        unordered_set<int> seen;
        for (int entry2 : report)
        {
            if (seen.count(2020 - entry1 - entry2))
            {
                return entry1 * entry2 * (2020 - entry1 - entry2);
            }
            seen.insert(entry2);
        }
    }

    return -1;
}

int main()
{
    auto example_report = parse_input("example.txt");
    auto report = parse_input("input.txt");

    assert(product_of_2020_sum_pair(example_report) == 514579);
    cout << product_of_2020_sum_pair(report) << endl;

    assert(product_of_2020_sum_triplet(example_report) == 241861950);
    cout << product_of_2020_sum_triplet(report) << endl;

    return 0;
}