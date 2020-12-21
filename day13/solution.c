#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_SIZE 256

typedef struct
{
    int timestamp;
    int num_bus_ids;
    int bus_ids[MAX_SIZE]; // x is stored as 0
} input;

// Receives filename, stores the parsed file information in problem_input
void parse_input(char *filename, input *problem_input)
{
    FILE *input_file = fopen(filename, "r");
    char line[MAX_SIZE];

    fgets(line, MAX_SIZE, input_file);
    problem_input->timestamp = atoi(line);

    int n = 0;
    fgets(line, MAX_SIZE, input_file);
    for (char *token = strtok(line, ","); token != NULL; token = strtok(NULL, ","))
    {
        problem_input->bus_ids[n] = atoi(token);
        n++;
    }
    problem_input->num_bus_ids = n;

    fclose(input_file);
}

// Part I Solution

// Given a timestamp and a bus id, return the wait time for the first bus with the given id, starting at timestamp
int wait_time_for_bus(int timestamp, int bus_id)
{
    return bus_id - timestamp % bus_id;
}

// Given the problem input, return the id of the earliest bus to arrive after timestamp
int earliest_bus(input *problem_input)
{
    int earliest_bus_id = -1;
    for (int i = 0; i < problem_input->num_bus_ids; i++)
    {
        int curr_bus_id = problem_input->bus_ids[i];
        if (curr_bus_id == 0)
        {
            continue;
        }
        if (earliest_bus_id == -1 || wait_time_for_bus(problem_input->timestamp, curr_bus_id) < wait_time_for_bus(problem_input->timestamp, earliest_bus_id))
        {
            earliest_bus_id = curr_bus_id;
        }
    }
    return earliest_bus_id;
}

int bus_id_times_wait_time(input *problem_input)
{
    int earliest_bus_id = earliest_bus(problem_input);
    return earliest_bus_id * wait_time_for_bus(problem_input->timestamp, earliest_bus_id);
}

// Part II Solution
typedef struct
{
    long x;
    long y;
} coeffs;

// Given a, b, use the extended Euclidean algorithm to calculate x, y such that a*x+b*y = gcd(a,b)
coeffs bezout_coefficients(long a, long b)
{
    long r_old = a, r = b;
    long s_old = 1, s = 0;
    long t_old = 0, t = 1;

    while (r != 0)
    {
        long quotient = r_old / r;

        long temp_r_old = r;
        r = r_old - quotient * r;
        r_old = temp_r_old;

        long temp_s_old = s;
        s = s_old - quotient * s;
        s_old = temp_s_old;

        long temp_t_old = t;
        t = t_old - quotient * t;
        t_old = temp_t_old;
    }

    coeffs bezout_coeffs = {s_old, t_old};
    return bezout_coeffs;
}

// Use the chinese remainder theorem to calculate the smallest positive solution to a system of modular congruences
long chinese_remainder(long divisors[], long remainders[], int n)
{
    long N = 1;
    for (int i = 0; i < n; i++)
    {
        N *= divisors[i];
    }

    long sol = 0;
    for (int i = 0; i < n; i++)
    {
        long Ni = N / divisors[i];
        coeffs c = bezout_coefficients(Ni, divisors[i]);
        long Mi = c.x;

        sol += remainders[i] * Mi * Ni;
    }

    // If solution is negative, add sufficient multiples of N
    if (sol >= 0)
    {
        return sol;
    }
    return sol + (-sol / N + 1) * N;
}

long earliest_timestamp(input *problem_input)
{
    long divisors[problem_input->num_bus_ids];
    long remainders[problem_input->num_bus_ids];

    int n = 0;
    for (int i = 0; i < problem_input->num_bus_ids; i++)
    {
        if (problem_input->bus_ids[i] == 0)
        {
            continue;
        }
        divisors[n] = problem_input->bus_ids[i];
        remainders[n] = problem_input->bus_ids[i] - i;
        n++;
    }

    return chinese_remainder(divisors, remainders, n);
}

int main()
{
    input example_input;
    parse_input("example.txt", &example_input);
    int example_earliest_bus_id = earliest_bus(&example_input);
    assert(bus_id_times_wait_time(&example_input) == 295);
    assert(earliest_timestamp(&example_input) == 1068781);

    input problem_input;
    parse_input("input.txt", &problem_input);
    int earliest_bus_id = earliest_bus(&problem_input);
    printf("%d\n", earliest_bus_id * wait_time_for_bus(problem_input.timestamp, earliest_bus_id));
    printf("%ld\n", earliest_timestamp(&problem_input));

    return 0;
}