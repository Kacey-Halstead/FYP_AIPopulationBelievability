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
--embed-file traits.json ^
--embed-file Images\AGENT.png ^
--embed-file Images\AGENT2.png ^
--embed-file Images\BLUEBUSHF.png ^
--embed-file Images\BUSHE.png ^
--embed-file Images\BUSHF.png ^
--embed-file Images\COAST.png ^
--embed-file Images\EMOTE_ANGER.png ^
--embed-file Images\EMOTE_ANGRYFACE.png ^
--embed-file Images\EMOTE_DOT1.png ^
--embed-file Images\EMOTE_DOT2.png ^
--embed-file Images\EMOTE_DOT3.png ^
--embed-file Images\EMOTE_DROPS.png ^
--embed-file Images\EMOTE_EXCLAIM.png ^
--embed-file Images\EMOTE_FEAR.png ^
--embed-file Images\EMOTE_HAPPYFACE.png ^
--embed-file Images\EMOTE_HEART.png ^
--embed-file Images\EMOTE_SADFACE.png ^
--embed-file Images\EMOTE_SWIRL.png ^
--embed-file Images\LAND.png ^
--embed-file Images\PATH.png ^
--embed-file Images\ROCK.png ^
--embed-file Images\SEA.png ^
--embed-file Images\TREE.png ^
--embed-file Images\bushes.png ^
--embed-file Images\base_character.png ^
--embed-file Images\bushes_red.png ^
--embed-file Images\bushes_blue.png ^
--embed-file Images\bushesh_pink.png ^
--embed-file Images\world_tiles.png ^
--embed-file Images\sandwaterwithmiddle.png ^
--embed-file Images\sand_water_tiles.png ^
--embed-file Images\sand_tiles.png ^
--embed-file Images\land_coast_overlap.png ^
--embed-file Images\bushRED.png ^
--embed-file Images\bushBLUE.png ^
--embed-file Images\agent_single.png ^
-sUSE_SDL=2 ^
-sUSE_SDL_IMAGE=2 ^
-sSINGLE_FILE=1 ^
-sSDL2_IMAGE_FORMATS="png" ^
-o.\build\index.html ^
-sALLOW_MEMORY_GROWTH=1 ^
-O3