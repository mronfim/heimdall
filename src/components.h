#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include "entitiy.h"

#define MAX_NUM_COMPONENTS 1024

typedef enum  {
    COMPONENT_NONE = 0,
    COMPONENT_TRANSFORM,
    COMPONENT_SPRITE
} ComponentType;

typedef ComponentInstance uint32_t;

typedef struct ComponentManager {
    ComponentType type;
    ComponentData componentData;

} ComponentManager;

typedef struct ComponentData {
    uint32_t size = 1;
    void *data[MAX_NUM_COMPONENTS];
} ComponentData;

ComponentInstance component_add(Entity e, void* c, ComponentType t);
void *lookup(




#endif // COMPONENTS_H_
