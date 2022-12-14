#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "hash.h"
#include "mem_mac.h"

struct Map* newMap() {
    struct Map* m = new(struct Map);
    m->pairs = new_array(struct Pair*, INITIAL_CAPACITY);
    m->capacity = INITIAL_CAPACITY;
    m->pairsUsed = 0;

    for (int i = 0; i < INITIAL_CAPACITY; i ++) {
        m->pairs[i] = NULL;
    }

    return m;
}

void deleteMap(struct Map* m) {
    for (int i = 0; i < m->capacity; i ++) {
        if (m->pairs[i] != NULL) {
            if (m->pairs[i]->Key != NULL) 
                free(m->pairs[i]->Key);
            free(m->pairs[i]);
        }
    }

    free(m->pairs);
    free(m);
}

uint32_t hashKey(char* key) {
    uint32_t hash = 2166136261u;

    int i = 0;
    for (char curChar = key[i]; curChar != '\0'; curChar = key[++i]) {
        hash ^= curChar;
        hash *= 16777619;
    }

    return hash;
}

void setPair(struct Map* m, char* key, long long value) {
    struct Pair* pairToAdd = new(struct Pair);
    pairToAdd->Value = value;
    pairToAdd->keyHash = hashKey(key);
    pairToAdd->Key = new_array(char, strlen(key) + 1);
    strcpy(pairToAdd->Key, key);

    if (m->capacity * ALLOC_THRESHOLD < m->pairsUsed + 1) {
        expandMap(m);
    }

    uint32_t location = pairToAdd->keyHash % m->capacity;
    while (m->pairs[location] != NULL) { 
        location = (location + 1) % (m->capacity);
    }

    m->pairs[location] = pairToAdd;
    m->pairsUsed ++;

}

void setPairInt(struct Map* m, int key, int value) {
    struct Pair* pairToAdd = new(struct Pair);
    pairToAdd->Value = value;
    pairToAdd->keyHash = key;
    pairToAdd->Key = NULL;

    if (m->capacity * ALLOC_THRESHOLD < m->pairsUsed + 1) {
        expandMap(m);
    }

    uint32_t location = key % m->capacity;
    while (m->pairs[location] != NULL) {
        location = (location + 1) % m->capacity;
    }

    m->pairs[location] = pairToAdd;
    m->pairsUsed ++;
}

void expandMap(struct Map* m) {
    uint32_t new_capacity = (m->capacity + 1) * 2;
    struct Pair** newSet = new_array(struct Pair*, new_capacity);

    for (int i = 0; i < new_capacity; i++) {
        newSet[i] = NULL;
    }

    for (int i = 0; i < m->capacity; i++) {
        if (m->pairs[i] != NULL) {
            uint32_t location = m->pairs[i]->keyHash % new_capacity;
            while (newSet[location] != NULL) {
                location = (location + 1) % new_capacity;
            }

            newSet[location] = m->pairs[i];
        }
    }

    free(m->pairs);
    m->pairs = newSet;
    m->capacity = new_capacity;
}

long long lookup(struct Map* m, char* key) {
    uint32_t location = hashKey(key) % m->capacity;
    uint32_t starting_location = location;
    do {
        if (m->pairs[location] != NULL && !strcmp(m->pairs[location]->Key, key)) {
            return m->pairs[location]->Value;
        }
        location = (location + 1) % m->capacity;

    } while (location != starting_location);
    
    return -1;
}

int lookupInt(struct Map* m, int key) {
    uint32_t location = key % m->capacity;
    uint32_t starting_location = location;
    do {
        if (m->pairs[location] != NULL && m->pairs[location]->keyHash == key) {
            return m->pairs[location]->Value;
        }
        location = (location + 1) % m->capacity;
    } while (location != starting_location);
    return -1;
}

void printMap(struct Map *m) {
    for (int i = 0; i < m->capacity; i++) {
        if (m->pairs[i] != NULL) {
            printf("%s: %lld\n", m->pairs[i]->Key, m->pairs[i]->Value);
        }
    }
}
