#include "WFC.h"
#include "Tile.h"
#include "FoodSource.h"
#include "Agent.h"

namespace WFC
{
	namespace
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

		std::vector<glm::ivec2> unoccupiedTiles{};
		std::vector<FoodSource> foodSources{};
		std::vector<Agent>* agentsRef;
	}

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
		if (typeCounter[0] < 50 || typeCounter[1] < 3 || typeCounter[2] < 3)
		{
			WFCReset(); 
		}

		//check if too many sea tiles
		if (typeCounter[2] > (gridSizeX * gridSizeY) / 3)
		{
			//WFCReset();
		}

		SetEdgesAndCorners('S');
		SetEdgesAndCorners('C');

		gridRef->landTilePositions = gridRef->GetTilesOfType('L');
		gridRef->waterPositions = gridRef->GetTilesOfType('S');

		unoccupiedTiles = gridRef->landTilePositions;

		PlaceFood();
		PlaceDecor();
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
		for (std::vector<Tile>& tiles : gridRef->Tiles)
		{
			for (Tile& t : tiles)
			{
				t.Reset();
			}
		}
		typeCounter = { 0, 0, 0 }; //reset
		std::fill(gridRef->sourceRectPositions.begin(), gridRef->sourceRectPositions.end(), glm::ivec2(0, 0));
		std::fill(gridRef->decorSourceRectPositions.begin(), gridRef->decorSourceRectPositions.end(), glm::ivec2(0, 0));
		
		for (auto& vec : gridRef->sourceRectPositionsCorners)
		{
			vec.clear();
		}

		foodSources.clear();
		
		if (agentsRef != nullptr)
		{
			for (auto& agent : *agentsRef)
			{
				agent.states.foodState.prevFoodPositions.clear();
				agent.states.waterState.prevWaterPositions.clear();
				agent.states.socialState.isTalkingTo = false;
			}
		}

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

	void SetEdgesAndCorners(char tileType)
	{
		char tileToFind = 'C';
		if (tileType == 'C')
		{
			tileToFind = 'L';
		}

		//EDGES + CORNERS
		for (auto& tile : gridRef->GetTilesOfType(tileType))
		{
			std::bitset<4> dirs; //ORDER - UP, RIGHT, DOWN, LEFT
			std::bitset<4> diagonalDirs; //ORDER -TOP RIGHT, BOTTOM RIGHT, BOTTOM LEFT, TOP LEFT

			static std::vector<glm::vec2> offsets = { {0, -1}, {1, 0}, {0, 1}, {-1, 0} };
			int addedDirectionIndexes = 0;
			int diagonalAddedDirectionIndexes = 0;

			glm::ivec2 tilePos = { static_cast<int>(tile.x), static_cast<int>(tile.y) };
			int index = (tilePos.y * gridSizeX) + tilePos.x;

			//check neighbours - up left down right
			for (int i = 0; i < 4; i++)
			{
				Tile* neighbour = gridRef->GetTileFromPos({ tile.x + offsets[i].x, tile.y + offsets[i].y });

				if (neighbour == nullptr) continue;

				if (neighbour->GetType() == tileToFind)
				{
					dirs.set(i);
					addedDirectionIndexes += i + 1;
				}
			}

			//check diagonals - top right, bottom right, bottom left, top left
			static std::vector<glm::vec2> cornerOffsets = { {1, -1}, {1, 1}, {-1, 1}, {-1, -1} };

			for (int i = 0; i < 4; i++)
			{
				Tile* diagonal = gridRef->GetTileFromPos({ tile.x + cornerOffsets[i].x, tile.y + cornerOffsets[i].y });

				if (diagonal == nullptr) continue;

				if (diagonal->GetType() == tileToFind)
				{
					diagonalDirs.set(i);
				}

				if (tileType == 'S' && diagonal->GetType() == 'L')
				{
					gridRef->sourceRectPositionsCorners[index].emplace_back(std::make_pair(i, LAND_OVERLAP));
				}
			}

			glm::ivec2 sourceRectPos{ 0, 0 };

			int numNeighbours = dirs.count();

			switch (numNeighbours)
			{
			case 0:
				sourceRectPos = { 2, 1 };
				break;
			case 1:
			{
				static std::vector<glm::ivec2> positions = { {2, 0}, {3, 1}, {2, 2}, {1, 1} }; //UP DOWN LEFT RIGHT

				for (int i = 0; i < 4; i++)
				{
					if (dirs[i])
					{
						sourceRectPos = positions[i];
						break;
					}
				}
			}
			break;
			case 2:
			{
				static std::vector<glm::ivec2> twoNeighourPositions = { {3, 0}, {2, 3}, {1, 0}, {3, 2}, {0, 1}, {1, 2} }; //UP DOWN LEFT RIGHT 

				if (addedDirectionIndexes == 5)
				{
					if (dirs[0] && dirs[3])
					{
						sourceRectPos = twoNeighourPositions[2];
					}
					else
					{
						sourceRectPos = twoNeighourPositions[3];
					}
				}
				else
				{
					if (addedDirectionIndexes < 5)
					{
						sourceRectPos = twoNeighourPositions[addedDirectionIndexes - 3];
					}
					else
					{
						sourceRectPos = twoNeighourPositions[addedDirectionIndexes - 2];
					}
				}
			}
			break;
			case 3:
			{
				static std::vector<glm::ivec2> positions = { {0, 2}, {1, 3}, {0, 0}, {3, 3} }; //UP RIGHT DOWN LEFT

				for (int i = 0; i < 4; i++)
				{
					if (!dirs[i])
					{
						sourceRectPos = positions[i];
						break;
					}
				}
			}
			break;
			case 4:
				sourceRectPos = { 0, 3 };
				break;
			default:
				break;
			}

			//corners
			for (int i = 0; i < diagonalDirs.size(); i++)
			{
				if (diagonalDirs[i])
				{
					if (tileType == 'S')
					{
						gridRef->sourceRectPositionsCorners[index].emplace_back(std::make_pair(i, WATER_SAND));
					}
					else
					{
						gridRef->sourceRectPositionsCorners[index].emplace_back(std::make_pair(i, LAND_OVERLAP));
					}
				}
			}

			//neighbours
			gridRef->sourceRectPositions[index] = sourceRectPos;
		}
	}

	void PlaceFood()
	{
		for (int i = 0; i < 50 && !unoccupiedTiles.empty(); i++)
		{
			std::uniform_int_distribution<> distrib(0, unoccupiedTiles.size() - 1);
			int index = distrib(RandomGenerator::gen);
			Tile* currentTile = gridRef->GetTileFromPos(unoccupiedTiles[index]);
			if (currentTile != nullptr)
			{
				bool isBlue = true;
				i % 2 == 0 ? isBlue = false : isBlue = true;

				foodSources.emplace_back(gridRef, currentTile->GetGridPos(), isBlue).isActive = true;
				unoccupiedTiles.erase(unoccupiedTiles.begin() + index);
			}
			else
			{
				i--;
			}
		}
	}

	void PlaceDecor()
	{
		if (unoccupiedTiles.empty()) return;

		int decorNumber = floor(unoccupiedTiles.size() / 3);

		for (int i = 0; i < decorNumber; i++)
		{
			std::uniform_int_distribution<> distrib(0, unoccupiedTiles.size() - 1);
			int index = distrib(RandomGenerator::gen);
			Tile* currentTile = gridRef->GetTileFromPos(unoccupiedTiles[index]);

			int decorArrayIndex = (currentTile->GetGridPos().y * gridSizeX) + currentTile->GetGridPos().x;

			if (currentTile != nullptr)
			{
				std::uniform_int_distribution<> decorDistribX(1, 18);
				int decorIndexX = decorDistribX(RandomGenerator::gen);

				if (decorIndexX < 6) //mushrooms
				{
					std::uniform_int_distribution<> decorDistribYMushrooms(1, 3);
					int decorIndexY = decorDistribYMushrooms(RandomGenerator::gen);
					gridRef->decorSourceRectPositions[decorArrayIndex] = glm::ivec2(decorIndexX, decorIndexY);
				}
				else if (decorIndexX > 6 && decorIndexX < 12) //petals
				{
					if (decorIndexX == 11)
					{
						std::uniform_int_distribution<> decorDistribYPetals(1, 2);
						int decorIndexY = decorDistribYPetals(RandomGenerator::gen);

						decorIndexY == 1 ? gridRef->decorSourceRectPositions[decorArrayIndex] = glm::ivec2(decorIndexX, 3) : gridRef->decorSourceRectPositions[decorArrayIndex] = glm::ivec2(decorIndexX, 5);
					}

					std::uniform_int_distribution<> decorDistribYPetals2(2, 5);
					int decorIndexY = decorDistribYPetals2(RandomGenerator::gen);
					gridRef->decorSourceRectPositions[decorArrayIndex] = glm::ivec2(decorIndexX, decorIndexY);
				}
				else if (decorIndexX >= 12) //grass
				{
					std::uniform_int_distribution<> decorDistribGrass(2, 3);
					int decorIndexY = decorDistribGrass(RandomGenerator::gen);

					if (decorIndexY == 3) decorIndexY++;
					gridRef->decorSourceRectPositions[decorArrayIndex] = glm::ivec2(decorIndexX, decorIndexY);
				}
				unoccupiedTiles.erase(unoccupiedTiles.begin() + index);
			}
		}
	}

	std::vector<FoodSource>& GetFoodSources()
	{
		return foodSources;
	}

	void SetAgentsVec(std::vector<Agent>* agents)
	{
		agentsRef = agents;
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


