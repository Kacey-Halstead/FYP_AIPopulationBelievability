#pragma once
#include <random>

namespace RandomGenerator
{
	extern std::mt19937_64 gen;

	std::mt19937_64 SeededGenerator();
}