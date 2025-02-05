#pragma once
#include <memory>
#include <vector>

class Grid;
class Agent;
class SDLWindow;
class DAG;
class FoodSource;

class FYP_AIBelievability
{
public:
	FYP_AIBelievability();
	~FYP_AIBelievability();

	void MainLoop();
private:
	std::unique_ptr<SDLWindow> mSDL;
	std::unique_ptr<Grid> mGrid;
	std::unique_ptr<DAG> mDAG;

	std::vector<Agent> mAgents;
	std::vector<FoodSource> mFoodSources;

	float mAccumulatedTime = 0;
	float mCounter = 0;
	int index = 1;

	void Render() const;
	void Update();
	void UpdateAgentsandFood();
};