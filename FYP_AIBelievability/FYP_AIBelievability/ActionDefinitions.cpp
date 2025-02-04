#include "ActionDefinitions.h"
#include "DAG.h"


//Defined Actions

//FOOD

struct EatFood
{
	static void Execute(States& states)
	{
		if (states.foodState.foundFoodRef && ComparePositions(states.agent->position, states.foodState.foundFoodRef->position, 1.0f)) //if near food
		{
			if (states.foodState.foundFoodRef->EatFrom(100 - states.agent->needs.hungerVal)) //if canEat
			{
				states.agent->needs.hungerVal = 100;
			}
			else
			{
				states.foodState.prevFoodPositions.erase(states.foodState.prevFoodPositions.begin());
				states.agent->ChangeEmotionValue(ANGER, 2);
				states.agent->ChangeEmotionValue(SADNESS, 2);
			}
			states.foodState.foundFoodRef = nullptr;
		}
		else if(!states.moveState.isMoveToSet)
		{
			states.foodState.foundFoodRef = FindNearestFoodSource(states);
			states.moveState.to = states.foodState.foundFoodRef->position;
			states.moveState.isMoveToSet = true;
			states.moveState.path = AStar::toFindPath(states.agent->position, states.moveState.to);
		}
	}

	static std::pair<ActionProgress,int> IsValid(States& states)
	{
		if (states.agent->needs.hungerVal > 60)
		{
			return { ActionProgress::Complete, 1 };
		}
		else if (states.foodState.prevFoodPositions.empty())
		{
			return { ActionProgress::Impossible, 1 };
		}
		else
		{
			return { ActionProgress::InProgress, 1 };
		}
	}

	static FoodSource* FindNearestFoodSource(States& states)
	{
		std::pair<float, FoodSource*> closestFood = std::make_pair(std::numeric_limits<float>().max(), states.foodState.prevFoodPositions[0].second);

		for (std::pair<glm::vec2, FoodSource*> foodSource : states.foodState.prevFoodPositions)
		{
			float distance = DistanceBetween(states.agent->position, foodSource.first);
			if(distance < closestFood.first)
			{
				closestFood = std::make_pair(distance, foodSource.second);
			}
		}
		return closestFood.second;
	}
};

// -------------------------------------------------------------------------------------------------------------------

//WATER

struct DrinkWater
{
	static void Execute(States& states)
	{
		if (states.waterState.waterRefSet && ComparePositions(states.agent->position, states.waterState.foundWaterRef, 1.0f)) //if near food
		{
			states.agent->needs.thirstVal = 100;
			states.waterState.waterRefSet = false;
		}
		else
		{
			states.moveState.to = FindNearestWaterSource(states);
			states.waterState.foundWaterRef = states.moveState.to;
			states.waterState.waterRefSet = true;
			states.moveState.isMoveToSet = true;
			states.moveState.path = AStar::toFindPath(states.agent->position, states.moveState.to);
		}
	}

	static std::pair<ActionProgress,int> IsValid(States& states)
	{
		if (states.agent->needs.thirstVal > 60)
		{
			return { ActionProgress::Complete, 1 };
		}
		else if (states.waterState.prevWaterPositions.empty())
		{
			return { ActionProgress::Impossible, 1 };
		}
		else
		{
			return { ActionProgress::InProgress, 1 };
		}
	}

	static glm::vec2 FindNearestWaterSource(States& states)
	{
		float limit = std::numeric_limits<float>().max();
		std::pair<float, glm::vec2> closestWater = std::make_pair(limit, states.waterState.prevWaterPositions[0]);

		for (glm::vec2 waterSource : states.waterState.prevWaterPositions)
		{
			float distance = DistanceBetween(states.agent->position, waterSource);
			if (distance < closestWater.first)
			{
				closestWater.second = waterSource;
			}
		}
		return closestWater.second;
	}
};

// -------------------------------------------------------------------------------------------------------------------

struct Wander
{
	static void Execute(States& states)
	{
		if(!states.moveState.isMoveToSet)
		{ 
			states.moveState.from = states.agent->position;

			states.moveState.to = GetToPos(states, states.agent->GetDominantEmotion().first);

			states.moveState.path = AStar::toFindPath(states.moveState.from, states.moveState.to);
			if (!states.moveState.path.empty())
			{
				states.moveState.isMoveToSet = true;
			}		
		}
	}

