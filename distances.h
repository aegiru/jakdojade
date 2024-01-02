class Distance
{
public:
    int dist;
    char *name;

    Distance(char *iname, int idist)
    {
        this->name = iname;
        this->dist = idist;
    };
};



class DistanceBucket
{
public:
    Distance **table;
    int capacity;
    int size;

    DistanceBucket(int cap)
    {
        this->size = 0;
        this->capacity = cap;
        this->table = new Distance *[cap];

        for (int i = 0; i < this->capacity; i++)
        {
            this->table[i] = nullptr;
        }
    }

    ~DistanceBucket()
    {
        for (int i = 0; i < this->size; i++)
        {
            delete this->table[i];
        }

        delete[] this->table;
    }

    void resize(int newCap)
    {
        Distance **newTable = new Distance *[newCap];
        for (int i = 0; i < this->size; i++)
        {
            newTable[i] = this->table[i];
        }
        delete[] this->table;
        this->table = newTable;
        this->capacity = newCap;
    }

    void add(char *name, int dist)
    {
        Distance *target = new Distance(name, dist);

        if (this->size == this->capacity)
        {
            resize(this->capacity * 2);
        }

        this->table[this->size++] = target;
    }

    Distance *get(char *name)
    {
        Distance *target = nullptr;

        for (int i = 0; i < this->size; i++)
        {
            if (this->table[i] != nullptr)
            {
                if (strcmp(name, this->table[i]->name) == 0)
                {
                    target = this->table[i];
                    break;
                }
            }
        }

        return target;
    }

    void set(char *name, int dist)
    {
        for (int i = 0; i < this->size; i++)
        {
            if (strcmp(name, this->table[i]->name) == 0)
            {
                this->table[i]->dist = dist;
            }
        }
    }

    bool contains(char *name)
    {
        for (int i = 0; i < this->size; i++)
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

    void update(char *name, int dist)
    {
        if (this->contains(name))
        {
            this->set(name, dist);
        }
        else
        {
            this->add(name, dist);
        }
    }
};



class DistHashTable
{
public:
    int capacity = 0;
    DistanceBucket **table;
    DistHashTable(int size)
    {
        this->capacity = size;
        this->table = new DistanceBucket *[size];

        for (int i = 0; i < this->capacity; i++)
        {
            this->table[i] = nullptr;
        }
    }

    ~DistHashTable()
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

    void addIfNotExists(char *name)
    {
        int index = getIndex(name);

        if (this->table[index] == nullptr)
        {
            this->table[index] = new DistanceBucket(1);
        }
    }

    int getIndex(char *name)
    {
        return (hashValue(name, this->capacity)) % this->capacity;
    }

    int get(char *name)
    {
        int index = getIndex(name);

        DistanceBucket *bucket = this->table[index];

        if (bucket != nullptr)
        {
            Distance *value = bucket->get(name);

            if (value != nullptr)
            {
                return value->dist;
            }
        }

        return INF_DIST;
    }

    void set(char *name, int dist)
    {
        int index = this->getIndex(name);

        this->addIfNotExists(name);
        
        this->table[index]->update(name, dist);
    }
};

