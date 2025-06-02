#include "AStar.h"
#include "Tile.h"

namespace AStar
{
    namespace
    {
        std::vector<Node> path;
        bool CanCutCorners = false;
        bool AllowDiagonal = true;

        int maxPathCount = 1000;
        Grid* gridRef = nullptr;
    }

    void SetPath(Node* end)
    {
        path.clear();
        Node* curNode = end;

        if (curNode->tile->GetType() == 'S')
        {
            curNode = curNode->parent;
        }

        while (curNode != nullptr)
        {
            Node* next = curNode->parent;
            path.emplace_back(*curNode);
            curNode = next;
        }
        reverse(path.begin(), path.end());
    }

    std::vector<Node> FindPath(const glm::vec2& start, const glm::vec2& end)
    {
        Tile* tile = gridRef->GetTileFromPos(start);
        Tile* endTile = gridRef->GetTileFromPos(end);

        if (tile == nullptr || endTile == nullptr)
        {
            return std::vector<Node>();
        }

        return FindPath(tile, endTile);
    }

    bool DoesContainNode(std::vector<std::shared_ptr<Node>>& list, Tile* tile)
    {
        for (const std::shared_ptr<Node>& n : list)
        {
            if (n.get()->tile == tile)
            {
                return true;
            }
        }
        return false;
    }

    float Magnitude(SDL_Point s)
    {
        float  squared = (s.x * s.x) + (s.y * s.y);
        return sqrt(squared);
    }

    Node* GetNodeInList(const std::vector<std::shared_ptr<Node>>& list, Tile* tile)
    {
        for (const std::shared_ptr<Node>& n : list)
        {
            if (n->tile == tile)
            {
                return n.get();
            }
        }

        return nullptr;
    }

    std::shared_ptr<Node> GetCheapestNode(std::vector<std::shared_ptr<Node>>& openList)
    {
        std::sort(openList.begin(), openList.end(), [](const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b)
            {
                return a->fcost < b->fcost;
            });

        return openList[0];
    }

    Tile* GetNeighbour(int index, Tile* current)
    {
        //0 - up, 1- top right, 2- right, 3- bottom right, 4- bottom, 5-bottom left, 6-left, 7-top left

        static std::array<glm::ivec2, 8> offsets = { { {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, { -1, 0} , {-1, 1} } };

        if (gridRef->IsInGrid(current->GetGridPos(), offsets[index])) //is neighbour in grid
        {
            glm::ivec2 pos = current->GetGridPos() + offsets[index];
            return &gridRef->Tiles[pos.x][pos.y];
        }

        return nullptr;
    }

    float Heuristic_Manhatten(Tile* start, Tile* end)
    {
        glm::ivec2 distance = end->GetGridPos() - start->GetGridPos();
        float XDiff = abs(distance.x);
        float YDiff = abs(distance.y);
        return XDiff + YDiff;
    }

    std::vector<Node> FindPath(Tile* start, Tile* end)
    {
        path.clear();

        std::vector<std::shared_ptr<Node>> OpenList; //not visited
        std::vector<std::shared_ptr<Node>> ClosedList; //visited
        OpenList.emplace_back(std::make_shared<Node>(start, nullptr, 0.0f, Heuristic_Manhatten(start, end)));
        std::shared_ptr<Node> current = OpenList[0];

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
                SetPath(current.get());
                return path;
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
                if (neighbour != nullptr && !DoesContainNode(ClosedList, neighbour) && (neighbour->IsWalkable() || neighbour == end)) //if not null, if not in visited list and is walkable
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

                            if (!beforeNode->IsWalkable() || !afterNode->IsWalkable()) //if either not walkable, skip. Does not cut corner
                            {
                                continue;
                            }
                        }
                    }

                    glm::vec2 distbetween = neighbour->GetGridPos() - current->tile->GetGridPos();

                    float h = Heuristic_Manhatten(neighbour, end);
                    float g = current->gcost + glm::length(distbetween);

                    float f = g + h;

                    if (DoesContainNode(OpenList, neighbour))
                    {
                        Node* neighbourInfo = GetNodeInList(OpenList, neighbour);

                        if (neighbourInfo->fcost > f)
                        {
                            neighbourInfo->UpdateInfo(neighbourInfo->tile, current.get(), g, h);
                        }
                    }
                    else
                    {
                        OpenList.emplace_back(std::make_shared<Node>(neighbour, current.get(), g, h));
                    }
                }
                else
                {
                    continue;
                }
            }

            // after checked all directions get next node from open list
            ClosedList.push_back(current);
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

        return path;
    }



    

    void SetGridRef(Grid* grid)
    {
        gridRef = grid;
    }
}


