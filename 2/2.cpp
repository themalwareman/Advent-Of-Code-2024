// 2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <ranges>
#include <format>
#include <thread>
#include <format>
#include <mutex>

#include "..\aoc\aoc.h"

void WorkerFunc(std::vector<std::string_view>& lines, std::atomic_size_t& safeCount, size_t begin, size_t end)
{
    for (int i = begin; i < end; i++)
    {
        auto report = aoc::split(lines[i], " ");

        int currLevel = std::stoi(report[0].data());

        bool increment = (currLevel < std::stoi(report[1].data()));

        int j = 1;
        bool safe = true;

        for (; j < report.size(); j++)
        {
            int level = std::stoi(report[j].data());

            // Is splitting them out easier for the optimiser?
            if (level == currLevel)
            {
                safe = false;
                break;
            }
            else if (increment && (level < currLevel))
            {
                safe = false;
                break;
            }
            else if (not increment && (level > currLevel))
            {
                safe = false;
                break;
            }
            else if (std::abs(level - currLevel) > 3)
            {
                safe = false;
                break;
            }

            /*if ((std::abs(level - currLevel) > 3) || (increment && (level < currLevel)) || (not increment && (level > currLevel)) || (level == currLevel))
            {
                safe = false;
                break;
            }*/

            currLevel = level;
        }

        if (safe)
        {
            safeCount++;
        }
    }
}

/*
    This one has the fault tolerance which makes it impossible for us to know whether the sequence is
    incrementing or decrementing so I think we have to test both conditions as we go.

    We also have the special case where if transition 1 -> 2 fails we can remove either 1 or 2 and have
    the transition 1 -> 3 or transition 2 -> 3.
*/
void Worker2Func(std::vector<std::string_view>& lines, std::atomic_size_t& safeCount, size_t begin, size_t end)
{
    for (int i = begin; i < end; i++)
    {
        auto report = aoc::split(lines[i], " ");

        /*
            We need to evalute both cases so need seprate current level checks for failure tolerance
        */
        int incCurrLevel = std::stoi(report[0].data());
        int decCurrLevel = incCurrLevel;

        int j = 1;

        int incrementErrorCount = 0;
        int decrementErrorCount = 0;

        for (; j < report.size(); j++)
        {
            // We need to know if we hit an issue this step
            bool incError = false;
            bool decError = false;

            int level = std::stoi(report[j].data());

            // By doing the following checks separately for each case the if/else can short circuit when it hits a case

            // Test the increment conditions - skip this logic if our count is already too high
            if (incrementErrorCount <= 1)
            {
                if (std::abs(level - incCurrLevel) > 3)
                {
                    incrementErrorCount++;
                    incError = true;
                }
                else if (level < incCurrLevel)
                {
                    incrementErrorCount++;
                    incError = true;
                }
                else if (level == incCurrLevel)
                {
                    incrementErrorCount++;
                    incError = true;
                }
            }

            // Test decrement conditions - skip this logic if our count is already too high
            if (decrementErrorCount <= 1)
            {
                if (std::abs(level - decCurrLevel) > 3)
                {
                    decrementErrorCount++;
                    decError = true;
                }
                else if (level > decCurrLevel)
                {
                    decrementErrorCount++;
                    decError = true;
                }
                else if (level == decCurrLevel)
                {
                    decrementErrorCount++;
                    decError = true;
                }
            }

            /*
                Special case

                Is this step 1
            */
            if (1 == j)
            {
                int lookAhead = std::stoi(report[j + 1].data());

                /*
                    If this is step 1 and we triggered an increment error we can check to see if 1->3 or 2->3 is a valid transition
                    and update the current level accordingly
                */
                // We had a increment failure indicating 1 -> 2 is invalid for the incrment case
                if (incError)
                {
                    // If step 3 is greater than the current level but not by more than 3 then 1 -> 3 is valid
                    // So we don't change our current level however is 1 -> 3 isn't valid then we have a chance to
                    // use 2 -> 3 by updating our current level
                    if (not (lookAhead > incCurrLevel && std::abs(incCurrLevel - lookAhead) <= 3))
                    {
                        incCurrLevel = level;
                    }
                }
                else
                {
                    incCurrLevel = level;
                }

                if (decError)
                {
                    // If step 3 is greater than the current level but not by more than 3 then 1 -> 3 is valid
                    // So we don't change our current level however is 1 -> 3 isn't valid then we have a chance to
                    // use 2 -> 3 by updating our current level
                    if (not (lookAhead < decCurrLevel && std::abs(decCurrLevel - lookAhead) <= 3))
                    {
                        decCurrLevel = level;
                    }
                }
                else
                {
                    decCurrLevel = level;
                }
            }
            else
            {
                // Not first step

                // If we didn't trigger an increment error then advance the level because we had a valid transition
                // If the transition was invalid we count this error and ignore the next level by not updating the current
                // level
                if (not incError)
                {
                    incCurrLevel = level;
                }

                // Same logic for the decrement tracker
                if (not decError)
                {
                    decCurrLevel = level;
                }
            }

            // If we have more than 1 error for both options we can exit early as this cannot be valid
            if (incrementErrorCount > 1 && decrementErrorCount > 1)
            {
                break;
            }

        }

        // If either method had an error count of 1 or less then this is valid
        if (incrementErrorCount <= 1 || decrementErrorCount <= 1)
        {
            safeCount++;
        }
    }
}

int Part1(std::string& input)
{
    auto lines = aoc::split(input, "\n");

    std::atomic_size_t atomicSafeCount = 0;

    std::vector<std::thread> threads;

    const int jobSize = 200;

    for (int j = 0; j < lines.size(); j += jobSize)
    {
        threads.emplace_back(WorkerFunc, std::ref(lines), std::ref(atomicSafeCount), j, (j + jobSize) >= lines.size() ? lines.size() : j + jobSize);
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    return static_cast<int>(atomicSafeCount);
}

int Part2(std::string& input)
{
    std::vector<std::string_view> lines = aoc::split(input, "\n");

    std::atomic_size_t atomicSafeCount = 0;

    std::vector<std::thread> threads;

    const int jobSize = 200;

    for (int j = 0; j < lines.size(); j += jobSize)
    {
        threads.emplace_back(Worker2Func, std::ref(lines), std::ref(atomicSafeCount), j, (j + jobSize) >= lines.size() ? lines.size() : j + jobSize);
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    return static_cast<int>(atomicSafeCount);
}

int main()
{
    std::filesystem::path input = std::filesystem::current_path() / "..\\inputs\\2.txt";

    std::string inputText = aoc::readfile(input);

    auto part1 = aoc::timer<decltype(&Part1)>::time(Part1, inputText);

    std::cout << std::format("Part 1: {}", part1) << std::endl;

    auto part2 = aoc::timer<decltype(&Part2)>::time(Part2, inputText);

    std::cout << std::format("Part 2: {}", part2) << std::endl;
}
