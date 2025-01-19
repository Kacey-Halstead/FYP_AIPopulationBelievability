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
	"BUSHF"
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
	}

	SDL_Texture* GetTexture(TextureIndexes index)
	{
		return textures[index];
	}
}

