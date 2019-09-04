#include "system.h"

namespace Heimdall {

void System::registerWorld(World *world)
{
    parentWorld = world;
}

void System::registerEntity(Entity const &entity)
{
    registeredEntities.push_back(entity);
}

void System::unRegisterEntity(Entity const &entity)
{
    for (auto it = registeredEntities.begin(); it != registeredEntities.end(); ++it) {
        Entity e = *it;
        if (e.id == entity.id) {
            registeredEntities.erase(it);
            return;
        }
    }
}

ComponentMask System::getSignature()
{
    return signature;
}

}