	static std::pair<ActionProgress,int> IsValid(States& states)
	{
		return { ActionProgress::InProgress, 0 };
	}

	static glm::vec2 GetToPos(States& states, EEmotions emotion)
	{
		if (states.socialState.otherAgents.empty())
		{
			std::uniform_int_distribution<> distrib(0, gridSizeX - 1);
			return { distrib(RandomGenerator::gen), distrib(RandomGenerator::gen) };
		}

		Agent* closestAgent = states.agent->GetClosestAgent();
		glm::vec2 vecBetween = closestAgent->position - states.agent->position;
		vecBetween = glm::normalize(vecBetween);

		switch (emotion)
		{
		case SADNESS:
		{
			glm::vec2 toMove = states.agent->position - glm::vec2(vecBetween.x * 3, vecBetween.y * 3);
			if (states.agent->GetGridRef()->IsInGrid(states.agent->position, glm::vec2(-vecBetween.x * 3, -vecBetween.y * 3)))
			{
				//avoid other agents
				return toMove;
			}
			break;
		}
		case JOY:
		{
			if (ComparePositions(states.agent->position, closestAgent->position, 3.0f))
			{
				return states.agent->position;
			}

			glm::vec2 toMove = states.agent->position + glm::vec2(vecBetween.x * 3, vecBetween.y * 3);
			if (states.agent->GetGridRef()->IsInGrid(states.agent->position, glm::vec2(vecBetween.x * 3, vecBetween.y * 3)))
			{
				//go to other agents
				return toMove;
			}
			break;
		}
		case FEAR:
		{
			//if close to people, runAway
			if (ComparePositions(states.agent->position, closestAgent->position, 2.0f))
			{
				states.socialState.agentRef->responsiveStack.push(FLEE);
			}
			return states.agent->position;
		}
		case SURPRISE:
		{
			//if close to people, runAway
			if (ComparePositions(states.agent->position, closestAgent->position, 2.0f))
			{
				states.socialState.agentRef->responsiveStack.push(FLEE);
			}
			return states.agent->position;
		}
		default:
		{
			std::uniform_int_distribution<> distrib(0, gridSizeX - 1);
			return { distrib(RandomGenerator::gen), distrib(RandomGenerator::gen) };
		}
		}

		std::uniform_int_distribution<> distrib(0, gridSizeX - 1);
		return { distrib(RandomGenerator::gen), distrib(RandomGenerator::gen) };
	}
};

// -------------------------------------------------------------------------------------------------------------------

struct FindAgentToSocialise
{
	static void Execute(States& states)
	{
		if (!states.socialState.agentRef)
		{
			states.moveState.isMoveToSet = false;
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
		}

		if (!states.moveState.isMoveToSet)
		{
			states.socialState.agentRef->states.socialState.isTalkingTo = true;
			states.socialState.isSeekingOtherAgent = true;
			states.moveState.to = states.socialState.agentRef->position;
			states.moveState.isMoveToSet = true;
			states.moveState.path = AStar::toFindPath(states.agent->position, states.moveState.to);
		}
	}

