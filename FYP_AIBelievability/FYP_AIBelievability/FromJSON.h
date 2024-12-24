#pragma once
#include<vector>
#include <string>
#include "Commons.h"
#include <iomanip>
#include "nlohmann/json.hpp"
#include <fstream>
using json = nlohmann::json;

namespace FromJSONFile
{
	void ReadFromJSON();

	extern std::vector<Trait> traitsAndValues; //trait name, OCEAN val, effect
}