#include "AStar.h"

AStar::AStar(Grid* grid)
{
    gridRef = grid;
}

void AStar::Findpath(Tile* start, Tile* end)
{
	path.clear();

    // TODO: delete all pointers in open and closed list
	std::vector<Node*> OpenList; //not visited
	std::vector<Node*> ClosedList; //visited

    OpenList.emplace_back(new Node(start, nullptr, 0.0f, Heuristic_Manhatten(start, end)));
    Node* current = OpenList[0];

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
		if (current->tile == end)
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

            Tile* neighbour = GetNeighbour(i, current->tile);
            if (neighbour != nullptr && !DoesContainNode(ClosedList, neighbour) && neighbour->walkable) //if not null, if not in visited list and is walkable
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

                        Tile* beforeNode = GetNeighbour(before, current->tile); //node before corner
                        Tile* afterNode = GetNeighbour(after, current->tile); //node after corner

                        if (!beforeNode->walkable || !afterNode->walkable) //if either not walkable, skip. Does not cut corner
                        {
                            continue;
                        }
                    }
                }

                SDL_Point distbetween = neighbour->pos - current->tile->pos;

                float h = Heuristic_Manhatten(neighbour, end);
                float g = current->gcost + Magnitude(distbetween);
                
                float f = g + h;

                if (DoesContainNode(OpenList, neighbour))
                {
                    Node* neighbourInfo = GetNodeInList(OpenList, neighbour);

                    if (neighbourInfo->fcost > f)
                    {
                        neighbourInfo->UpdateInfo(neighbourInfo->tile, current, g, h);
                    }
                }
                else
                {
                    Node* nodeinfo = new Node(neighbour, current, g, h);
                    OpenList.push_back(nodeinfo);
                }
            }
            else
            {
                continue;
            }
        }

        // after checked all directions get next node from open list
        ClosedList.emplace_back(current);
        OpenList.erase(std::find(OpenList.begin(), OpenList.end(), current));

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

float AStar::Heuristic_Manhatten(const Tile* start, const Tile* end) const
{
    SDL_Point distance = end->pos - start->pos;
    float XDiff = abs(distance.x);
    float YDiff = abs(distance.y);
    return XDiff + YDiff;
}

void AStar::ResetTiles(vector<vector<Tile*>>& toReset)
{
    for (std::vector<Tile*>& v : toReset)
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
        Node* next = curNode->parent;
		path.emplace_back(*curNode);
		curNode = next;
	}
	reverse(path.begin(), path.end());
}

void AStar::DrawPath()
{
    for (Node n : path)
    {
        n.tile->isInPath = true;
    }
}

bool AStar::DoesContainNode(const std::vector<Node*>& list, Tile* tile)
{
    for (const Node* n : list)
    {
        if (n->tile == tile)
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

Node* AStar::GetNodeInList(const std::vector<Node*>& list, Tile* tile)
{
    for (Node* n : list)
    {
        if (n->tile == tile)
        {
            return n;
        }
    }

    return nullptr;
}

Node* AStar::GetCheapestNode(std::vector<Node*>& openList)
{
    std::sort(openList.begin(), openList.end(), [](const Node* a, const Node* b)
        {
            return a->fcost < b->fcost;
        });

    return openList[0];
}

Tile* AStar::GetNeighbour(int index, Tile* current)
{
    //0 - up, 1- top right, 2- right, 3- bottom right, 4- bottom, 5-bottom left, 6-left, 7-top left

    static std::array<SDL_Point, 8> offsets = { { {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, { -1, 0} , {-1, 1} } };

    if (gridRef->IsInGrid(current->pos, offsets[index])) //is neighbour in grid
    {
        SDL_Point pos = current->pos + offsets[index];
        return gridRef->Tiles[pos.x][pos.y];
    }

    return nullptr;
}
