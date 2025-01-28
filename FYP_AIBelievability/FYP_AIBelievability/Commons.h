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

struct Emotions
{
	float Surprise = 0;
	float Trust = 0;
	float Joy = 0;
	float Fear = 0;
	float Anticipation = 0;
	float Sadness = 0;
	float Disgust = 0;
	float Anger = 0;
};

constexpr int gridSizeX = 30;
constexpr int gridSizeY = 30;
constexpr glm::ivec2 windowSize{ 900, 900 };

constexpr std::array<char, 3> allTypes = { 'L', 'C', 'S' };
constexpr int numberOfTypes = allTypes.size();

bool ComparePositions(glm::vec2 a, glm::vec2 b, float acceptanceRadius);


