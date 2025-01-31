#include "ActionDefinitions.h"
#include "DAG.h"


//Defined Actions

//FOOD
struct FindFood
{
	static void Execute(States& states)
	{
		//can check previous positions?
		if (!states.foodState.prevFoodPositions.empty())
		{
			states.moveState.to = states.foodState.prevFoodPositions[0].first;
			states.foodState.foundFoodRef = states.foodState.prevFoodPositions[0].second;
			setNextCheck(states);
		}
	}

	static std::pair<ActionProgress,int> IsValid(States& states)
	{
		if (states.foodState.prevFoodPositions.empty())
		{
			return { ActionProgress::Impossible, 1 };
		}
		else if(states.foodState.foundFoodRef != nullptr && ComparePositions(states.moveState.agent->position, states.foodState.foundFoodRef->position, 1.0f))
		{
			return { ActionProgress::Complete, 1 };
		}
		  
		return { ActionProgress::InProgress, 1 };
	}

	static void setNextCheck(States& states)
	{
		states.moveState.isMoveToSet = true;
		states.moveState.from = states.moveState.agent->position;
		states.moveState.path = AStar::toFindPath(states.moveState.agent->position, states.moveState.to);
	}
};

// -------------------------------------------------------------------------------------------------------------------

struct EatFood
{
	static void Execute(States& states)
	{
		if (states.foodState.foundFoodRef->EatFrom(100 - states.moveState.agent->needs.hungerVal))
		{
			states.moveState.agent->needs.hungerVal = 100;
		}
		else
		{
			states.foodState.prevFoodPositions.erase(states.foodState.prevFoodPositions.begin());
		}

		states.foodState.foundFoodRef = nullptr;
		states.moveState.agent->ChangeEmotionValue("Joy", 1);
	}

	static std::pair<ActionProgress,int> IsValid(States& states)
	{
		if (states.moveState.agent->needs.hungerVal > 60)
			return { ActionProgress::Complete, 1 };
		else
			return { ActionProgress::InProgress, 1 };
	}


};

// -------------------------------------------------------------------------------------------------------------------

//WATER
struct FindWater
{
	static void Execute(States& states)
	{
		states.moveState.from = states.moveState.agent->position;

		//can check previous positions?
		if (!states.waterState.prevWaterPositions.empty())
		{
			states.waterState.foundWaterRef = states.waterState.prevWaterPositions[0];
			states.moveState.to = states.waterState.prevWaterPositions[0];
			states.waterState.waterRefSet = true;
			setNextCheck(states);
		}		
	}

	static void setNextCheck(States& states)
	{
		states.moveState.isMoveToSet = true;
		states.moveState.path = AStar::toFindPath(states.moveState.from, states.moveState.to);

	}

	static std::pair<ActionProgress,int> IsValid(States& states)
	{
		if (states.waterState.prevWaterPositions.empty())
		{
			return { ActionProgress::Impossible, 1 };
		}
		else if (states.waterState.waterRefSet && ComparePositions(states.moveState.agent->position, states.waterState.foundWaterRef, 1.0f))
		{
			return { ActionProgress::Complete, 1 };
		}

		return { ActionProgress::InProgress, 1 };
	}
};

// -------------------------------------------------------------------------------------------------------------------

struct DrinkWater
{
	static void Execute(States& states)
	{
		states.moveState.agent->DrinkWater((100-states.moveState.agent->needs.thirstVal));
		states.waterState.waterRefSet = false;

		states.moveState.agent->ChangeEmotionValue("Joy", 1);
	}

	static std::pair<ActionProgress,int> IsValid(States& states)
	{
		if(states.moveState.agent->needs.thirstVal > 60)
			return { ActionProgress::Complete, 1 };
		else
			return { ActionProgress::InProgress, 1 };
	}
};

// -------------------------------------------------------------------------------------------------------------------

struct Wander
{
	static void Execute(States& states)
	{
		std::uniform_int_distribution<> distrib(0, gridSizeX - 1);

		states.moveState.from = states.moveState.agent->position;

		states.moveState.to.x = distrib(RandomGenerator::gen);
		states.moveState.to.y = distrib(RandomGenerator::gen);

		states.moveState.path = AStar::toFindPath(states.moveState.from, states.moveState.to);
		if (!states.moveState.path.empty())
		{
			states.moveState.isMoveToSet = true;
		}
	}

	static std::pair<ActionProgress,int> IsValid(States& states)
	{
		return { ActionProgress::InProgress, 0 };
	}
};

