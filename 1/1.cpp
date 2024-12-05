// 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include <ranges>
#include <algorithm>
#include <format>
#include <map>

#include "..\aoc\aoc.h"


int Part1(std::string& input)
{
    size_t difference = 0;

    std::vector<std::string_view> lines = aoc::split(input, "\n");

    std::vector<int> list1;
    std::vector<int> list2;

    for (auto& line : lines) {
        auto dataPoints = aoc::split(line, "   ") | std::views::transform([](std::string_view s) {return std::stoi(s.data()); });
        list1.push_back(dataPoints[0]);
        list2.push_back(dataPoints[1]);
    }

    std::sort(list1.begin(), list1.end());
    std::sort(list2.begin(), list2.end());

    for (size_t i = 0; i < list1.size(); i++)
    {
        difference += std::abs(list1[i] - list2[i]);
    }

    return difference;
}

int Part2(std::string& input)
{
    size_t difference = 0;

    std::vector<std::string_view> lines = aoc::split(input, "\n");

    std::vector<int> list1;
    std::vector<int> list2;

    for (auto& line : lines) {
        auto dataPoints = aoc::split(line, "   ") | std::views::transform([](std::string_view s) {return std::stoi(s.data()); });
        list1.push_back(dataPoints[0]);
        list2.push_back(dataPoints[1]);
    }

    std::sort(list1.begin(), list1.end());
    std::sort(list2.begin(), list2.end());

    std::map<int, int> counts;

    std::for_each(list2.begin(), list2.end(), [&](int val) { counts[val]++; });

    size_t newDifference = 0;

    std::for_each(list1.begin(), list1.end(), [&](int val) { difference += val * counts[val]; });

    return difference;
}

int main()
{
    std::filesystem::path input = std::filesystem::current_path() / "..\\inputs\\1.txt";

    std::string inputText = aoc::readfile(input);

    auto part1 = aoc::timer<decltype(&Part1)>::time(Part1, inputText);

    std::cout << std::format("Part 1: {}", part1) << std::endl;

    auto part2 = aoc::timer<decltype(&Part1)>::time(Part2, inputText);

    std::cout << std::format("Part 2: {}", part2) << std::endl;
}
