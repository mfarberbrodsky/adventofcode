#include <iostream>
#include <cassert>
#include <fstream>
#include <vector>

using namespace std;

vector<string> parse_input(string filename)
{
    vector<string> seats;

    ifstream input_file;
    input_file.open(filename);

    string line;
    while (getline(input_file, line))
    {
        seats.push_back(line);
    }

    input_file.close();

    return seats;
}

// Given a string representation of a seat (i.e. a boarding pass), return its seat id.
int seat_id(string seat)
{
    int rowMin = 0, rowMax = 127;
    int colMin = 0, colMax = 7;

    for (int i = 0; i <= 6; i++)
    {
        if (seat[i] == 'F')
        {
            rowMax -= (rowMax - rowMin + 1) / 2;
        }
        else if (seat[i] == 'B')
        {
            rowMin += (rowMax - rowMin + 1) / 2;
        }
    }

    for (int i = 7; i <= 9; i++)
    {
        if (seat[i] == 'L')
        {
            colMax -= (colMax - colMin + 1) / 2;
        }
        else if (seat[i] == 'R')
        {
            colMin += (colMax - colMin + 1) / 2;
        }
    }

    assert(rowMin == rowMax);
    assert(colMin == colMax);

    return rowMin * 8 + colMin;
}

// Part I Solution
int highest_seat_id(vector<string> &seats)
{
    int max = 0;
    for (string seat : seats)
    {
        int id = seat_id(seat);
        if (id > max)
        {
            max = id;
        }
    }
    return max;
}

// Part II Solution
int missing_seat_id(vector<string> &seats)
{
    bool ids[1024] = {}; // there are 128*8=1024 possible seats

    for (string seat : seats)
    {
        int id = seat_id(seat);
        ids[id] = true;
    }

    for (int i = 1; i < 1023; i++)
    {
        if (ids[i - 1] && !ids[i] && ids[i + 1])
        {
            return i;
        }
    }

    return -1;
}

int main()
{
    assert(seat_id("FBFBBFFRLR") == 357);

    vector<string> seats = parse_input("input.txt");
    cout << highest_seat_id(seats) << endl;
    cout << missing_seat_id(seats) << endl;
}