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

void WFC::WFCBody()
{
	int count = 0;

	while (count < (gridX * gridY))
	{
		Tile* selectedTile = gridRef->SmallestEntropy();
		ChangeTileWeighting(selectedTile);

		if (count == 0) //first iteration
		{
			selectedTile->SetType('S');
		}
		else
		{
			//assign cell to type at random
			size_t RandomWeights = 0;
			std::vector<char> random;
			for (pair<char, float> p : selectedTile->typesAndWeights)
			{
				int counter = p.second;
				while (counter > 0)
				{
					random.emplace_back(p.first);
					counter--;
				}
			}

			int randomType = rand() % random.size();
		
			selectedTile->SetType(random[randomType]);
		}
		selectedTile->typesAndWeights.clear();

		//update all neighbours(up, down, left, right)
		Evaluate(selectedTile, UP);
		Evaluate(selectedTile, DOWN);
		Evaluate(selectedTile, LEFT);
		Evaluate(selectedTile, RIGHT);

		count++;
	}
}

bool WFC::IsInGrid(const SDL_Point& pos, const SDL_Point& offset)
{
	SDL_Point newPos = { pos.x + offset.x, pos.y+offset.y };

	if (newPos.x < 0 || newPos.x >= gridRef->sizeX || newPos.y < 0 || newPos.y >= gridRef->sizeY)
	{
		return false;
	}
	return true;
}

void WFC::Evaluate(Tile* tile, directions dir)
{
	SDL_Point tilePos = { tile->pos.x, tile->pos.y };
	SDL_Point offset = { 0, 0 };

	switch (dir)
	{
	case UP:
		offset.y = 1; //up
		break;
	case DOWN:
		offset.y = -1; //down
		break;
	case LEFT:
		offset.x = -1; //left
		break;
	case RIGHT:
		offset.x = 1; //right
		break;
	default:
		break;
	}

	if (!IsInGrid(tilePos, offset)) return; //if not valid pos in grid
	
	Tile* neighbour = gridRef->Tiles[tilePos.x + offset.x][tilePos.y + offset.y];

	if (neighbour == nullptr) return;

	std::vector<char> typesToRemove = GetTypeAndRules(tile->type, dir);

	for (char t : typesToRemove)
	{
		FindAndErase(neighbour, t);
	}

	//if (tile->type == 'C' && (neighbour->type == 'S' || neighbour->type == 'L'))
	//{
	//	SDL_Point oppositeOffset = offset * -1;
	//	if (IsInGrid(tilePos, oppositeOffset))
	//	{
	//		Tile* oppositeneighbour = gridRef->Tiles[tilePos.x + oppositeOffset.x][tilePos.y + oppositeOffset.y];
	//		FindAndErase(oppositeneighbour, 'C');
	//	}
	//}

	CheckForEmptyTiles(neighbour);
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

std::vector<char> WFC::GetTypeAndRules(char input, directions dir)
{
	std::vector<char> toRemove; //vector of types to remove
	std::array<char, 4> dirs = { 'U', 'D', 'L', 'R' };

	for (std::array<char, 3> r : rules.ruleVec)
	{
		if (dirs[dir] == r[2] && r[0] == input) //if needed direction, remove type in second place in rule
		{
			toRemove.emplace_back(r[1]);
		}
	}

	return toRemove;
}

void WFC::ChangeTileWeighting(Tile* tile)
{
	Tile* neighbour = nullptr;

	for (int i = 0; i < 4; i++)
	{
		if (!IsInGrid(tile->pos, offsets[i])) continue;

		neighbour = gridRef->Tiles[tile->pos.x + offsets[i].x][tile->pos.y + offsets[i].y];
		char type = neighbour->type;
		if (type == '0') continue;

		auto it = std::find_if(tile->typesAndWeights.begin(), tile->typesAndWeights.end(), [type](const auto& p) {
			return p.first == type;
			});

		if (it != tile->typesAndWeights.end())
		{
			tile->UpdateTypeandWeight(type, 1);
		}
	}
}

void WFC::FindAndErase(Tile* tile, char toFind)
{
	auto it = std::find_if(tile->typesAndWeights.begin(), tile->typesAndWeights.end(), [toFind](const auto& p) {
		return p.first == toFind;
		});

	if (it != tile->typesAndWeights.end() && tile->type == '0')
	{
		tile->typesAndWeights.erase(it);
	}
}

void WFC::CheckForEmptyTiles(Tile* tile)
{
	//if neighbour has no options left, clear that tile and its neighbours
	if (tile->typesAndWeights.size() == 0 && tile->type == '0')
	{
		vector<Tile*> toReset;
		toReset.push_back(tile);

		for (int i = 0; i < 4; i++)
		{
			if (!IsInGrid(tile->pos, offsets[i])) continue;
			toReset.push_back(gridRef->Tiles[tile->pos.x + offsets[i].x][tile->pos.y + offsets[i].y]);
		}
		ResetNeighbours(toReset);
	}
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

SDL_Point operator*(const SDL_Point& a, const int& b)
{
	SDL_Point p = { a.x * b, a.y * b };
	return p;
}