	static std::pair<ActionProgress,int> IsValid(States& states)
	{
		if (states.socialState.agentRef && ComparePositions(states.agent->position, states.moveState.to, 1.0f))
		{
			if (!ComparePositions(states.agent->position, states.socialState.agentRef->position, 1.0f))
			{
				states.socialState.agentRef->states.socialState.isTalkingTo = false;
				states.socialState.isSeekingOtherAgent = false;
				states.socialState.agentRef = nullptr;
				return { ActionProgress::Impossible, 1 };
			}

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
		Agent* agentWithMore = nullptr;
		Agent* agentWithLess = nullptr;

		//if agent foodPos > otherAgent foodPos
		if (states.agent->states.foodState.prevFoodPositions.size() > states.socialState.agentRef->states.foodState.prevFoodPositions.size())
		{
			agentWithMore = states.agent;
			agentWithLess = states.socialState.agentRef;
		}
		else
		{
			agentWithLess = states.agent;
			agentWithMore = states.socialState.agentRef;
		}

		for ( const std::pair<glm::vec2, FoodSource*>& positions : agentWithMore->states.foodState.prevFoodPositions)
		{
			if (std::none_of(agentWithLess->states.foodState.prevFoodPositions.begin(), agentWithLess->states.foodState.prevFoodPositions.end(), [positions](const std::pair<glm::vec2, FoodSource*>& other) {
				return positions.first == other.first;
				}))
			{
				agentWithLess->states.foodState.prevFoodPositions.push_back(positions);
				FinishSocial(states);
				return;
			}
		}

		for (const glm::vec2& positions : agentWithMore->states.waterState.prevWaterPositions)
		{
			if (std::none_of(agentWithLess->states.waterState.prevWaterPositions.begin(), agentWithLess->states.waterState.prevWaterPositions.end(), [positions](const glm::vec2& other) {
				return positions == other;
				}))
			{
				agentWithLess->states.waterState.prevWaterPositions.push_back(positions);
				FinishSocial(states);
				return;
			}
		}

		FinishSocial(states);
	}

	static std::pair<ActionProgress, int> IsValid(States& states)
	{
		if (states.agent->needs.socialVal > 80)
		{
			return { ActionProgress::Complete, 1 };
		}
		else if (!states.socialState.agentRef)
		{
			return { ActionProgress::Impossible, 1 };
		}
		
		return { ActionProgress::InProgress, 1 };
	}

	static void FinishSocial(States& states)
	{
		states.agent->ChangeEmotionValue(JOY, 1);
		states.agent->ChangeEmotionValue(TRUST, 2);

		states.socialState.agentRef->states.agent->ChangeEmotionValue(JOY, 1);
		states.socialState.agentRef->states.agent->ChangeEmotionValue(TRUST, 2);

		states.agent->needs.socialVal = 100;
		states.socialState.agentRef->needs.socialVal = 100;
		states.socialState.agentRef->states.socialState.isTalkingTo = false;
		states.socialState.isSeekingOtherAgent = false;
		states.socialState.agentRef = nullptr;
	}
};

// -------------------------------------------------------------------------------------------------------------------

struct Socialise
{
	static void Execute(States& states)
	{
		states.agent->ChangeEmotionValue(JOY, 2);
		states.agent->ChangeEmotionValue(TRUST, 1);

		states.socialState.agentRef->states.agent->ChangeEmotionValue(JOY, 2);
		states.socialState.agentRef->states.agent->ChangeEmotionValue(TRUST, 1);

		states.agent->needs.socialVal = 100;
		states.socialState.agentRef->needs.socialVal = 100;
		states.socialState.agentRef->states.socialState.isTalkingTo = false;
		states.socialState.isSeekingOtherAgent = false;
		states.socialState.agentRef = nullptr;
	}

	static std::pair<ActionProgress, int> IsValid(States& states)
	{
		if (states.agent->needs.socialVal > 80)
		{
			return { ActionProgress::Complete, 1 };
		}


		return { ActionProgress::InProgress, 1 };
	}
};

// -------------------------------------------------------------------------------------------------------------------

struct Fight
{
	static void Execute(States& states)
	{
		if (states.socialState.agentRef->GetDominantEmotion().first == ANGER) //if fight
		{
			states.socialState.agentRef->states.agent->ChangeEmotionValue(ANGER, 2);
			states.socialState.agentRef->states.agent->ChangeEmotionValue(ANTICIPATION, 1);

			states.socialState.agentRef->needs.healthVal -= 20.0f;
			states.agent->needs.healthVal -= 20.0f;

			states.socialState.agentRef->states.socialState.isTalkingTo = false;
		}
		else //if flight
		{			
			states.socialState.agentRef->states.socialState.agentRef = states.agent;
			states.socialState.agentRef->responsiveStack.push(FLEE);
		} 

		states.agent->needs.socialVal = 100;
		states.socialState.agentRef = nullptr;
		states.socialState.isSeekingOtherAgent = false;
	}

