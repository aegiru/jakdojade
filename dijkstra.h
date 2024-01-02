#pragma once



using namespace std;



struct Edge
{
    int distance;
    City *target;
};



class DijkstraQueue
{
public:
    Edge *heap;
    int size;
    int capacity;

    DijkstraQueue(int cap)
    {
        this->capacity = cap;
        this->heap = new Edge[cap];
        this->size = 0;
    }

    ~DijkstraQueue()
    {
        delete[] this->heap;
    }

    const bool isEmpty()
    {
        return (this->size == 0);
    }

    void resize(int newCap)
    {
        Edge *newHeap = new Edge[newCap];
        for (int i = 0; i < this->size; i++)
        {
            newHeap[i] = this->heap[i];
        }
        delete[] this->heap;
        this->heap = newHeap;
        capacity = newCap;
    }

    static void swap(Edge &one, Edge &two)
    {
        Edge temp = one;
        one = two;
        two = temp;
    }

    void bubbleUp(int i)
    {
        while (i > 0 && (this->heap[(i - 1) / 2].distance > this->heap[i].distance))
        {
            swap(this->heap[i], this->heap[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    void bubbleDown(int i)
    {
        while (2 * i + 1 < this->size)
        {
            int child = 2 * i + 1;
            if (child + 1 < this->size && this->heap[child + 1].distance < this->heap[child].distance)
            {
                child++;
            }

            if (this->heap[i].distance > this->heap[child].distance)
            {
                swap(this->heap[i], this->heap[child]);
                i = child;
            }
            else
            {
                break;
            }
        }
    }

    void add(int dist, City *target)
    {
        if (this->size == this->capacity)
        {
            resize(this->capacity * 2);
        }
        Edge next;
        next.distance = dist;
        next.target = target;
        this->heap[this->size++] = next;
        bubbleUp(this->size - 1);
    }

    Edge pop()
    {
        if (isEmpty())
        {
            Edge none = Edge();
            none.distance = -1;
            none.target = nullptr;
            return none;
        }
        else
        {
            Edge node = this->heap[0];
            this->heap[0] = this->heap[--size];
            bubbleDown(0);
            return node;
        }
    }
};



void DijkstraSearch(CityHashTable *graph, char *startName, char *targetName, int verbose)
{
    if (strcmp(startName, targetName) == 0)
    {
        cout << 0 << endl;
        return;
    }

    VisitHashTable *visited = new VisitHashTable(graph->capacity);
    DistHashTable *distances = new DistHashTable(graph->capacity);
    DijkstraQueue queue(DIJKSTRA_QUEUE_SIZE);

    queue.add(0, graph->get(startName));
    distances->set(startName, 0);

    while (!queue.isEmpty())
    {
        Edge next = queue.pop();
        char *currName = next.target->name;

        if (strcmp(currName, targetName) == 0)
        {
            cout << distances->get(currName);

            if (verbose == 1)
            {
                CityNameList *travelPoints = nullptr;

                City *city = graph->get(currName)->prev;
                while (city != nullptr)
                {
                    if (strcmp(city->name, startName) == 0)
                    {
                        break;
                    }

                    addName(&travelPoints, city->name);
                    city = city->prev;
                }

                if (travelPoints != nullptr)
                {
                    CityNameList *travelled = travelPoints;

                    while (travelled != nullptr)
                    {
                        cout << " " << travelled->name;
                        travelled = travelled->next;
                    }

                    deleteNames(&travelPoints);
                }
            }

            cout << endl;

            break;
        }

        if (visited->get(currName))
        {
            continue;
        }

        visited->set(currName, true);

        CityAdjacencyList *neighbor = graph->get(currName)->neighbors;
        while (neighbor != nullptr)
        {
            char *neighborName = neighbor->target->name;
            int newDist = distances->get(currName) + neighbor->distance;

            if (!visited->get(neighborName) && newDist < distances->get(neighborName))
            {
                distances->set(neighborName, newDist);
                neighbor->target->prev = graph->get(currName);
                queue.add(distances->get(neighborName), neighbor->target);
            }
            neighbor = neighbor->next;
        }
    }

    delete visited;
    delete distances;
}