// Puzzle: https://adventofcode.com/2020/day/3

#include <fstream>
#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

vector<string> parse_input(string filename)
{
    ifstream input_file;
    input_file.open(filename);

    vector<string> map;
    while (!input_file.eof())
    {
        string line;
        input_file >> line;
        map.push_back(line);
    }

    input_file.close();

    return map;
}

// Part I Solution
// Time complexity: O(n/slopeY) where n is the height of the map
long trees_on_slope(vector<string> &map, int slopeX, int slopeY)
{
    long result = 0;
    int posX = 0, posY = 0;
    for (auto line : map)
    {
        posX = (posX + slopeX) % line.length();
        posY = posY + slopeY;

        if (posY >= map.size())
        {
            return result;
        }

        if (map[posY][posX] == '#')
        {
            result++;
        }
    }

    return result;
}

// Part II Solution
// Time complexity: O(n) where n is the height of the map
long trees_on_slopes_product(vector<string> &map)
{
    return trees_on_slope(map, 1, 1) * trees_on_slope(map, 3, 1) * trees_on_slope(map, 5, 1) * trees_on_slope(map, 7, 1) * trees_on_slope(map, 1, 2);
}

int main()
{
    auto example_map = parse_input("example.txt");
    auto map = parse_input("input.txt");

    assert(trees_on_slope(example_map, 3, 1) == 7);
    cout << trees_on_slope(map, 3, 1) << endl;

    assert(trees_on_slopes_product(example_map) == 336);
    cout << trees_on_slopes_product(map) << endl;

    return 0;
}