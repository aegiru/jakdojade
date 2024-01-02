#pragma once


#define DISTANCE_MULTIPLIER 1
#define CITY_STRING_LENGTH 10
#define INPUT_BUFFER_SIZE 64

#define HASH_FIRST_LETTERS 2
#define HASH_LAST_LETTERS 2
#define HASH_SINGLE_RANGE 37
#define HASH_TABLE_SIZE 25000
#define HASH_MULTIPLIER 541

#define INF_DIST 9999999
#define DIJKSTRA_QUEUE_SIZE 25000


int iPow(int base, int exponent) {
    int a = 1;

    for (int i = 0; i < exponent; i++) {
        a *= base;
    }

    return a;
}



int hashCharValue(char c) {
    if (c >= '0' && c <= '9') {
        return (int)(c - '0');
    }
    else if (c >= 'A' && c <= 'Z') {
        return (int)(10 + c - 'A');
    }

    return 0;
}


int hashValue(char* string, int size) {
    int len = strlen(string);
    int sum = 0;
    int counter = 0;

    for (int i = 0; i < HASH_FIRST_LETTERS && counter <= len; i++) {
        sum += (hashCharValue(string[i]) * iPow(HASH_SINGLE_RANGE, counter));
        sum = (sum * HASH_MULTIPLIER) % size;
        counter++;
    }

    for (int j = 0; j < HASH_LAST_LETTERS && counter <= len; j++) {
        sum += (hashCharValue(string[len - (j + 1)]) * iPow(HASH_SINGLE_RANGE, counter));
        sum = (sum * HASH_MULTIPLIER) % size;
        counter++;
    }

    return sum;
}



bool isNameElement(char c) {
    if (c >= '0' && c <= '9') {
        return true;
    }
    else if (c >= 'A' && c <= 'Z') {
        return true;
    }
    else if (c >= 'a' && c <= 'z') {
        return true;
    }

    return false;
}


bool wasVisited(int i) {
    return (i != INF_DIST);
}



bool isNonCity(char a) {
    return (a == '#' || a == '@');
}



bool isCity(char a) {
    return (a == '*');
}




bool isRoadElement(char a) {
    return (a == '#' || a == '*');
}



bool isCrossRoads(char** map, int i, int j, int x, int y) {
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;

    if (i > 0) {
        if (isRoadElement(map[i - 1][j])) {
            left = true;
        }
    }

    if (i < y - 2) {
        if (isRoadElement(map[i + 1][j])) {
            right = true;
        }
    }

    if (j > 0) {
        if (isRoadElement(map[i][j - 1])) {
            up = true;
        }
    }

    if (j < x - 2) {
        if (isRoadElement(map[i][j + 1])) {
            down = true;
        }
    }

    if ((up && right) || (up && left) || (down && right) || (down && left)) {
        return true;
    }

    return false;
}



void overlayCrossroads(char** map, int x, int y, int& nodeCount, int& cityCount) {

    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            if (isNonCity(map[i][j])) {
                if (isCrossRoads(map, i, j, x, y)) {
                    map[i][j] = '@';
                    nodeCount++;
                }
            }

            if (isCity(map[i][j])) {
                cityCount++;
            }
        }
    }
}



void getAndParseInput(char* miasto1, char* miasto2, int* i) {
    char help[CITY_STRING_LENGTH];
    int c;
    int index = 0;
    int field = 0;
    getchar();

    while ((c = getchar()) != EOF && field < 3) {
        if (c == ' ' || c == '\n') {
            if (field == 0) {
                miasto1[index] = '\0';
            }
            else if (field == 1) {
                miasto2[index] = '\0';
            }
            else if (field == 2) {
                help[index] = '\0';
                *i = atoi(help);
            }
            index = 0;
            field++;
            continue;
        }

        if (field == 0) {
            miasto1[index++] = c;
        }
        else if (field == 1) {
            miasto2[index++] = c;
        }
        else if (field == 2) {
            help[index++] = c;
        }
    }
}


char read_char() {
    char c = getchar();
    if (c == '\r') {
        c = getchar();
    }
    return c;
}



int readInt() {
    int x = 0;
    char c = read_char();

    while (c < '0' || c > '9') {
        c = read_char();
    }

    while (c >= '0' && c <= '9') {
        x = x * 10 + (c - '0');
        c = read_char();
    }

    return x;
}


void readString(char* str, int maxLen) {
    int i = 0;
    char c = read_char();

    while ((c != ' ' && c != '\n') && i < maxLen - 1) {
        str[i++] = c;
        c = read_char();
    }

    str[i] = '\0';
}


void readMap(char** map, int width, int height) {
    for (int y = 0; y < height; y++) {
        map[y] = new char[width];
        char c = read_char();

        while (c == ' ' || c == '\n' || c == '\r') {
            c = read_char();
        }

        for (int x = 0; x < width; x++) {
            map[y][x] = c;
            c = read_char();
        }
    }
}