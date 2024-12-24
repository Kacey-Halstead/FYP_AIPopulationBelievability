#pragma once
#include <array>
#include "Commons.h"

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

	extern bool isAgentPressed;
	extern int agentCount;
	extern std::array<int, 5> OCEANValues;
	extern std::array<Trait, 6> Traits;
}