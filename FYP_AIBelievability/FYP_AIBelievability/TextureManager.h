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
	AGENT,
	PATH,
	BUSHE,
	BUSHF,
	EMOTE_ANGER,
	EMOTE_ANGRYFACE,
	EMOTE_DOT1,
	EMOTE_DOT2,
	EMOTE_DOT3,
	EMOTE_DROPS,
	EMOTE_EXCLAIM,
	EMOTE_FEAR,
	EMOTE_HAPPYFACE,
	EMOTE_HEART,
	EMOTE_SADFACE,
	EMOTE_SWIRL,
	ROCK,
	TREE,
	BLUE_BUSH,
	AGENT_SPRITES,
	WORLD,
	BUSHES_RED,
	BUSHES_BLUE,
	NO_EMOTE
};

namespace TextureManager
{
	bool TextureManagerInit(SDL_Renderer* renderer, SDL_Window* window);
	void DestroyTextureManager();
	bool LoadTextures();
	SDL_Texture* GetTexture(TextureIndexes index);
}

