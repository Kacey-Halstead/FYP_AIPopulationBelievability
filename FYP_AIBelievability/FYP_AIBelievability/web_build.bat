emcc ^
main.cpp ^
Action.cpp ^
ActionDefinitions.cpp ^
Agent.cpp ^
AStar.cpp ^
Commons.cpp ^
DAG.cpp ^
FoodSource.cpp ^
FromJSON.cpp ^
FYP_AIBelievability.cpp ^
Grid.cpp ^
ImGuiImplementation.cpp ^
imgui\imgui.cpp ^
imgui\imgui_draw.cpp ^
imgui\imgui_impl_sdl2.cpp ^
imgui\imgui_impl_sdlrenderer2.cpp ^
imgui\imgui_stdlib.cpp ^
imgui\imgui_tables.cpp ^
imgui\imgui_widgets.cpp ^
imgui\implot.cpp ^
imgui\implot_items.cpp ^
PersonalityComponent.cpp ^
RandomGenerator.cpp ^
SDLWindow.cpp ^
States.cpp ^
TextureManager.cpp ^
Tile.cpp ^
WFC.cpp ^
-Iglm ^
-Iimgui\include ^
-std=c++20 ^
--preload-file traits.json ^
--preload-file Images\AGENT.png ^
--preload-file Images\AGENT2.png ^
--preload-file Images\BLUEBUSHF.png ^
--preload-file Images\BUSHE.png ^
--preload-file Images\BUSHF.png ^
--preload-file Images\COAST.png ^
--preload-file Images\EMOTE_ANGER.png ^
--preload-file Images\EMOTE_ANGRYFACE.png ^
--preload-file Images\EMOTE_DOT1.png ^
--preload-file Images\EMOTE_DOT2.png ^
--preload-file Images\EMOTE_DOT3.png ^
--preload-file Images\EMOTE_DROPS.png ^
--preload-file Images\EMOTE_EXCLAIM.png ^
--preload-file Images\EMOTE_FEAR.png ^
--preload-file Images\EMOTE_HAPPYFACE.png ^
--preload-file Images\EMOTE_HEART.png ^
--preload-file Images\EMOTE_SADFACE.png ^
--preload-file Images\EMOTE_SWIRL.png ^
--preload-file Images\LAND.png ^
--preload-file Images\PATH.png ^
--preload-file Images\ROCK.png ^
--preload-file Images\SEA.png ^
--preload-file Images\TREE.png ^
-sUSE_SDL=2 ^
-sUSE_SDL_IMAGE=2 ^
-sSDL2_IMAGE_FORMATS="png" ^
-o.\build\index.html ^
-sALLOW_MEMORY_GROWTH=1 ^
-O3