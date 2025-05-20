#include "TextureManager.h"

namespace TextureManager
{
	SDL_Renderer* renderer = nullptr;
	SDL_Window* window = nullptr;

	std::vector<SDL_Texture*> textures;

	std::vector<std::string> imageNames = {
	"LAND",
	"COAST",
	"SEA",
	"AGENT",
	"PATH",
	"BUSHE",
	"BUSHF",
	"EMOTE_ANGER",
	"EMOTE_ANGRYFACE",
	"EMOTE_DOT1",
	"EMOTE_DOT2",
	"EMOTE_DOT3",
	"EMOTE_DROPS",
	"EMOTE_EXCLAIM",
	"EMOTE_FEAR",
	"EMOTE_HAPPYFACE",
	"EMOTE_HEART",
	"EMOTE_SADFACE",
	"EMOTE_SWIRL",
	"ROCK",
	"TREE",
	"BLUEBUSHF",
	"base_character",
	"world_tiles",
	"bushes_red",
	"bushes_blue",
	"sand_tiles"
	};

	bool TextureManagerInit(SDL_Renderer* mainRenderer, SDL_Window* mainWindow)
	{
		renderer = mainRenderer;
		window = mainWindow;

		if (!LoadTextures())
		{
			std::cout << "Error loading textures";
			return false;
		}
		return true;
	}

	void DestroyTextureManager()
	{
		for (SDL_Texture* tex : textures)
		{
			SDL_DestroyTexture(tex);
		}
	}

	bool LoadTextures()
	{
		for (std::string t : imageNames)
		{
			std::string path = "Images/" + t + ".png";

			SDL_Surface* image = IMG_Load(path.c_str());
			if (image == NULL) {
				std::cout << "Error loading image: " << IMG_GetError();
				return false;
			}

			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
			if (texture == NULL) {
				std::cout << "Error creating texture";
				return false;
			}

			SDL_FreeSurface(image);
			textures.emplace_back(texture);
		}
		return true;
	}

	SDL_Texture* GetTexture(TextureIndexes index)
	{
		return textures[index];
	}
}

