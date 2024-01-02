#pragma once



using namespace std;



class Coord
{
public:
    int x = -1;
    int y = -1;

    Coord(int ix, int iy)
    {
        this->x = ix;
        this->y = iy;
    }
};



class CoordList
{
public:
    Coord *pos;
    CoordList *next = nullptr;
};



class BFSQueue
{
public:
    CoordList *list = nullptr;
    CoordList *last = nullptr;

    BFSQueue(){};

    ~BFSQueue()
    {
        CoordList *current = this->list;

        while (current != nullptr)
        {
            CoordList *temp = current;
            current = current->next;
            delete temp;
        }
    }

    const bool isEmpty()
    {
        return (this->list == nullptr);
    }

    Coord *pop()
    {
        Coord *pos = nullptr;

        if (!isEmpty())
        {
            CoordList *temp = this->list;

            pos = temp->pos;

            list = temp->next;

            delete temp;
        }

        return pos;
    }

    void add(Coord *pos)
    {
        CoordList *new_node = new CoordList();
        new_node->pos = pos;

        if (this->list == nullptr)
        {
            this->list = new_node;
            this->last = new_node;
        }
        else
        {
            this->last->next = new_node;
            this->last = new_node;
        }
    }
};



bool isInsideMap(int x, int y, int sizeX, int sizeY)
{
    return ((x >= 0) && (x < sizeX) && (y >= 0) && (y < sizeY));
}



bool isValid(char **map, int x, int y, int sizeX, int sizeY)
{
    if (isInsideMap(x, y, sizeX, sizeY))
    {
        return isRoadElement(map[y][x]);
    }

    return false;
}



bool isLonely(char **map, int x, int y, int sizeX, int sizeY)
{
    int adjacentX[4] = {-1, 0, 1, 0};
    int adjacentY[4] = {0, 1, 0, -1};

    for (int i = 0; i < 4; i++)
    {
        int currX = x + adjacentX[i];
        int currY = y + adjacentY[i];

        if (isValid(map, currX, currY, sizeX, sizeY))
        {
            if (isRoadElement(map[currY][currX]))
            {
                return false;
            }
        }
    }

    return true;
}



void areNeighborsLetters(char **map, int x, int y, int sizeX, int sizeY, bool &left, bool &right)
{
    if (isInsideMap(x - 1, y, sizeX, sizeY))
    {
        if (isNameElement(map[y][x - 1]))
        {
            left = true;
        }
    }

    if (isInsideMap(x + 1, y, sizeX, sizeY))
    {
        if (isNameElement(map[y][x + 1]))
        {
            right = true;
        }
    }
}



void findStringIndices(char **map, int x, int y, int sizeX, int sizeY, int &direction, int &startX, int &startY, int &endX)
{
    int adjacentX[8] = {-1, 0, 0, 1, -1, -1, 1, 1};
    int adjacentY[8] = {0, -1, 1, 0, -1, 1, -1, 1};

    for (int i = 0; i < 8; i++)
    {
        if (isInsideMap(x + adjacentX[i], y + adjacentY[i], sizeX, sizeY))
        {
            if (isNameElement(map[y + adjacentY[i]][x + adjacentX[i]]))
            {
                bool letterToTheLeft = false;
                bool letterToTheRight = false;

                areNeighborsLetters(map, x + adjacentX[i], y + adjacentY[i], sizeX, sizeY, letterToTheLeft, letterToTheRight);

                if (letterToTheLeft && letterToTheRight)
                {
                    continue;
                }

                if (letterToTheLeft)
                {
                    direction = -1;
                }

                if (letterToTheRight)
                {
                    direction = 1;
                }

                startX = x + adjacentX[i];
                startY = y + adjacentY[i];

                break;
            }
        }
    }

    endX = startX + direction;

    if (direction != 0)
    {
        while (isInsideMap(endX, startY, sizeX, sizeY) && isNameElement(map[startY][endX]))
        {
            endX += direction;
        }
    }
}



char *findCityName(char **map, int x, int y, int sizeX, int sizeY)
{
    char *name = new char[CITY_STRING_LENGTH];

    int direction = 0;
    int startX = 0;
    int endX = 0;
    int startY = 0;

    findStringIndices(map, x, y, sizeX, sizeY, direction, startX, startY, endX);

    int k = 0;

    if (direction == 0)
    {
        name[k] = map[startY][k + startX];
        k++;
    }

    if (direction > 0)
    {
        for (k = 0; k < endX - startX; k++)
        {
            name[k] = map[startY][k + startX];
        }
    }

    if (direction < 0)
    {
        for (k = 0; k < startX - endX; k++)
        {
            name[k] = map[startY][k + endX + 1];
        }
    }

    name[k] = '\0';

    return name;
}



void BFSFloodFill(char **map, CityHashTable *graph, char *startName, int startX, int startY, int sizeX, int sizeY)
{
    graph->addIfNotExists(startName);
    if (isLonely(map, startX, startY, sizeX, sizeY))
    {
        return;
    }

    int *visited = new int[sizeX * sizeY];

    for (int k = 0; k < (sizeX * sizeY); k++)
    {
        visited[k] = INF_DIST;
    }

    int adjacentX[4] = {-1, 0, 1, 0};
    int adjacentY[4] = {0, 1, 0, -1};

    BFSQueue q;

    Coord *start = new Coord(startX, startY);
    q.add(start);

    visited[startY * sizeX + startX] = 0;

    while (!q.isEmpty())
    {
        Coord *pos = q.pop();

        for (int i = 0; i < 4; i++)
        {
            int currX = pos->x + adjacentX[i];
            int currY = pos->y + adjacentY[i];
            int currPos = currY * sizeX + currX;

            if (isValid(map, currX, currY, sizeX, sizeY))
            {
                if (!wasVisited(visited[currPos]))
                {

                    visited[currPos] = visited[pos->y * sizeX + pos->x] + 1;

                    if (isCity(map[currY][currX]))
                    {
                        char *cityName = findCityName(map, currX, currY, sizeX, sizeY);

                        graph->addConnectionToCity(startName, cityName, visited[currPos]);
                    }
                    else
                    {
                        Coord *next = new Coord(currX, currY);

                        q.add(next);
                    }
                }
            }
        }

        delete pos;
    }

    // delete start;
    delete[] visited;
}

