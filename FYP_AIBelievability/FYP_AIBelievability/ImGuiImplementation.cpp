#include "ImGuiImplementation.h"
#include <imgui.h>
#include <implot.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <SDL.h>



namespace ImGui_Implementation
{
	int agentCount = 0;
	std::array<int, 5> OCEANValues{};
	std::array<Trait, 6> Traits{};
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
		ImGui_Implementation::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui_Implementation::GetDrawData(), renderer);
	}

	void ProcessEvents(SDL_Event* e)
	{
		ImGui_ImplSDL2_ProcessEvent(e);
	}

	void AgentPopUp()
	{
		if (isAgentPressed)
		{
			ImGui_Implementation::Begin("Agent Information");

			//OCEAN VALUES
			if (ImPlot::BeginPlot("Agent Graph"))
			{		
				ImPlot::SetupAxes("OCEAN Factors", "OCEAN Value Intensities", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
				ImPlot::SetupAxesLimits(0, 5, 0, 5);

				static const double positions[] = { 0,1,2,3,4 };
				static const char* labels[] = {"O", "C", "E", "A", "N"};

				ImPlot::SetupAxisTicks(ImAxis_X1, positions, 5, labels);
				ImPlot::PlotBars("OCEAN Values", OCEANValues.data(), OCEANValues.size(), 0.5);
				ImPlot::EndPlot();
			}

			//TRAITS
			
			if (ImGui_Implementation::BeginChild("Scrolling"))
			{
				for (int i = 0; i < Traits.size(); i++)
				{
					ImGui_Implementation::Text(Traits[i].traitName.c_str());
				}
			}
			ImGui_Implementation::EndChild();

			ImGui_Implementation::Text("agent = %d", agentCount);

			ImGui_Implementation::End();	
		}
	}

	void MainUI(WFC* wfcRef)
	{
		ImGui_Implementation::Begin("Menu");
		if (ImGui_Implementation::Button("Regenerate WFC", {120, 30}))
		{
			wfcRef->WFCReset();
		}
		ImGui_Implementation::End();
	}

	void RenderBefore()
	{
		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui_Implementation::NewFrame();
	}
}
