#include "FromJSON.h"

namespace FromJSONFile
{
	std::vector<Trait> traitsAndValues{};

	void ReadFromJSON()
	{
		std::ifstream file("traits.json");
		json j = json::parse(file);

		int currentType = 0;
		for (json& object : j) //for every OCEAN attribute
		{
			for (auto& trait : object.items()) //for every Trait 
			{
				if (trait.key() == "TYPE")
				{
					currentType = trait.value(); // sets OCEAN type
				}
				else
				{
					Trait t{};
					t.traitName = trait.key();
					t.OCEANEffect = currentType;
					t.traitEffect = trait.value();
					traitsAndValues.emplace_back(t);
				}
			}
		}
	}
}