#pragma once

class City;



class CityAdjacencyList
{
public:
    int distance = 0;
    City *target = nullptr;
    CityAdjacencyList *next = nullptr;
};



class City
{
public:
    CityAdjacencyList *neighbors = nullptr;
    char *name;
    City *prev;

    City(char *iname)
    {
        this->neighbors = nullptr;
        this->name = iname;
        this->prev = nullptr;
    };

    ~City()
    {
        CityAdjacencyList *curr = this->neighbors;
        CityAdjacencyList *next = nullptr;

        while (curr != nullptr)
        {
            next = curr->next;
            delete curr;
            curr = next;
        }

        delete name;
    }

    void addConnection(City *target, int distance)
    {
        CityAdjacencyList *new_node = new CityAdjacencyList();
        new_node->target = target;
        new_node->distance = distance;

        if (this->neighbors == nullptr)
        {
            this->neighbors = new_node;
        }
        else
        {
            new_node->next = this->neighbors;
            this->neighbors = new_node;
        }
    }
};



class CityBucket
{
public:
    City **table;
    int capacity;
    int size;

    CityBucket(int cap)
    {
        this->size = 0;
        this->capacity = cap;
        this->table = new City *[cap];

        for (int i = 0; i < this->capacity; i++)
        {
            this->table[i] = nullptr;
        }
    }

    ~CityBucket()
    {
        for (int i = 0; i < this->size; i++)
        {
            delete this->table[i];
        }

        delete[] this->table;
    }

    void resize(int newCap)
    {
        City **newTable = new City *[newCap];
        for (int i = 0; i < this->size; i++)
        {
            newTable[i] = this->table[i];
        }
        delete[] table;
        this->table = newTable;
        this->capacity = newCap;
    }

    void add(char *name)
    {
        City *target = new City(name);

        if (this->size == this->capacity)
        {
            resize(this->capacity * 2);
        }
        this->table[this->size++] = target;
    }

    City *get(char *name)
    {
        City *target = nullptr;

        for (int i = 0; i < this->size; i++)
        {
            if (strcmp(name, this->table[i]->name) == 0)
            {
                target = this->table[i];
                break;
            }
        }

        return target;
    }

    bool contains(char *name)
    {
        for (int i = 0; i < size; i++)
        {
            if (this->table[i] != nullptr)
            {
                if (strcmp(name, this->table[i]->name) == 0)
                {
                    return true;
                }
            }
        }

        return false;
    }
};



class CityHashTable
{
public:
    int capacity = 0;
    CityBucket **table;

    CityHashTable(int size)
    {
        this->capacity = size;
        this->table = new CityBucket *[size];

        for (int i = 0; i < this->capacity; i++)
        {
            this->table[i] = nullptr;
        }
    }

    ~CityHashTable()
    {
        for (int i = 0; i < this->capacity; i++)
        {
            if (this->table[i] != nullptr)
            {
                delete this->table[i];
            }
        }

        delete[] this->table;
    }

    int getIndex(char *name)
    {
        return (hashValue(name, this->capacity)) % this->capacity;
    }

    City *get(char *name)
    {
        int index = getIndex(name);

        return this->table[index]->get(name);
    }

    void addIfNotExists(char *name)
    {
        int index = getIndex(name);

        if (this->table[index] == nullptr)
        {
            this->table[index] = new CityBucket(1);
        }

        if (!this->table[index]->contains(name))
        {
            this->table[index]->add(name);
        }
    }

    void addConnectionToCity(char *cityOne, char *cityTwo, int dist)
    {
        addIfNotExists(cityOne);
        addIfNotExists(cityTwo);

        this->get(cityOne)->addConnection(this->get(cityTwo), dist);
    }
};



class CityNameList
{
public:
    char *name;
    CityNameList *next;
};



void addName(CityNameList **head, char *name)
{
    CityNameList *new_node = new CityNameList();
    new_node->next = nullptr;
    new_node->name = name;

    if ((*head) == nullptr)
    {
        (*head) = new_node;
        return;
    }
    else
    {
        new_node->next = (*head);
        (*head) = new_node;
    }
}



void deleteNames(CityNameList **head)
{
    CityNameList *curr = (*head);
    CityNameList *next = nullptr;

    while (curr != nullptr)
    {
        next = curr->next;
        delete curr;
        curr = next;
    }

    (*head) = nullptr;
}