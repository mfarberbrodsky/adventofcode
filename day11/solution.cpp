#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <tuple>

using namespace std;

vector<vector<char>> parse_input(string filename)
{
    vector<vector<char>> layout;

    ifstream input_file;
    input_file.open(filename);

    string line;
    while (getline(input_file, line))
    {
        vector<char> row;
        for (char ch : line)
        {
            row.push_back(ch);
        }
        layout.push_back(row);
    }

    input_file.close();

    return layout;
}

// Generic step function, with some threshold and counting function
tuple<bool, vector<vector<char>>> step(vector<vector<char>> &layout, int threshold, int (*num_occupied)(vector<vector<char>> &, int, int))
{
    bool changed = false;
    vector<vector<char>> new_layout;

    for (int i = 0; i < layout.size(); i++)
    {
        vector<char> row;
        for (int j = 0; j < layout[i].size(); j++)
        {
            int num = num_occupied(layout, i, j);
            if (layout[i][j] == 'L' && num == 0)
            {
                row.push_back('#');
                changed = true;
            }
            else if (layout[i][j] == '#' && num >= threshold)
            {
                row.push_back('L');
                changed = true;
            }
            else
            {
                row.push_back(layout[i][j]);
            }
        }
        new_layout.push_back(row);
    }
    return {changed, new_layout};
}

// Part I specific functions
int num_occupied_adjacent(vector<vector<char>> &layout, int row, int col)
{
    int num = 0;
    for (int i = max(row - 1, 0); i <= row + 1 && i < layout.size(); i++)
    {
        for (int j = max(col - 1, 0); j <= col + 1 && j < layout[i].size(); j++)
        {
            if (!(i == row && j == col) && layout[i][j] == '#')
            {
                num++;
            }
        }
    }
    return num;
}

tuple<bool, vector<vector<char>>> step_by_adjacent(vector<vector<char>> &layout)
{
    return step(layout, 4, num_occupied_adjacent);
}

// Part II specific functions
int num_occupied_direction(vector<vector<char>> &layout, int row, int col)
{
    int num = 0;
    for (int dirX = -1; dirX <= 1; dirX++)
    {
        for (int dirY = -1; dirY <= 1; dirY++)
        {
            if (dirX == 0 && dirY == 0)
            {
                continue;
            }

            int x = row + dirX, y = col + dirY;
            bool found = false;
            while (!found && 0 <= x && x < layout.size() && 0 <= y && y < layout[0].size())
            {
                if (layout[x][y] == 'L')
                {
                    found = true;
                }
                if (layout[x][y] == '#')
                {
                    num++;
                    found = true;
                }

                x += dirX;
                y += dirY;
            }
        }
    }
    return num;
}

tuple<bool, vector<vector<char>>> step_by_direction(vector<vector<char>> &layout)
{
    return step(layout, 5, num_occupied_direction);
}

// Generic solution to both parts, given the appropriate step function
int num_occupied_at_equilibrium(vector<vector<char>> &layout, tuple<bool, vector<vector<char>>> step_func(vector<vector<char>> &))
{
    bool changed;
    vector<vector<char>> temp_layout;
    tie(changed, temp_layout) = step_func(layout);
    while (changed)
    {
        tie(changed, temp_layout) = step_func(temp_layout);
    }

    int num = 0;
    for (vector<char> row : temp_layout)
    {
        for (char seat : row)
        {
            if (seat == '#')
            {
                num++;
            }
        }
    }
    return num;
}

int main()
{
    vector<vector<char>> example_layout = parse_input("example.txt");
    assert(num_occupied_at_equilibrium(example_layout, step_by_adjacent) == 37);
    assert(num_occupied_at_equilibrium(example_layout, step_by_direction) == 26);

    vector<vector<char>> layout = parse_input("input.txt");
    cout << num_occupied_at_equilibrium(layout, step_by_adjacent) << endl;
    cout << num_occupied_at_equilibrium(layout, step_by_direction) << endl;

    return 0;
}