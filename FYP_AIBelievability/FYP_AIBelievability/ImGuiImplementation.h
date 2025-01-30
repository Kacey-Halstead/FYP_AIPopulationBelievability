#pragma once
#include <array>
#include <glm/glm.hpp>
#include <string>

#include "Commons.h"
#include "WFC.h" 
#include "PersonalityComponent.h"

struct SDL_Renderer;
union SDL_Event;
struct SDL_Window;

namespace ImGui_Implementation
{
	void Init(SDL_Renderer* renderer, SDL_Window* window);
	void Destroy();
	void ImGuiDraw(SDL_Renderer* renderer);
	void ProcessEvents(SDL_Event* e);
	void AgentPopUp();
	void MainUI();
	void RenderBefore();

	extern bool isAgentPressed;	
	extern int agentCount;
	extern std::array<int, 5> OCEANValues;
	extern std::array<Trait, 6> Traits;
	extern Needs needStruct;
	extern float currentTime;
	extern std::string action;

	extern std::vector<float> hungerValues;
	extern std::vector<float> thirstValues;
	extern std::vector<float> socialValues;
	extern std::vector<float> time;

	extern bool pause;
	extern bool toSelectDest;
}