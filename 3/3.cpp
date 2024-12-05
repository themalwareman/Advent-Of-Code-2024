// 3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include <regex>

#include "..\aoc\aoc.h"

int Part1(std::string& inputText)
{
    int retval = 0;

    std::regex mulReg("mul\\(([0-9]*),([0-9]*)\\)");

    auto begin = std::sregex_iterator(inputText.begin(), inputText.end(), mulReg);
    auto end = std::sregex_iterator();

    for (std::sregex_iterator i = begin; i != end; i++)
    {
        retval += (std::stoi(i->str(1)) * std::stoi(i->str(2)));
    }

    return retval;
}

int Part2(std::string& inputText)
{
    int retval = 0;
    bool enabled = true;

    std::regex mulReg("mul\\(([0-9]*),([0-9]*)\\)|don\'t\\(\\)|do\\(\\)");

    auto begin = std::sregex_iterator(inputText.begin(), inputText.end(), mulReg);
    auto end = std::sregex_iterator();

    for (std::sregex_iterator i = begin; i != end; i++)
    {
        if ("do()" == i->str())
        {
            enabled = true;
        }
        else if ("don't()" == i->str())
        {
            enabled = false;
        }
        else
        {
            if (enabled)
            {
                retval += (std::stoi(i->str(1)) * std::stoi(i->str(2)));
            }
        }
    }

    return retval;
}

int main()
{
    std::filesystem::path input = std::filesystem::current_path() / "..\\inputs\\3.txt";

    std::string inputText = aoc::readfile(input);

    auto part1 = aoc::timer<decltype(&Part1)>::time(Part1, inputText);

    std::cout << std::format("Part 1: {}", part1) << std::endl;

    auto part2 = aoc::timer<decltype(&Part2)>::time(Part2, inputText);

    std::cout << std::format("Part 2: {}", part2) << std::endl;
}

