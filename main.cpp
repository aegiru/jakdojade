#include <string.h>
#include <iostream>



#include "main.h"
#include "cities.h"
#include "visits.h"
#include "distances.h"
#include "bfs.h"
#include "dijkstra.h"



using namespace std;



int main(int, char**) {
    int x = readInt();
    int y = readInt();

    char** map = new char* [y];

    readMap(map, x, y);
    

    CityHashTable* graph = new CityHashTable(HASH_TABLE_SIZE);

    for (int i = 0; i < y; i++) {
        

        for (int j = 0; j < x; j++) {
            if (isCity(map[i][j])) {
                char* name = findCityName(map, j, i, x, y);

                BFSFloodFill(map, graph, name, j, i, x, y);
            }
        }
    }

    delete[] map;

    int liczbaLotow = readInt();

    for (int k = 0; k < liczbaLotow; k++) {
        char miasto1[CITY_STRING_LENGTH], miasto2[CITY_STRING_LENGTH];
        int dystans;

        readString(miasto1, CITY_STRING_LENGTH);
        readString(miasto2, CITY_STRING_LENGTH);
        dystans = readInt();

        graph->addConnectionToCity(miasto1, miasto2, dystans);
    }

    int liczbaPytan = readInt();

    for (int l = 0; l < liczbaPytan; l++) {
        char miasto1[CITY_STRING_LENGTH], miasto2[CITY_STRING_LENGTH];
        int verbose;

        readString(miasto1, CITY_STRING_LENGTH);
        readString(miasto2, CITY_STRING_LENGTH);
        verbose = readInt();

        DijkstraSearch(graph, miasto1, miasto2, verbose);
    }

    delete graph;
}
