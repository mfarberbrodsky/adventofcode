#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>

using namespace std;

vector<int> parse_input(string filename)
{
    vector<int> adapters;

    ifstream input_file;
    input_file.open(filename);

    while (!input_file.eof())
    {
        int adapter;
        input_file >> adapter;
        adapters.push_back(adapter);
    }

    input_file.close();

    return adapters;
}

// Range of elements is 0..3n, so count sort is linear
vector<int> count_sort(vector<int> &array)
{
    int max = 0;
    for (int num : array)
    {
        if (num > max)
        {
            max = num;
        }
    }

    vector<int> count(max + 1, 0);
    vector<int> sorted(array.size());

    for (int i = 0; i < array.size(); i++)
    {
        count[array[i]]++;
    }

    for (int i = 1; i <= max; i++)
    {
        count[i] += count[i - 1];
    }

    for (int i = array.size() - 1; i >= 0; i--)
    {
        sorted[count[array[i]] - 1] = array[i];
        count[array[i]]--;
    }

    return sorted;
}

// Part I Solution
int difference_product(vector<int> &adapters)
{
    vector<int> adapter_chain = count_sort(adapters);

    int difference1 = 1, difference3 = 1;
    for (int i = 1; i < adapter_chain.size(); i++)
    {
        if (adapter_chain[i] - adapter_chain[i - 1] == 1)
        {
            difference1++;
        }
        if (adapter_chain[i] - adapter_chain[i - 1] == 3)
        {
            difference3++;
        }
    }

    return difference1 * difference3;
}

// Part II Solution
long num_arrangements(vector<int> &adapters)
{
    vector<int> sorted_adapters = count_sort(adapters);
    sorted_adapters.insert(sorted_adapters.begin(), 0);

    vector<long> num_arrangements_ending_with(sorted_adapters.size(), 0);
    num_arrangements_ending_with[0] = 1;

    for (int i = 0; i < sorted_adapters.size(); i++)
    {
        if (i - 1 >= 0 && sorted_adapters[i] - sorted_adapters[i - 1] <= 3)
        {
            num_arrangements_ending_with[i] += num_arrangements_ending_with[i - 1];
        }
        if (i - 2 >= 0 && sorted_adapters[i] - sorted_adapters[i - 2] <= 3)
        {
            num_arrangements_ending_with[i] += num_arrangements_ending_with[i - 2];
        }
        if (i - 3 >= 0 && sorted_adapters[i] - sorted_adapters[i - 3] <= 3)
        {
            num_arrangements_ending_with[i] += num_arrangements_ending_with[i - 3];
        }
    }

    return num_arrangements_ending_with[sorted_adapters.size() - 1];
}

int main()
{
    vector<int> example_adapters = parse_input("example.txt");
    assert(difference_product(example_adapters) == 35);
    assert(num_arrangements(example_adapters) == 8);

    vector<int> example2_adapters = parse_input("example2.txt");
    assert(difference_product(example2_adapters) == 220);
    assert(num_arrangements(example2_adapters) == 19208);

    vector<int> adapters = parse_input("input.txt");
    cout << difference_product(adapters) << endl;
    cout << num_arrangements(adapters) << endl;

    return 0;
}