// -------------------------------------------------------------------------------------------------------------------

struct FindAgentToSocialise
{
	static void Execute(States& states)
	{
		//choose random agent
		//go to them

		states.moveState.from = states.moveState.agent->position;

		bool validAgent = false;
		int index = 0;

		while (!validAgent)
		{
			if (index == states.socialState.otherAgents.size() - 1)
			{
				return;
			}

			states.socialState.agentRef = states.socialState.otherAgents[index];

			if (!states.socialState.agentRef->states.socialState.isSeekingOtherAgent && !states.socialState.agentRef->states.socialState.isTalkingTo)
			{
				validAgent = true;
			}

			index++;
		}

		states.socialState.agentRef->states.socialState.isTalkingTo = true;
		states.socialState.isSeekingOtherAgent = true;
		states.moveState.to = states.socialState.agentRef->position;
		states.moveState.isMoveToSet = true;
		states.moveState.path = AStar::toFindPath(states.moveState.from, states.moveState.to);
	}

	static std::pair<ActionProgress,int> IsValid(States& states)
	{
		if (states.socialState.agentRef && ComparePositions(states.moveState.agent->position, states.socialState.agentRef->position, 1.0f))
		{
			return { ActionProgress::Complete, 1 };
		}
		else if(states.socialState.otherAgents.empty())
		{
			return { ActionProgress::Impossible, 1 };
		}
		else
		{
			return { ActionProgress::InProgress, 1 };
		}
	}
};

// -------------------------------------------------------------------------------------------------------------------

struct TransferKnowledge
{
	static void Execute(States& states)
	{
		//look through selected agent's food/water positions, if different, transfer

		for ( const std::pair<glm::vec2, FoodSource*>& positions : states.foodState.prevFoodPositions)
		{
			if (std::none_of(states.socialState.agentRef->states.foodState.prevFoodPositions.begin(), states.socialState.agentRef->states.foodState.prevFoodPositions.end(), [positions](const std::pair<glm::vec2, FoodSource*>& other) {
				return positions.first == other.first;
				}))
			{
				states.socialState.agentRef->states.foodState.prevFoodPositions.push_back(positions);

				break;
			}
		}

		for (const glm::vec2& positions : states.waterState.prevWaterPositions)
		{
			if (std::none_of(states.socialState.agentRef->states.waterState.prevWaterPositions.begin(), states.socialState.agentRef->states.waterState.prevWaterPositions.end(), [positions](const glm::vec2& other) {
				return positions == other;
				}))
			{
				states.socialState.agentRef->states.waterState.prevWaterPositions.push_back(positions);
				break;
			}
		}

		states.moveState.agent->ChangeEmotionValue("Joy", 1);
		states.moveState.agent->ChangeEmotionValue("Trust", 2);

		states.socialState.agentRef->states.moveState.agent->ChangeEmotionValue("Joy", 1);
		states.socialState.agentRef->states.moveState.agent->ChangeEmotionValue("Trust", 2);

		states.moveState.agent->needs.socialVal = 100;
		states.socialState.agentRef->needs.socialVal = 100;
		states.socialState.agentRef->states.socialState.isTalkingTo = false;
		states.socialState.isSeekingOtherAgent = false;
		states.socialState.agentRef = nullptr;
	}

	static std::pair<ActionProgress, int> IsValid(States& states)
	{
		if (states.moveState.agent->needs.socialVal > 80)
		{
			return { ActionProgress::Complete, 1 };
		}
		else if (!states.socialState.agentRef)
		{
			return { ActionProgress::Impossible, 1 };
		}
		
		return { ActionProgress::InProgress, 1 };
	}
};

// -------------------------------------------------------------------------------------------------------------------

struct Fight
{
	static void Execute(States& states)
	{
		if (states.socialState.agentRef->GetDominantEmotion().first == "Anger" || 
			states.socialState.agentRef->GetDominantEmotion().first == "Sadness") //if fight
		{

			states.socialState.agentRef->states.moveState.agent->ChangeEmotionValue("Anger", 2);
			states.socialState.agentRef->states.moveState.agent->ChangeEmotionValue("Anticipation", 1);

			states.socialState.agentRef->needs.healthVal -= 50.0f;
			states.moveState.agent->needs.healthVal -= 50.0f;
			states.socialState.agentRef->states.socialState.isTalkingTo = false;
		}
		else //if flight
		{			
			states.socialState.agentRef->responsiveStack.push(Actions::FindID("Run Away"));
		} 
	}

