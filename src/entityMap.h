#ifndef ENTITY_MAP_H_
#define ENTITY_MAP_H_

typedef struct node {
    uint32_t key;
    uint32_t val;
    struct node *next;
} EntityNode;

typedef struct table {
    int size;
    struct node **list;
} EntityMap;

EntityMap *em_create_table(int size);
uint32_t em_hash_code(EntityMap *t, uint32_t key);
void em_insert(EntityMap *t, uint32_t key, uint32_t val);
uint32_t lookup(EntityMap *t, uint32_t key);

#endif // ENTITY_MAP_H_


