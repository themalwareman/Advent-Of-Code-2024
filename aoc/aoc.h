#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <format>
#include <iostream>

namespace aoc
{

	template<typename T>
	struct timer;

	template<typename R, typename... Args>
	struct timer<R(*)(Args...)>
	{
		using fn = R(*)(Args...);

		static R time(fn func, Args... args)
		{
			auto start = std::chrono::high_resolution_clock::now();
			R retval = func(args...);
			auto stop = std::chrono::high_resolution_clock::now();

			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

			std::cout << std::format("Func took {:.2f} milliseconds", static_cast<float>(duration.count()) / 1000) << std::endl;

			return retval;
		}
	};

	std::string readfile(std::filesystem::path& path)
	{
		std::ifstream input(path);

		std::stringstream buffer;

		buffer << input.rdbuf();

		return buffer.str();
	}

	std::vector<std::string_view> split(std::string_view input, std::string_view delim)
	{
		std::vector<std::string_view> results;

		size_t last = 0;
		size_t next = 0;

		while ((next = input.find(delim, last)) != std::string::npos)
		{
			results.emplace_back(&input[last], next - last);
			last = next + delim.size();
		}

		if (last < input.size())
		{
			results.emplace_back(&input[last]);
		}

		return results;
	}
}