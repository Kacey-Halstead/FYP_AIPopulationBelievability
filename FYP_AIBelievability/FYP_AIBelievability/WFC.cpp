#include "WFC.h"

WFC::WFC(int GridX, int GridY, SDL_Window* window, SDL_Renderer* renderer)
{
	gridX = GridX;
	gridY = GridY;
	SDLWindowRef = window;

	for (std::string t : fullTypes)
	{
		std::string path = "Images/" + t + ".png";
		char charType= t[0];
		charTypes.emplace_back(charType);
	}

	grid = new Grid(gridX, gridY, charTypes);
}

void WFC::DefineRules()
{
	//S to right of C
	//C to right L
	Rules.emplace_back(GenerateRule('C', 'S', 'R'));
	Rules.emplace_back(GenerateRule('S', 'C', 'L'));

	Rules.emplace_back(GenerateRule('C', 'L', 'L'));
	Rules.emplace_back(GenerateRule('L', 'C', 'R'));
}

void WFC::WFCBody()
{
	int count = 0;

	while (count < (gridX * gridY))
	{
		Tile* selectedTile = grid->SmallestEntropy();

		//assign cell to type at random
		int randomType = rand() % selectedTile->availableTypes.size();
		//if between 0 and 10, rand num between. 5 equal. Land 6, Sea 4.
		selectedTile->SetType(selectedTile->availableTypes[randomType]);
		selectedTile->availableTypes.clear();

		//update all neighbours(up, down, left, right)
		Evaluate(grid, selectedTile, 'U');
		Evaluate(grid, selectedTile, 'D');
		Evaluate(grid, selectedTile, 'L');
		Evaluate(grid, selectedTile, 'R');

		count++;
	}
}

void WFC::Evaluate(Grid* grid, Tile* tile, char dir)
{
	Tile* neighbour = nullptr;

	int x_index = tile->pos[0];
	int y_index = tile->pos[1];

	bool upCondition = tile->pos[1] - 1 >= 0;
	bool downCondition = tile->pos[1] + 1 < gridY;
	bool leftCondition = tile->pos[0] - 1 >= 0;
	bool rightCondition = tile->pos[0] + 1 < gridX;

	switch (dir)
	{
	case 'U':
		if (upCondition)
		{
			neighbour = grid->Tiles[x_index][y_index - 1]; //up

		}
		break;
	case 'D':
		if (downCondition)
		{
			neighbour = grid->Tiles[x_index][y_index + 1]; //down
		}
		break;
	case 'L':
		if (leftCondition)
		{

			neighbour = grid->Tiles[x_index - 1][y_index]; //left
		}
		break;
	case 'R':
		if (rightCondition)
		{
			neighbour = grid->Tiles[x_index + 1][y_index]; //right
		}
		break;
	default:
		break;
	}

	if (neighbour == nullptr) return;

	for (char t : GetTypeAndRules(tile->type, dir))
	{
		//remove option from neighbours available tiles list
		auto it = find(neighbour->availableTypes.begin(), neighbour->availableTypes.end(), t);
		if (it != neighbour->availableTypes.end() && neighbour->type == '0')
		{
			neighbour->availableTypes.erase(it);
		}
	}

	//if neighbour has no options left, clear that tile and its neighbours
	if (neighbour->availableTypes.size() == 0 && neighbour->type == '0')
	{
		vector<Tile*> toReset;
		toReset.push_back(neighbour);
		toReset.push_back(grid->Tiles[tile->pos[0]][tile->pos[1] - 1]); //up
		toReset.push_back(grid->Tiles[tile->pos[0]][tile->pos[1] + 1]); //down
		toReset.push_back(grid->Tiles[tile->pos[0] - 1][tile->pos[1]]); //left
		toReset.push_back(grid->Tiles[tile->pos[0] + 1][tile->pos[1]]); //right

		ResetNeighbours(toReset);
	}

}

void WFC::ResetNeighbours(vector<Tile*> tiles)
{
	for (Tile* t : tiles)
	{
		t->Reset();
	}
}

void WFC::RenderWFC(SDL_Renderer* renderer)
{
	//stores all rects to render tiles to
	vector<SDL_Rect> rects;
	for (int x = 0; x < gridX; x++)
	{
		for (int y = 0; y < gridY; y++)
		{
			SDL_Rect newRec{ x * (SDL_GetWindowSurface(SDLWindowRef)->w / gridX), y * (SDL_GetWindowSurface(SDLWindowRef)->h / gridY), SDL_GetWindowSurface(SDLWindowRef)->w / gridX, SDL_GetWindowSurface(SDLWindowRef)->h / gridY };
			rects.push_back(newRec);
		}

		int counter = 0;
		for (int x = 0; x < gridX; x++)
		{
			for (int y = 0; y < gridY; y++)
			{
				switch (grid->Tiles[x][y]->type)
				{
				case 'S':
					SDL_RenderCopy(renderer, textures[2], NULL, &rects[counter]);
					break;
				case 'L':
					SDL_RenderCopy(renderer, textures[0], NULL, &rects[counter]);
					break;
				case 'C':
					SDL_RenderCopy(renderer, textures[1], NULL, &rects[counter]);
					break;
				default:
					break;
				}
				counter++;
			}
		}
	}
}

std::vector<char> WFC::GetTypeAndRules(char input, char dir)
{
	std::vector<char> toRemove;
	for (std::array<char, 3> r : Rules)
	{
		if (dir == r[2])
		{
			toRemove.emplace_back(r[1]);
		}
	}

	return toRemove;
}



void WFC::DeleteTextures()
{
	for (SDL_Texture* tex : textures)
	{
		SDL_DestroyTexture(tex);
	}
}

void WFC::CreateTextures(SDL_Renderer* renderer)
{
	//creates textures and inserts into vector
	for (std::string t : fullTypes)
	{
		std::string path = "Images/" + t + ".png";
		SDL_Texture* texture = CreateTexture((path).c_str(), renderer);
		textures.emplace_back(texture);
	}
}

SDL_Texture* WFC::CreateTexture(const char* filePath, SDL_Renderer* renderer)
{
	SDL_Surface* IMG = IMG_Load(filePath);
	if (IMG == NULL) {
		std::cout << "Error loading image: " << IMG_GetError();
		return nullptr;
	}

	SDL_Texture* TEX = SDL_CreateTextureFromSurface(renderer, IMG);
	if (TEX == NULL) {
		std::cout << "Error creating texture";
		return nullptr;
	}

	SDL_FreeSurface(IMG);
	return TEX;
}