#pragma once
#include <array>
#include <string>

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
	extern std::array<float, 5> OCEANValues;
	extern std::array<std::string, 6> Traits;
}