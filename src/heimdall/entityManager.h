#pragma once
#include "entity.h"

namespace Heimdall {

class EntityManager {
public:
    Entity createEntity();
    void destroy(Entity entity);

private:
    int lastEntity = 0;
};

}