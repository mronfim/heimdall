#ifndef ENTITY_H_
#define ENTITY_H_

#include <stdint.h>

typedef Entity uint32_t;

typedef struct EntityManager {
    uint32_t entity_count;
}

inline void create_entity(EntityManager* em, Entity* e) {
    *e = em->entity_count++;
}

#endif
