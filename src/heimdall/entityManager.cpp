#include "entityManager.h"

namespace Heimdall {

Entity EntityManager::createEntity()
{
    return { lastEntity++ };
}

void EntityManager::destroy(Entity entity)
{
    // TODO
}

}