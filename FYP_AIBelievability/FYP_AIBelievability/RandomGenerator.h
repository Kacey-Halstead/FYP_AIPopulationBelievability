#pragma once
#include <random>

namespace RandomGenerator
{
	extern std::random_device rd;
	extern std::mt19937_64 gen;
}