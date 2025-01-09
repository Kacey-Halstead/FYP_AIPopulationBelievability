#pragma once
#include <array>
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
	void MainUI(WFC* wfcRef);
	void RenderBefore();

	extern bool isAgentPressed;	
	extern int agentCount;
	extern std::array<int, 5> OCEANValues;
	extern std::array<Trait, 6> Traits;
}