// 5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <set>
#include <unordered_map>
#include <map>
#include <string>

#include "..\aoc\aoc.h"

int Part1(std::string& input)
{
    int total = 0;

    auto lines = aoc::split(input, "\n");

    // Lets read the ordering rules
    std::unordered_multimap<int, int> backwardOrdering;

    int i = 0;
    while (not lines[i].empty())
    {
        auto pages = aoc::split(lines[i], "|");

        int first = std::stoi(pages[0].data());
        int second = std::stoi(pages[1].data());

        backwardOrdering.emplace(second, first);

        i++;
    }

    i++;

    // Now iterate reports
    for (; i < lines.size(); i++)
    {
        // Get line as vector of int
        std::vector<int> pageSequence;

        for (auto& page : aoc::split(lines[i], ","))
        {
            pageSequence.push_back(std::stoi(page.data()));
        }

        /*
            Okay, so what do we need to check?

            For each number we come across I think we just need to check that no ordering
            rule exists that prevents one of the already seen number from preceeding it

            If one of the already seen numbers has the current number as a value in the backward
            ordering map then we have a violation because the current number should have come before
            it
        */

        // Now walk the sequence
        bool valid = true;

        for (int j = 1; j < pageSequence.size(); j++)
        {
            // For each number that we have already seen
            for (int y = 0; y < j; y++)
            {
                if (backwardOrdering.contains(pageSequence[y]))
                {
                    // Iterate over all backward orderings
                    for (auto [itr, rangeEnd] = backwardOrdering.equal_range(pageSequence[y]); itr != rangeEnd; ++itr)
                    {
                        if (pageSequence[j] == itr->second)
                        {
                            valid = false;
                            break;
                        }
                    }
                }

                if (not valid)
                {
                    break;
                }
            }

            if (not valid)
            {
                break;
            }
        }

        if (valid)
        {
            total += pageSequence[pageSequence.size() / 2];
        }
    }

    return total;
}

int Part2(std::string& input)
{
    int total = 0;

    auto lines = aoc::split(input, "\n");

    // Lets read the ordering rules
    std::unordered_multimap<int, int> backwardOrdering;

    int i = 0;
    while (not lines[i].empty())
    {
        auto pages = aoc::split(lines[i], "|");

        int first = std::stoi(pages[0].data());
        int second = std::stoi(pages[1].data());

        backwardOrdering.emplace(second, first);

        i++;
    }

    i++;

    // Now iterate reports
    for (; i < lines.size(); i++)
    {
        // Get line as vector of int
        std::vector<int> pageSequence;
        auto pages = aoc::split(lines[i], ",");
        for (auto& page : pages)
        {
            pageSequence.push_back(std::stoi(page.data()));
        }

        /*
            Okay, so what do we need to check?

            For each number we come across I think we just need to check that no ordering
            rule exists that prevents one of the already seen number from preceeding it

            If one of the already seen numbers has the current number as a value in the backward
            ordering map then we have a violation because the current number should have come before
            it
        */

        // Now walk the sequence
        bool valid = true;

        for (int j = 1; j < pageSequence.size(); j++)
        {
            // For each number that we have already seen
            for (int y = 0; y < j; y++)
            {
                // Iterate over all backward orderings
                for (auto [itr, rangeEnd] = backwardOrdering.equal_range(pageSequence[y]); itr != rangeEnd; ++itr)
                {
                    if (pageSequence[j] == itr->second)
                    {
                        valid = false;
                        break;
                    }
                }

                if (not valid)
                {
                    break;
                }
            }

            if (not valid)
            {
                break;
            }
        }

        if (not valid)
        {
            // Time to attempt a fix
            std::vector<int> correctedSequence = { pageSequence[0]};

            for (int j = 1; j < pageSequence.size(); j++)
            {
                /*
                    For each number we now want to insert into our corrected
                    sequence we need to check no rules are violated
                */

                bool inserted = false;

                // Consider every position in the corrected sequence for insertion
                for (int y = 0; y < correctedSequence.size(); y++)
                {
                    

                    // We have to check that no value that comes after this would trigger a violation
                    // So look at every value after index y
                    bool allowed = true;

                    for (int x = y; x < correctedSequence.size(); x++)
                    {
                        // Treat the new number as "seen" i.e. comes before
                        for (auto [itr, rangeEnd] = backwardOrdering.equal_range(pageSequence[j]); itr != rangeEnd; ++itr)
                        {
                            // If the value in our corrected sequence appears as a backward ordering thats a violation
                            // so our insert cannot happen before this value in our corrected sequence
                            if (correctedSequence[x] == itr->second)
                            {
                                allowed = false;
                                break;
                            }
                        }

                        if (not allowed)
                        {
                            break;
                        }
                    }

                    if (allowed)
                    {
                        // No violation would occur by inserting before this value
                        correctedSequence.insert(correctedSequence.begin() + y, pageSequence[j]);
                        inserted = true;
                        break;
                    }

                    // else keep looking
                }

                if (not inserted)
                {
                    correctedSequence.push_back(pageSequence[j]);
                }
            }

            // I think we assume we're okay here
            total += correctedSequence[correctedSequence.size() / 2];
        }
    }

    return total;
}

int main()
{
    std::filesystem::path input = std::filesystem::current_path() / "..\\inputs\\5.txt";

    std::string inputText = aoc::readfile(input);

    auto part1 = aoc::timer<decltype(&Part1)>::time(Part1, inputText);

    std::cout << std::format("Part 1: {}", part1) << std::endl;

    auto part2 = aoc::timer<decltype(&Part2)>::time(Part2, inputText);

    std::cout << std::format("Part 2: {}", part2) << std::endl;
}

