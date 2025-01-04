#include "WFC.h"

WFC::WFC(Grid* grid)
{
	gridX = grid->sizeX;
	gridY = grid->sizeY;
	gridRef = grid;
}

WFC::~WFC()
{
	delete gridRef;
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
		Tile* selectedTile = gridRef->SmallestEntropy();

		//assign cell to type at random
		int randomType = rand() % selectedTile->availableTypes.size();
		//if between 0 and 10, rand num between. 5 equal. Land 6, Sea 4.
		selectedTile->SetType(selectedTile->availableTypes[randomType]);
		selectedTile->availableTypes.clear();

		//update all neighbours(up, down, left, right)
		Evaluate(selectedTile, 'U');
		Evaluate(selectedTile, 'D');
		Evaluate(selectedTile, 'L');
		Evaluate(selectedTile, 'R');

		count++;
	}
}

void WFC::Evaluate(Tile* tile, char dir)
{
	Tile* neighbour = nullptr;

	int x_index = tile->pos.x;
	int y_index = tile->pos.y;

	bool upCondition = tile->pos.y - 1 >= 0;
	bool downCondition = tile->pos.y + 1 < gridY;
	bool leftCondition = tile->pos.x - 1 >= 0;
	bool rightCondition = tile->pos.x + 1 < gridX;

	switch (dir)
	{
	case 'U':
		if (upCondition)
		{
			neighbour = gridRef->Tiles[x_index][y_index - 1]; //up

		}
		break;
	case 'D':
		if (downCondition)
		{
			neighbour = gridRef->Tiles[x_index][y_index + 1]; //down
		}
		break;
	case 'L':
		if (leftCondition)
		{

			neighbour = gridRef->Tiles[x_index - 1][y_index]; //left
		}
		break;
	case 'R':
		if (rightCondition)
		{
			neighbour = gridRef->Tiles[x_index + 1][y_index]; //right
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
		toReset.push_back(gridRef->Tiles[tile->pos.x][tile->pos.y - 1]); //up
		toReset.push_back(gridRef->Tiles[tile->pos.x][tile->pos.y + 1]); //down
		toReset.push_back(gridRef->Tiles[tile->pos.x - 1][tile->pos.y]); //left
		toReset.push_back(gridRef->Tiles[tile->pos.x + 1][tile->pos.y]); //right

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
	int counter = 0;
	for (int x = 0; x < gridX; x++)
	{
		for (int y = 0; y < gridY; y++)
		{
			if (gridRef->Tiles[x][y]->isInPath)
			{
				SDL_RenderCopy(renderer, TextureManager::GetTexture(PATH), NULL, &rects[counter]);
				counter++;
				continue;
			}

			switch (gridRef->Tiles[x][y]->type)
			{
			case 'S':		
				SDL_RenderCopy(renderer, TextureManager::GetTexture(SEA), NULL, &rects[counter]);
				break;
			case 'L':
				SDL_RenderCopy(renderer, TextureManager::GetTexture(LAND), NULL, &rects[counter]);
				break;
			case 'C':
				SDL_RenderCopy(renderer, TextureManager::GetTexture(COAST), NULL, &rects[counter]);
				break;
			default:
				break;
			}
			counter++;
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

void WFC::CreateRects(SDL_Window* SDLWindowRef)
{
	for (int x = 0; x < gridX; x++)
	{
		for (int y = 0; y < gridY; y++)
		{
			SDL_Rect newRec{ x * (SDL_GetWindowSurface(SDLWindowRef)->w / gridX), y * (SDL_GetWindowSurface(SDLWindowRef)->h / gridY), SDL_GetWindowSurface(SDLWindowRef)->w / gridX, SDL_GetWindowSurface(SDLWindowRef)->h / gridY };
			rects.emplace_back(newRec);
		}
	}
}

bool WFC::IsInTile(SDL_Point p, Tile t)
{
	return SDL_PointInRect(&p, &rects[t.index]);
}

vector<vector<Tile*>> WFC::GetTiles()
{
	return gridRef->Tiles;
}