	static std::pair<ActionProgress,int> IsValid(States& states)
	{
		if (states.agent->needs.socialVal > 50)
		{
			return { ActionProgress::Complete, 1 };
		}

		return { ActionProgress::InProgress, 1 };
	}
};

// -------------------------------------------------------------------------------------------------------------------

struct RunAway
{
	static void Execute(States& states)
	{
		if (!states.moveState.isMoveToSet)
		{
			states.agent->SetSpeed(2.5f);
			states.agent->ChangeEmotionValue(SURPRISE, 1);
			states.agent->ChangeEmotionValue(FEAR, 2);

			states.socialState.runAwayPosBefore = states.agent->position;
			states.moveState.from = states.agent->position;
			states.moveState.to = GetRandomValidTile(states);
			states.moveState.isMoveToSet = true;
		}
	}

	static glm::vec2 GetRandomValidTile(States& states)
	{
		glm::vec2 otherAgentPos = states.socialState.agentRef->position;
		glm::vec2 diff = otherAgentPos - states.agent->position;
		diff = glm::normalize(diff);

		if (states.agent->GetGridRef()->IsInGrid(states.agent->position, glm::vec2(-diff.x * 5, -diff.y * 5)))
		{
			return { -diff.x * 4, -diff.y * 4 };
		}

		return states.agent->position;
	}

	static std::pair<ActionProgress, int> IsValid(States& states)
	{
		if (!ComparePositions(states.agent->position, states.socialState.runAwayPosBefore, 3.0f)) //is away from angry agent?
		{
			states.agent->SetSpeed(0.0f);
			states.socialState.isTalkingTo = false;
			return { ActionProgress::Complete, 1 };
		}

		return { ActionProgress::InProgress, 1 };
	}
};

// -------------------------------------------------------------------------------------------------------------------

//emotional outbursts if emotion too high
//anger - rampage
//sadness - avoid people
//Joy - be around people more
//


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

	std::vector<Action> ActionVec = {

		//FOOD
		{EatFood::Execute, EatFood::IsValid, GOAL_EATFOOD, "Eat Food", NONE},

		//WATER
		{DrinkWater::Execute, DrinkWater::IsValid, GOAL_DRINKWATER,"Drink Water", NONE},

		//WANDER
		{Wander::Execute, Wander::IsValid, GOAL_WANDER,"Wander", NONE},

		//SOCIAL
		{TransferKnowledge::Execute, TransferKnowledge::IsValid, GOAL_TRANSFERINFO,"Tell Other Agents About Food", TRUST},
		{FindAgentToSocialise::Execute, FindAgentToSocialise::IsValid, FIND_OTHER_AGENT,"Find Other Agent", NONE},
		{Fight::Execute, Fight::IsValid, GOAL_FIGHT,"Fight Agent", ANGER},
		{RunAway::Execute, RunAway::IsValid, FLEE,"Run Away", FEAR},
		{Socialise::Execute, Socialise::IsValid, GOAL_SOCIALISE,"Socialise", NONE}
	};

	std::vector<Action*> GetAllActions()
	{
		std::vector<Action*> actionPtrs;
		actionPtrs.reserve(ActionVec.size());

		for (Action& action : ActionVec)
		{
			actionPtrs.emplace_back(&action);
		}

		return actionPtrs;
	}

	Action GetAction(actionIDs ID)
	{
		return ActionVec[ID];
	}

	int Counter(std::string nameOfAction)
	{
		names.push_back(nameOfAction);
		return counter++;
	}

	Action GetGoalAction(ActionIndexes actionIndex, EEmotions emotion)
	{
		actionIDs toReturn{};

		switch (actionIndex)
		{
		case FOOD:
			toReturn = GOAL_EATFOOD;
			break;
		case WATER:
			toReturn = GOAL_DRINKWATER;
			break;
		case WANDER:
			toReturn = GOAL_WANDER;
			break;
		case SOCIAL:
		{
			if (emotion == ANGER)
			{
				toReturn = GOAL_FIGHT;
			}
			else if (emotion == TRUST)
			{
				toReturn = GOAL_TRANSFERINFO;
			}
			else
			{
				toReturn = GOAL_SOCIALISE;
			}
		}
			break;
		}

		return GetAction(toReturn);
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