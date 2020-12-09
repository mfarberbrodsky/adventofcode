#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <unordered_set>

using namespace std;

vector<long> parse_input(string filename)
{
    vector<long> numbers;

    ifstream input_file;
    input_file.open(filename);

    while (!input_file.eof())
    {
        long num;
        input_file >> num;
        numbers.push_back(num);
    }

    input_file.close();

    return numbers;
}

// Given the previous n numbers, return true iff there is a pair that sum to sum
bool is_sum_pair(vector<long> previous, long sum)
{
    unordered_set<long> seen;
    for (long num : previous)
    {
        if (seen.count(sum - num))
        {
            return true;
        }
        seen.insert(num);
    }

    return false;
}

// Part I Solution
long first_not_sum(vector<long> numbers, int preamble_size)
{
    vector<long> previous;
    for (int i = 0; i < numbers.size(); i++)
    {
        long num = numbers[i];
        if (i < preamble_size)
        {
            previous.push_back(num);
        }
        else
        {
            if (!is_sum_pair(previous, num))
            {
                return num;
            }
            previous.erase(previous.begin());
            previous.push_back(num);
        }
    }

    return -1;
}

// Part II Solution
long contiguous_subsequence(vector<long> numbers, long sum)
{
    long curr_sum = numbers[0];
    int start_index = 0, end_index = 0;

    int i = 0;
    while (start_index <= end_index)
    {
        if (curr_sum < sum && end_index < numbers.size() - 1)
        {
            end_index += 1;
            curr_sum += numbers[end_index];
        }
        else if (curr_sum == sum)
        {
            long min = numbers[start_index], max = numbers[start_index];
            for (int i = start_index; i <= end_index; i++)
            {
                if (numbers[i] < min)
                {
                    min = numbers[i];
                }
                if (numbers[i] > max)
                {
                    max = numbers[i];
                }
            }
            return min + max;
        }
        else if (curr_sum > sum)
        {
            curr_sum -= numbers[start_index];
            start_index += 1;
        }
    }

    return -1;
}

int main()
{
    vector<long> example_numbers = parse_input("example.txt");
    assert(first_not_sum(example_numbers, 5) == 127);
    assert(contiguous_subsequence(example_numbers, 127) == 62);

    vector<long> numbers = parse_input("input.txt");
    int weak_number = first_not_sum(numbers, 25);
    cout << weak_number << endl;
    cout << contiguous_subsequence(numbers, weak_number) << endl;
    return 0;
}