	static std::pair<ActionProgress,int> IsValid(States& states)
	{
		if (states.moveState.agent->needs.healthVal < 50 || ComparePositions(states.moveState.agent->position, states.socialState.agentRef->position, 3.0f))
		{
			states.socialState.agentRef = nullptr;
			states.socialState.isSeekingOtherAgent = false;
			return { ActionProgress::Complete, 1 };
		}
		else if (!states.socialState.agentRef && !states.moveState.agent->QueryDominantEmotions("Anger"))
		{
			return { ActionProgress::Impossible, 1 };
		}

		return { ActionProgress::InProgress, 1 };
	}
};

// -------------------------------------------------------------------------------------------------------------------

struct RunAway
{
	static void Execute(States& states)
	{
		states.moveState.agent->ChangeEmotionValue("Surprise", 1);
		states.moveState.agent->ChangeEmotionValue("Fear", 2);

		states.socialState.runAwayPosBefore = states.moveState.agent->position;
		states.moveState.from = states.moveState.agent->position;
		states.moveState.to = GetRandomValidTile(states);
		states.moveState.isMoveToSet = true;
	}

	static glm::vec2 GetRandomValidTile(States& states)
	{
		static std::vector<glm::vec2> Offsets = { {5, 0}, {-5, 0}, {0, 5}, {0, -5} }; //right, left, up, down

		for (int i = 0; i < 4; i++) //for all 4 directions
		{
			if (states.moveState.agent->GetGridRef()->IsInGrid(states.moveState.agent->position, Offsets[i])) //if in grid
			{
				return (states.moveState.agent->position + Offsets[i]);
			}
		}

		return glm::vec2();
	}

	static std::pair<ActionProgress, int> IsValid(States& states)
	{
		if (ComparePositions(states.moveState.agent->position, states.socialState.runAwayPosBefore, 5.0f)) //is away from angry agent?
		{
			states.socialState.isTalkingTo = false;
			return { ActionProgress::Complete, 1 };
		}
		else if (!states.socialState.isTalkingTo)
		{
			return { ActionProgress::Impossible, 1 };
		}

		return { ActionProgress::InProgress, 1 };
	}
};

// -------------------------------------------------------------------------------------------------------------------

struct FightBack
{
	static void Execute(States& states)
	{

	}

	static std::pair<ActionProgress, int> IsValid(States& states)
	{

	}
};

// -------------------------------------------------------------------------------------------------------------------
namespace Actions
{
	int counter = 0;

	std::vector<std::string> names{};

	std::vector<Action> foodActions = {
		MakeAction(EatFood::Execute, EatFood::IsValid, Counter("Eat Food")),
		MakeAction(FindFood::Execute, FindFood::IsValid, Counter("Find Food"))
	};

	std::vector<Action> waterActions = {
		MakeAction(DrinkWater::Execute, DrinkWater::IsValid, Counter("Drink Water")),
		MakeAction(FindWater::Execute, FindWater::IsValid, Counter("Find Water")),
	};

	std::vector<Action> wanderActions = {
		MakeAction(Wander::Execute, Wander::IsValid, Counter("Wander"))
	};

	std::vector<Action> socialActions = {
		MakeAction(TransferKnowledge::Execute, TransferKnowledge::IsValid, Counter("Tell Other Agents About Food")),
		MakeAction(FindAgentToSocialise::Execute, FindAgentToSocialise::IsValid, Counter("Find Other Agent")),
		MakeAction(Fight::Execute, Fight::IsValid, Counter("Fight Agent")),
		MakeAction(RunAway::Execute, RunAway::IsValid, Counter("Run Away"))
	};

	std::vector<Action>* GetActions(ActionIndexes index)
	{
		switch (index)
		{
		case FOOD:
			return &foodActions;
		case WATER:
			return &waterActions;
		case WANDER:
			return &wanderActions;
		case SOCIAL:
			return &socialActions;
		}

		return nullptr;
	}

	std::string Getname(int IDs)
	{
		return names[IDs];
	}

	int Counter(std::string nameOfAction)
	{
		names.push_back(nameOfAction);
		return counter++;
	}

	Action MakeAction(ExecuteFunc executeFunc, IsValidFunc isValidFunc, int ID)
	{
		return std::make_pair(std::make_pair(executeFunc, isValidFunc), ID);
	}

	int FindID(std::string nameToSearch)
	{
		int indexCounter = 0;

		for (std::string name : names) 
		{
			if (name == nameToSearch)
			{
				return indexCounter;
			}
			indexCounter++;
		}
	}
}