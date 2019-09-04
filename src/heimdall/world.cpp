#include "world.h"
#include "entityHandle.h"

namespace Heimdall {

World::World(std::unique_ptr<EntityManager> entityManager) : entityManager(std::move(entityManager)) {}

void World::init()
{
    for (auto &system : systems) {
        system->init();
    }
}

void World::update(float delta)
{
    // TODO (mronfim) check that the world was initialized.
    for (auto &system : systems) {
        system->update(delta);
    }
}

void World::render(float alpha)
{
    for (auto &system : systems) {
        system->render(alpha);
    }
}

EntityHandle World::createEntity()
{
    return { entityManager->createEntity(), this };
}

void World::destroyEntity(Entity entity)
{
    for (auto &system : systems) {
        system->unRegisterEntity(entity);
    }
    entityManager->destroy(entity);
}

void World::addSystem(std::unique_ptr<System> system)
{
    system->registerWorld(this);
    systems.push_back(std::move(system));
}

void World::updateEntityMask(Entity const &entity, ComponentMask oldMask)
{
    ComponentMask newMask = entityMasks[entity];
    for (auto &system : systems) {
        ComponentMask systemSignature = system->getSignature();
        if (newMask.isNewMatch(oldMask, systemSignature)) {
            // We match but didn't match before
            system->registerEntity(entity);
        } else if (newMask.isNoLongerMatched(oldMask, systemSignature)) {
            system->unRegisterEntity(entity);
        }
    }
}

}