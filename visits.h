#pragma once

class Visits
{
public:
    bool visited;
    char *name;

    Visits(char *iname, bool ivisit)
    {
        this->name = iname;
        this->visited = ivisit;
    };
};



class VisitBucket
{
public:
    Visits **table;
    int capacity;
    int size;

    VisitBucket(int cap)
    {
        this->size = 0;
        this->capacity = cap;
        this->table = new Visits *[cap];

        for (int i = 0; i < this->capacity; i++)
        {
            this->table[i] = nullptr;
        }
    }

    ~VisitBucket()
    {
        for (int i = 0; i < this->size; i++)
        {
            delete this->table[i];
        }

        delete[] this->table;
    }

    void resize(int newCap)
    {
        Visits **newTable = new Visits *[newCap];
        for (int i = 0; i < this->size; i++)
        {
            newTable[i] = this->table[i];
        }
        delete[] this->table;
        this->table = newTable;
        this->capacity = newCap;
    }

    void add(char *name, bool ivisit)
    {
        Visits *target = new Visits(name, ivisit);

        if (this->size == this->capacity)
        {
            resize(this->capacity * 2);
        }

        this->table[this->size++] = target;
    }

    Visits *get(char *name)
    {
        Visits *target = nullptr;

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

    void set(char *name, bool ivisit)
    {
        for (int i = 0; i < this->size; i++)
        {
            if (strcmp(name, this->table[i]->name) == 0)
            {
                this->table[i]->visited = ivisit;
            }
        }
    }

    bool contains(char *name)
    {
        for (int i = 0; i < this->size; i++)
        {
            if (strcmp(name, this->table[i]->name) == 0)
            {
                return true;
            }
        }

        return false;
    }

    void update(char *name, bool ivisit)
    {
        if (this->contains(name))
        {
            this->set(name, ivisit);
        }
        else
        {
            this->add(name, ivisit);
        }
    }
};



class VisitHashTable
{
public:
    int capacity = 0;
    VisitBucket **table;
    VisitHashTable(int size)
    {
        this->capacity = size;
        this->table = new VisitBucket *[size];

        for (int i = 0; i < this->capacity; i++)
        {
            this->table[i] = nullptr;
        }
    }

    ~VisitHashTable()
    {
        for (int i = 0; i < this->capacity; i++)
        {
            if (this->table[i] != nullptr)
            {
                delete this->table[i];
            }
        }

        delete[] table;
    }

    int getIndex(char *name)
    {
        return (hashValue(name, this->capacity)) % this->capacity;
    }

    void addIfNotExists(char *name)
    {
        int index = getIndex(name);

        if (this->table[index] == nullptr)
        {
            this->table[index] = new VisitBucket(1);
        }
    }

    bool get(char *name)
    {
        int index = getIndex(name);

        VisitBucket *bucket = this->table[index];

        if (bucket != nullptr)
        {
            Visits *value = bucket->get(name);

            if (value != nullptr)
            {
                return value->visited;
            }
        }

        return false;
    }

    void set(char *name, bool ivisit)
    {
        int index = this->getIndex(name);

        this->addIfNotExists(name);

        this->table[index]->update(name, ivisit);
    }
};