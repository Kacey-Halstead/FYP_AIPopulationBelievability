#include "AStar.h"

AStar::AStar()
{
}

void AStar::Findpath(Node start, Node end)
{
	path.clear();

	std::vector<Node> OpenList; //not visited
	std::vector<Node> ClosedList; //visited

	OpenList.emplace_back(start.tile, nullptr, 0.0f, Heuristic_Manhatten(start, end));
    Node* current = &OpenList[0];

	int maxIteration = 0;

	//while current node not empty
	while (current != nullptr)
	{
		maxIteration++;
		if (maxIteration > maxPathCount)
		{
            //max path length reached
			break;
		}

		//if the current node is the end node, a path has been found.
		if (current->tile == end.tile)
		{
			//Debug.Log("Path found, start pos = " + start.transform.position + " - end pos = " + end.transform.position);
			SetPath(current);
			DrawPath();
			return;
		}

        for (int i = 0; i < 8; ++i) //in every direction
        {
            if (!AllowDiagonal) //if not allowing diagonal
            {
                if (i % 2 != 0) //if not orthogonal
                {
                    continue;
                }
            }

            Tile* neighbour = current->tile.neighbourTiles[i];

            if (neighbour != nullptr && !DoesContainNode(ClosedList, *neighbour) && neighbour->walkable) //if not null, if not in visited list and is walkable
            {
                if (!CanCutCorners)
                {
                    if (i % 2 != 0) //if diagonal (1, 3, 5, 7)
                    {
                        int before = i - 1;
                        int after;

                        if (i == 7)
                        {
                            after = 0;
                        }
                        else
                        {
                            after = i + 1;
                        }

                        Tile beforeNode = *current->tile.neighbourTiles[before]; //node before corner
                        Tile afterNode = *current->tile.neighbourTiles[after]; //node after corner
                        if (!beforeNode.walkable || !afterNode.walkable) //if either not walkable, skip. Does not cut corner
                        {
                            continue;
                        }
                    }
                }

                SDL_Point distbetween = neighbour->pos - current->tile.pos;

                float h = Heuristic_Manhatten(*neighbour, end.tile);
                float g = current->gcost + Magnitude(distbetween);
                
                float f = g + h;

                if (DoesContainNode(OpenList, *neighbour))
                {
                    Node* neighbourInfo = GetNodeInList(OpenList, *neighbour);

                    if (neighbourInfo->fcost > f)
                    {
                        neighbourInfo->UpdateInfo(neighbourInfo->tile, current, g, h);
                    }
                }
                else
                {
                    Node nodeinfo = Node(*neighbour, current, g, h);
                    OpenList.emplace_back(nodeinfo);
                }
            }
            else
            {
                continue;
            }

            ClosedList.emplace_back(*current);
            OpenList.erase(std::find(OpenList.begin(), OpenList.end(), *current));

            if (OpenList.size() > 0)
            {
                current = GetCheapestNode(OpenList);
            }
            else
            {
                break; 
            } 
        }
	}
}


float AStar::Heuristic_Manhatten(Node start, Node end)
{
	SDL_Point distance = end.tile.pos - start.tile.pos;
	float XDiff = abs(distance.x);
	float YDiff = abs(distance.y);
	return XDiff + YDiff;
}

float AStar::Heuristic_Manhatten(Tile start, Tile end)
{
    SDL_Point distance = end.pos - start.pos;
    float XDiff = abs(distance.x);
    float YDiff = abs(distance.y);
    return XDiff + YDiff;
}

void AStar::ResetTiles(vector<vector<Tile*>> toReset)
{
    for (std::vector<Tile*> v : toReset)
    {
        for (Tile* t : v)
        {
            t->isInPath = false;
        }
    }
}

void AStar::SetPath(Node* end)
{
    path.clear();
	Node* curNode = end;
	while (curNode != nullptr)
	{
		path.emplace_back(*curNode);
		curNode = curNode->parent;
	}
	reverse(path.begin(), path.end());
}

void AStar::DrawPath()
{
    for (Node n : path)
    {
        n.tile.isInPath = true;
    }
}

bool AStar::DoesContainNode(std::vector<Node> list, Tile tile)
{
    for (Node n : list)
    {
        if (n.tile.pos == tile.pos)
        {
            return true;
        }
    }
    return false;
}

float AStar::Magnitude(SDL_Point s)
{
    float  squared = (s.x * s.x) + (s.y * s.y);
    return sqrt(squared);
}

Node* AStar::GetNodeInList(std::vector<Node> list, Tile tile)
{
    for (Node& n : list)
    {
        if (n.tile.pos == tile.pos)
        {
            return &n;
        }
    }

    return nullptr;
}

Node* AStar::GetCheapestNode(std::vector<Node> openList)
{
    std::sort(openList.begin(), openList.end(), [](Node a, Node b)
        {
            return a.fcost > b.fcost;
        });

    return &openList[0];
}

Tile* AStar::GetNeighbour(int index, Tile* current)
{
    //0 - up, 1- top right, 2- right, 3- bottom right, 4- bottom, 5-bottom left, 6-left, 7-top left

    static std::array<SDL_Point, 8> offsets = { { {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, { -1, 0} , {-1, 1} } };

    SDL_Point pos = current->pos + offsets[index];

    //Getting neighbour so dont have to store it
    Tile* newTile = 

    return nullptr;
}
