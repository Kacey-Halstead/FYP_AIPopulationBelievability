#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include <iostream>

enum TextureIndexes
{
	LAND,
	COAST,
	SEA,
	AGENT
};

namespace TextureManager
{
	bool TextureManagerInit(SDL_Renderer* renderer, SDL_Window* window);
	void DestroyTextureManager();
	bool LoadTextures();
	SDL_Texture* GetTexture(TextureIndexes index);
};

