// 4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <filesystem>

#include "..\aoc\aoc.h"

int Part1(std::string& input)
{
    std::string searchTerm = "XMAS";
    const int searchTermLen = searchTerm.size();

    auto lines = aoc::split(input, "\n");

    int xBound = lines[0].length();
    int yBound = lines.size();

    int occurences = 0;

    // Iterate the grid
    for (int y = 0; y < yBound; y++)
    {
        for (int x = 0; x < xBound; x++)
        {
            /*
                Given words can appear backwards we have 8 directions to check for our search term
                but certain directions can be excluded depedning on our x & y coord as the grid in
                a specific direction may not have enough space to fit said word

                If this is slow we could cut it to 4 directions and check for bacwards at the same time
                We then just have horizontal forward, vertical downward, diagonal forward up & diagional forward down
            */

            // Horizontal
            if (x + searchTermLen <= xBound)
            {
                bool forwardErr = false;
                bool backwardErr = false;
                for (int i = 0; i < searchTermLen; i++)
                {
                    if (lines[y][x + i] != searchTerm[i])
                    {
                        forwardErr = true;
                        
                    }

                    if (lines[y][x + i] != searchTerm[searchTermLen - (i+1)])
                    {
                        backwardErr = true;
                    }

                    if (forwardErr && backwardErr)
                    {
                        break;
                    }
                }

                if (not forwardErr || not backwardErr)
                {
                    occurences++;
                }
            }

            // Vertical
            if (y + searchTermLen <= yBound)
            {
                bool forwardErr = false;
                bool backwardErr = false;
                for (int i = 0; i < searchTermLen; i++)
                {
                    if (lines[y+i][x] != searchTerm[i])
                    {
                        forwardErr = true;

                    }

                    if (lines[y+i][x] != searchTerm[searchTermLen - (i + 1)])
                    {
                        backwardErr = true;
                    }

                    if (forwardErr && backwardErr)
                    {
                        break;
                    }
                }

                if (not forwardErr || not backwardErr)
                {
                    occurences++;
                }
            }

            // Diagonal Up
            if (x + searchTermLen <= xBound && y - searchTermLen >= -1)
            {
                bool forwardErr = false;
                bool backwardErr = false;
                for (int i = 0; i < searchTermLen; i++)
                {
                    if (lines[y - i][x + i] != searchTerm[i])
                    {
                        forwardErr = true;

                    }

                    if (lines[y - i][x + i] != searchTerm[searchTermLen - (i + 1)])
                    {
                        backwardErr = true;
                    }

                    if (forwardErr && backwardErr)
                    {
                        break;
                    }
                }

                if (not forwardErr || not backwardErr)
                {
                    occurences++;
                }
            }

            // Diagonal Down
            if (x + searchTermLen <= xBound && y + searchTermLen <= yBound)
            {
                bool forwardErr = false;
                bool backwardErr = false;
                for (int i = 0; i < searchTermLen; i++)
                {
                    if (lines[y + i][x + i] != searchTerm[i])
                    {
                        forwardErr = true;

                    }

                    if (lines[y + i][x + i] != searchTerm[searchTermLen - (i + 1)])
                    {
                        backwardErr = true;
                    }

                    if (forwardErr && backwardErr)
                    {
                        break;
                    }
                }

                if (not forwardErr || not backwardErr)
                {
                    occurences++;
                }
            }
        }
    }

    return occurences;
}

int Part2(std::string& input)
{
    auto lines = aoc::split(input, "\n");

    int xBound = lines[0].length();
    int yBound = lines.size();

    int occurences = 0;

    // Iterate the grid
    for (int y = 0; y < yBound; y++)
    {
        for (int x = 0; x < xBound; x++)
        {
            /*
                looking for x-mas we can only have it if we hit an A
                and we have space for 1 above and below and 1 for left and right

                8 9
            */

            if (lines[y][x] == 'A' && x + 1 < xBound && y + 1 < yBound && x - 1 >= 0 && y - 1 >= 0)
            {
                // Okay, we have an A and the space for x-MAS Given they both run diagonal
                if ((lines[y - 1][x - 1] == 'M' && lines[y + 1][x + 1] == 'S') || (lines[y - 1][x - 1] == 'S' && lines[y + 1][x + 1] == 'M'))
                {
                    if ((lines[y + 1][x - 1] == 'M' && lines[y - 1][x + 1] == 'S') || (lines[y + 1][x - 1] == 'S' && lines[y - 1][x + 1] == 'M'))
                    {
                        occurences++;
                    }
                }
            }
        }
    }

    return occurences;
}

int main()
{
    std::filesystem::path input = std::filesystem::current_path() / "..\\inputs\\4.txt";

    std::string inputText = aoc::readfile(input);

    auto part1 = aoc::timer<decltype(&Part1)>::time(Part1, inputText);

    std::cout << std::format("Part 1: {}", part1) << std::endl;

    auto part2 = aoc::timer<decltype(&Part2)>::time(Part2, inputText);

    std::cout << std::format("Part 2: {}", part2) << std::endl;
}

