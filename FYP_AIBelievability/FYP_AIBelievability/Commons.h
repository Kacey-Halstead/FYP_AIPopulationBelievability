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
	float healthVal = 100;
	float socialVal = 100;
};

enum EEmotions
{
	SURPRISE,
	ANTICIPATION,
	DISGUST,
	JOY,
	ANGER,
	FEAR,
	TRUST,
	SADNESS,
	NONE
};

constexpr int gridSizeX = 30;
constexpr int gridSizeY = 30;
constexpr glm::ivec2 windowSize{ 900, 900 };

constexpr std::array<char, 3> allTypes = { 'L', 'C', 'S' };
constexpr int numberOfTypes = allTypes.size();

bool ComparePositions(glm::vec2 a, glm::vec2 b, float acceptanceRadius);

static std::array<std::array<float, 8>, 8> OCEANMultipliers = {
		-1, -1, -1, 1, 0, -1, 1, -1,
		0, 1, 0, 0, -1, 0, 0, 1,
		0, -1, 0, 1, 0, 0, 1, 1,
		1, -1, -1, 1, 0, 0, 1, 0,
		1, 1, 1, -1, 1, 1, -1, 1
};