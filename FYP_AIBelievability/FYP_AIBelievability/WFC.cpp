#include "WFC.h"
#include "Tile.h"

namespace WFC
{
	Grid* gridRef = nullptr;

	std::array<int, 3> typeCounter = {}; //L, C, S

	std::vector<glm::ivec2> offsets = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} }; //down, up, left, right

	std::vector<std::array<char, 3>> rules = {

	{'L', 'S', 'U'}, //L and S can never touch
	{'L', 'S', 'D'},
	{'L', 'S', 'L'},
	{'L', 'S', 'R'},

	{'S', 'L', 'U'},
	{'S', 'L', 'D'},
	{'S', 'L', 'L'},
	{'S', 'L', 'R'}
	};

	void WFCBody(Grid* grid)
	{
		gridRef = grid;
		int count = 0;

		while (!EveryTileHasType())
		{
			Tile* selectedTile = gridRef->SmallestEntropy();
			ChangeTileWeighting(selectedTile);

			//assign cell to type at random with weighting
			std::vector<char> random;
			for (pair<char, float> p : selectedTile->typesAndWeights)
			{
				//creates vector with (type * weight) * number of types
				int counter = p.second;
				while (counter > 0)
				{
					random.emplace_back(p.first);
					counter--;
				}
			}

			std::uniform_int_distribution<> distrib(0, random.size()-1);
			int randomType = distrib(RandomGenerator::gen);

			selectedTile->SetType(random[randomType]);
			if (random[randomType] == 'S')
			{
				selectedTile->walkable = false;
				grid->waterPositions.push_back(selectedTile->GetGridPos());
			}

			TypeIncrement(random[randomType]);
			random.clear();

			selectedTile->typesAndWeights.clear();

			//update all neighbours(up, down, left, right)
			Evaluate(selectedTile, UP);
			Evaluate(selectedTile, DOWN);
			Evaluate(selectedTile, LEFT);
			Evaluate(selectedTile, RIGHT);
		}

		AStar::SetGridRef(gridRef);

		//check if enough of each tile. if not, regenerate
		if (typeCounter[0] < 3 || typeCounter[1] < 3 || typeCounter[2] < 3)
		{
			//WFCReset();
		}

		//check if too many sea tiles
		if (typeCounter[2] > (gridSizeX * gridSizeY) / 3)
		{
			//WFCReset();
		}

		gridRef->landTilePositions = gridRef->GetLandTiles();

		for (int i = 0; i < 10 && !gridRef->landTilePositions.empty(); i++)
		{
			// need to make sure can't gen on same tile
			std::uniform_int_distribution<> distrib(0, gridRef->landTilePositions.size() - 1);
			int index = distrib(RandomGenerator::gen);
			gridRef->rocks.push_back(gridRef->landTilePositions[index]);
			gridRef->landTilePositions.erase(gridRef->landTilePositions.begin() + index);
		}
	}

	bool IsInGrid(const glm::ivec2& pos, const glm::ivec2& offset)
	{
		SDL_Point newPos = { pos.x + offset.x, pos.y + offset.y };

		if (newPos.x < 0 || newPos.x >= gridSizeX || newPos.y < 0 || newPos.y >= gridSizeY)
		{
			return false;
		}
		return true;
	}

	void Evaluate(Tile* tile, directions dir)
	{
		glm::ivec2 tilePos = { tile->GetGridPos().x, tile->GetGridPos().y };
		glm::ivec2 offset = { 0, 0 };
		offset = offsets[dir];

		if (!IsInGrid(tilePos, offset)) return; //if not valid pos in grid

		Tile* neighbour = &gridRef->Tiles[tilePos.x + offset.x][tilePos.y + offset.y];

		std::vector<char> typesToRemove = GetTypeAndRules(tile->GetType(), dir);

		for (char t : typesToRemove)
		{
			FindAndErase(neighbour, t);
		}

		CheckForEmptyTiles(neighbour);
	}


	void WFCReset()
	{
		for (std::vector<Tile> tiles : gridRef->Tiles)
		{
			for (Tile& t : tiles)
			{
				t.Reset();
			}
		}
		typeCounter = { 0, 0, 0 }; //reset

		WFCBody(gridRef);
	}

	std::vector<char> GetTypeAndRules(char input, directions dir)
	{
		std::vector<char> toRemove; //vector of types to remove
		static std::array<char, 4> dirs = { 'U', 'D', 'L', 'R' };

		for (std::array<char, 3> r : rules)
		{
			if (dirs[dir] == r[2] && r[0] == input) //if needed direction and type equal to current set type, remove type in second place in rule
			{
				toRemove.emplace_back(r[1]);
			}
		}

		return toRemove;
	}

	void ChangeTileWeighting(Tile* tile)
	{
		Tile* neighbour = nullptr;

		for (int i = 0; i < 4; i++)
		{
			if (!IsInGrid(tile->GetGridPos(), offsets[i])) continue;

			neighbour = &gridRef->Tiles[tile->GetGridPos().x + offsets[i].x][tile->GetGridPos().y + offsets[i].y];
			char type = neighbour->GetType();
			if (type == '0') continue;

			auto it = std::find_if(tile->typesAndWeights.begin(), tile->typesAndWeights.end(), [type](const auto& p) {
				return p.first == type;
				});

			if (it != tile->typesAndWeights.end()) //is neighbour type = potential type of current tile?
			{
				tile->UpdateTypeandWeight(type, 2);

			}
		}
	}

	void FindAndErase(Tile* tile, char toFind)
	{
		auto it = std::find_if(tile->typesAndWeights.begin(), tile->typesAndWeights.end(), [toFind](const auto& p) {
			return p.first == toFind;
			});

		if (it != tile->typesAndWeights.end() && tile->GetType() == '0')
		{
			tile->typesAndWeights.erase(it);
		}
	}

	void CheckForEmptyTiles(Tile* tile)
	{
		//if neighbour has no options left, clear that tile and its neighbours
		if (tile->typesAndWeights.size() == 0 && tile->GetType() == '0')
		{
			tile->Reset();

			for (int i = 0; i < 4; i++)
			{
				if (!IsInGrid(tile->GetGridPos(), offsets[i])) continue;
				gridRef->Tiles[tile->GetGridPos().x + offsets[i].x][tile->GetGridPos().y + offsets[i].y].Reset();
			}
		}
	}

	void TypeIncrement(char typeToIncrement)
	{
		switch (typeToIncrement)
		{
		case 'L':
			typeCounter[0]++;
			break;
		case 'C':
			typeCounter[1]++;
			break;
		case 'S':
			typeCounter[2]++;
			break;
		default:
			break;
		}
	}

	bool EveryTileHasType()
	{
		for (int x = 0; x < gridSizeX; x++)
		{
			for (int y = 0; y < gridSizeY; y++)
			{
				if (gridRef->Tiles[x][y].GetType() == '0')
				{
					return false;
				}
			}
		}
		return true;
	}


	SDL_Point operator*(const SDL_Point& a, const int& b)
	{
		SDL_Point p = { a.x * b, a.y * b };
		return p;
	}
}


