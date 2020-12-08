#include <iostream>
#include <fstream>
#include <cassert>
#include <regex>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using namespace std;

typedef tuple<int, string> bag;

unordered_map<string, vector<bag>> parse_input(string filename)
{
    regex first_bag_regex("(\\w+\\b \\w+\\b) bags");
    regex contained_bag_regex("(\\d) (\\w+\\b \\w+\\b) bags?");

    unordered_map<string, vector<bag>> rules;

    ifstream input_file;
    input_file.open(filename);

    string line;
    while (getline(input_file, line))
    {
        string first_bag;
        vector<bag> contained_bags;
        smatch match;

        regex_search(line, match, first_bag_regex);
        first_bag = match[1];
        line = match.suffix();

        while (regex_search(line, match, contained_bag_regex))
        {
            contained_bags.push_back(make_tuple(stoi(match[1]), match[2]));
            line = match.suffix();
        }

        rules[first_bag] = contained_bags;
    }

    return rules;
}

// Given a set of rules, each given as a bag and a list of bags contained inside it, return a new set of rules given as a bag and a list of bags containing it.
unordered_map<string, vector<string>> reverse_rules(unordered_map<string, vector<bag>> &rules)
{
    unordered_map<string, vector<string>> graph;
    for (auto rule : rules)
    {
        string containing_bag = rule.first;
        vector<bag> contained_bags = rule.second;
        for (bag b : contained_bags)
        {
            string contained_bag;
            tie(ignore, contained_bag) = b;
            if (graph.find(contained_bag) == graph.end())
            {
                vector<string> containing_bags;
                graph[contained_bag] = containing_bags;
            }
            graph[contained_bag].push_back(containing_bag);
        }
    }

    return graph;
}

// Part I Solution
int num_containing(unordered_map<string, vector<bag>> &rules, string bag_color)
{
    unordered_map<string, vector<string>> graph = reverse_rules(rules);
    queue<string> bag_queue;
    unordered_set<string> discovered_bags;

    // Perform breadth-first-search on the graph
    discovered_bags.insert(bag_color);
    bag_queue.push(bag_color);

    while (!bag_queue.empty())
    {
        string containing_bag = bag_queue.front();
        bag_queue.pop();
        for (string contained_bag : graph[containing_bag])
        {
            if (discovered_bags.find(contained_bag) == discovered_bags.end())
            {
                discovered_bags.insert(contained_bag);
                bag_queue.push(contained_bag);
            }
        }
    }

    return discovered_bags.size() - 1; // the bag itself doesn't count
}

// Part II Solution
int num_bags_inside_bag_rec(unordered_map<string, vector<bag>> &rules, string bag_color, unordered_map<string, int> &mem)
{
    if (mem.find(bag_color) == mem.end())
    {
        int result = 1;
        vector<bag> contained = rules[bag_color];
        for (bag b : contained)
        {
            int duplicates;
            string color;
            tie(duplicates, color) = b;
            result += duplicates * num_bags_inside_bag_rec(rules, color, mem);
        }
        mem[bag_color] = result;
    }
    return mem[bag_color];
}

int num_bags_inside_bag(unordered_map<string, vector<bag>> &rules, string bag_color)
{
    unordered_map<string, int> mem;                            // an array storing results to increase efficiency (memoization)
    return num_bags_inside_bag_rec(rules, bag_color, mem) - 1; // the bag itself doesn't count
}

int main()
{
    unordered_map<string, vector<bag>> example_rules = parse_input("example.txt");
    assert(num_containing(example_rules, "shiny gold") == 4);

    unordered_map<string, vector<bag>> example2_rules = parse_input("example2.txt");
    assert(num_bags_inside_bag(example2_rules, "shiny gold") == 126);

    unordered_map<string, vector<bag>> rules = parse_input("input.txt");
    cout << num_containing(rules, "shiny gold") << endl;
    cout << num_bags_inside_bag(rules, "shiny gold") << endl;

    return 0;
}