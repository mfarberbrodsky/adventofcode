#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <complex.h>

#define MAX_SIZE 1024
#define MAX_LINE 8

typedef struct
{
    enum
    {
        NORTH = 'N',
        SOUTH = 'S',
        EAST = 'E',
        WEST = 'W',
        LEFT = 'L',
        RIGHT = 'R',
        FORWARD = 'F'
    } action;
    int value;
} instruction;

int parse_input(char *filename, instruction *insts)
{
    int n = 0;

    FILE *input_file = fopen(filename, "r");
    char line[MAX_LINE];
    while (fgets(line, MAX_SIZE, input_file) != NULL)
    {
        insts[n].action = line[0];
        insts[n].value = atoi(line + 1);
        n++;
    }
    fclose(input_file);

    return n;
}

// we represent directions with complex numbers:
// real component is east <-> west where positive is east.
// imaginary component is south <-> north where positive is north.
complex degree_to_direction(int degree)
{
    switch (degree)
    {
    case 0:
        return 1;
    case 90:
        return I;
    case 180:
        return -1;
    case 270:
        return -I;
    }
}

// Part I solution
int ship_distance(instruction *insts, int length)
{
    complex loc = 0;
    complex dir = 1;

    for (int i = 0; i < length; i++)
    {
        switch (insts[i].action)
        {
        case NORTH:
            loc += insts[i].value * I;
            break;
        case SOUTH:
            loc -= insts[i].value * I;
            break;
        case EAST:
            loc += insts[i].value;
            break;
        case WEST:
            loc -= insts[i].value;
            break;
        case LEFT:
            dir *= degree_to_direction(insts[i].value);
            break;
        case RIGHT:
            dir *= degree_to_direction(360 - insts[i].value);
            break;
        case FORWARD:
            loc += dir * insts[i].value;
            break;
        }
    }

    return abs(creal(loc)) + abs(cimag(loc));
}

// Part II solution
int ship_distance_with_waypoint(instruction *insts, int length)
{
    complex ship_loc = 0;
    complex waypoint_loc = 10 + I;

    for (int i = 0; i < length; i++)
    {
        switch (insts[i].action)
        {
        case NORTH:
            waypoint_loc += insts[i].value * I;
            break;
        case SOUTH:
            waypoint_loc -= insts[i].value * I;
            break;
        case EAST:
            waypoint_loc += insts[i].value;
            break;
        case WEST:
            waypoint_loc -= insts[i].value;
            break;
        case LEFT:
            waypoint_loc *= degree_to_direction(insts[i].value);
            break;
        case RIGHT:
            waypoint_loc *= degree_to_direction(360 - insts[i].value);
            break;
        case FORWARD:
            ship_loc += waypoint_loc * insts[i].value;
            break;
        }
    }

    return abs(creal(ship_loc)) + abs(cimag(ship_loc));
}

int main()
{
    instruction example_insts[MAX_SIZE];
    int example_length = parse_input("example.txt", example_insts);
    assert(ship_distance(example_insts, example_length) == 25);
    assert(ship_distance_with_waypoint(example_insts, example_length) == 286);

    instruction insts[MAX_SIZE];
    int length = parse_input("input.txt", insts);
    printf("%d\n", ship_distance(insts, length));
    printf("%d\n", ship_distance_with_waypoint(insts, length));

    return 0;
}