#include "ImGuiImplementation.h"
#include <imgui.h>
#include <implot.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <SDL.h>

namespace ImGui_Implementation
{
	int agentCount = 0;
	std::array<float, 5> OCEANValues{};
	std::array<std::string, 6> Traits{};
	bool isAgentPressed = false;

	void Init(SDL_Renderer* renderer, SDL_Window* window)
	{
		//IMGUI
		ImGui_Implementation::CreateContext();
		ImPlot::CreateContext();
		ImGui_Implementation::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

		// Setup Platform/Renderer backends
		ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
		ImGui_ImplSDLRenderer2_Init(renderer);
	}

	void Destroy()
	{
		ImGui_ImplSDLRenderer2_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui_Implementation::DestroyContext();
		ImPlot::DestroyContext();
	}

	void ImGuiDraw(SDL_Renderer* renderer)
	{
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui_Implementation::GetDrawData(), renderer);
	}

	void ProcessEvents(SDL_Event* e)
	{
		ImGui_ImplSDL2_ProcessEvent(e);
	}

	void AgentPopUp()
	{
		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui_Implementation::NewFrame();

		if (isAgentPressed)
		{
			ImGui_Implementation::Begin("Agent Information");

			//OCEAN VALUES
			if (ImPlot::BeginPlot("Agent Graph"))
			{		
				ImPlot::PlotBars("OCEAN Values", OCEANValues.data(), OCEANValues.size());
				ImPlot::EndPlot();
			}

			//TRAITS
			
			if (ImGui_Implementation::BeginChild("Scrolling"))
			{
				for (int i = 0; i < Traits.size(); i++)
				{
					ImGui_Implementation::Text(Traits[i].c_str());
				}
			}
			ImGui_Implementation::EndChild();

			ImGui_Implementation::Text("agent = %d", agentCount);

			ImGui_Implementation::End();	
		}
		ImGui_Implementation::Render();
	}
}
