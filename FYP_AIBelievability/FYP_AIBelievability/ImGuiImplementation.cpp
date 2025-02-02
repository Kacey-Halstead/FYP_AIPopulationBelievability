#include "ImGuiImplementation.h"
#include <imgui.h>
#include <implot.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <SDL.h>

namespace ImGui_Implementation
{
	int agentCount = 1;
	std::array<int, 5> OCEANValues{};
	std::array<Trait, 6> Traits{};
	bool isAgentPressed = false;
	Needs needStruct{};
	float currentTime = 0;
	bool pause = false;
	bool toSelectDest = false;

	float deltaTimeModifier = 1.0f;

	std::list<std::string> actions = {"Wander"};

	std::array<std::pair<EEmotions, float>, 8> emotionValues = {};

	std::vector<float> hungerValues = std::vector<float>(400, 100);
	std::vector<float> thirstValues = std::vector<float>(400, 100);
	std::vector<float> socialValues = std::vector<float>(400, 100);
	std::vector<float> time = std::vector<float>(400, 0);

	ImU32 emotionColours[8] = { //SADJAFTS	
		0xFFe3d732, 
		0xFF4397e0,
		0xFFd945d1,
		0xFF10d1e3,
		0xFF1111d6,
		0xFF1b8c26,
		0xFF5fe86d,
		0xFFe35232
	};

	ImPlotColormap customEmotionColours;

	void Init(SDL_Renderer* renderer, SDL_Window* window)
	{
		//IMGUI
		ImGui_Implementation::CreateContext();
		ImPlot::CreateContext();
		ImGui_Implementation::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

		// Setup Platform/Renderer backends
		ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
		ImGui_ImplSDLRenderer2_Init(renderer);		
		customEmotionColours = ImPlot::AddColormap("FYPAiBelievability", emotionColours, 8);
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
		ImGui_Implementation::SetNextWindowPos({ 900, 200 });
		ImGui_Implementation::SetNextWindowSize({ 400, 700 });

		ImGui_Implementation::Begin("Agent Information", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

		ImGui_Implementation::Text("agent = %d", agentCount);

		//list of completed actions
		ImGui_Implementation::Text("Actions Performed");
		ImGui_Implementation::SetNextWindowSize({ 400, 200 });
		ImGui_Implementation::BeginChild("ActionsPerformed");

		std::list<std::string> toPrint = actions;

		if (!toPrint.empty())
		{
			ImGui_Implementation::Text("Most Recent Action: = %s", toPrint.front().c_str());
			toPrint.pop_front();

			for (std::string action : toPrint) //0 = most recent action
			{
				ImGui_Implementation::Text(action.c_str());
			}
		}

		ImGui_Implementation::EndChild();

		//EMOTIONS
		if (ImPlot::BeginPlot("Emotions", ImVec2(370, 370), ImPlotFlags_NoInputs))
		{
			const char* labels[8] = { "Surprise" , "Anticipation", "Disgust", "Joy", "Anger", "Fear", "Trust", "Sadness" };
			float data[8];

			for (int i = 0; i < 8; i++)
			{
				if (emotionValues[i].second < 0)
				{
					data[i] = 0;
					continue;
				}

				data[i] = emotionValues[i].second;
			}

			ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);
			ImPlot::SetupAxesLimits(0, 1, 0, 1);

			ImPlot::PushColormap(customEmotionColours);
			ImPlot::PlotPieChart(labels, data, 8, 0.5, 0.5, 0.4);
			ImPlot::PopColormap();

			ImPlot::EndPlot();

		}

		//AGENT NEEDS
		if (ImPlot::BeginPlot("Agent Needs"))
		{
			ImPlot::SetupAxisLimits(ImAxis_X1, time[0], time[0] + 50, ImPlotCond_Always);
			ImPlot::SetupAxes("Time", "Need Values");
			ImPlot::SetupAxisLimits(ImAxis_Y1, -5, 105, ImPlotCond_Always);

			ImPlot::PlotLine("Hunger", time.data(), hungerValues.data(), 400);
			ImPlot::PlotLine("Thirst", time.data(), thirstValues.data(), 400);
			ImPlot::PlotLine("Social", time.data(), socialValues.data(), 400);

			ImPlot::EndPlot();
		}

		//OCEAN VALUES
		if (ImPlot::BeginPlot("Agent Personality Values"))
		{
			ImPlot::SetupAxes("OCEAN Factors", "OCEAN Value Intensities", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
			ImPlot::SetupAxesLimits(0, 5, 0, 5);

			static const double positions[] = { 0,1,2,3,4 };
			static const char* labels[] = { "O", "C", "E", "A", "N" };

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



		ImGui_Implementation::End();
	}

	void MainUI()
	{
		ImGui_Implementation::SetNextWindowPos({ 900, 0 });
		ImGui_Implementation::SetNextWindowSize({ 400, 200 });
		ImGui_Implementation::Begin("Menu", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

		ImGui_Implementation::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui_Implementation::GetIO().Framerate, ImGui_Implementation::GetIO().Framerate);

		if (ImGui_Implementation::Button("Regenerate WFC", {120, 30}))
		{
			WFC::WFCReset();
		}

		if (ImGui_Implementation::Button("Pause", { 120, 30 }))
		{
			pause = !pause;
		}

		ImGui_Implementation::SliderFloat("Speed", &deltaTimeModifier, 0.1f, 10.0f);

		//if (isAgentPressed)
		//{
		//	if (ImGui_Implementation::Button("Select end destination for agent", { 120, 30 }))
		//	{
		//		toSelectDest = true;
		//	}
		//}

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
