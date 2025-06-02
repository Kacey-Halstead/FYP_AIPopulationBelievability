#include "RandomGenerator.h"

namespace RandomGenerator
{
	std::random_device rd{};
	std::mt19937_64 gen{ rd() };

	std::mt19937_64 SeededGenerator()
	{
#ifdef __EMSCRIPTEN__
	unsigned long long seed = 1934669435;
#else
	unsigned long long seed = 2840374865;
#endif
	std::mt19937_64 gen{};
	gen.seed(seed);
		return gen;
	}
}