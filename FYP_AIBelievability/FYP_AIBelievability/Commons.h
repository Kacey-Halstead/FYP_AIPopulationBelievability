#pragma once
#include <string>
#include <array>
#include <glm/glm.hpp>

struct Trait
{
	std::string traitName;
	int traitEffect;
	int OCEANEffect;
};

struct Needs
{
	float hungerVal = 100;
	float thirstVal = 100;
};

constexpr int gridSizeX = 30;
constexpr int gridSizeY = 30;
constexpr glm::ivec2 windowSize{ 900, 900 };

constexpr std::array<char, 3> allTypes = { 'L', 'C', 'S' };
constexpr int numberOfTypes = allTypes.size();

bool ComparePositions(glm::ivec2 a, glm::ivec2 b, int acceptanceRadius);


