#ifndef HASH_H
#define HASH_H

#include <inttypes.h>
#define ALLOC_THRESHOLD 0.66
#define INITIAL_CAPACITY  16

struct Map {
    struct Pair** pairs;
    int pairsUsed;
    uint32_t capacity;
};

struct Pair {
    char* Key;
    int keyHash;
    long long Value;
};

/* Construct & Destruct */
struct Map* newMap();
void deleteMap(struct Map* m);

/* Access */
void setPair(struct Map* m, char* key, long long value);
void setPairInt(struct Map* m, int key, int value);
long long lookup(struct Map* m, char* key);
int lookupInt(struct Map* m, int key);

/* Utility */
uint32_t hashKey(char* key);
void expandMap(struct Map* m);
void printMap(struct Map* m);
#endif
