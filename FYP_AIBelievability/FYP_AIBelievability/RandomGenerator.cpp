#include "RandomGenerator.h"

namespace RandomGenerator
{
	std::random_device rd{};
	std::mt19937_64 gen{ rd() };
}