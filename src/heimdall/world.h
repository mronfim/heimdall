#pragma once
#include <memory>
#include "component.h"
#include "componentHandle.h"
#include "componentManager.h"
#include "componentMask.h"
#include "entity.h"
#include "entityManager.h"
#include "system.h"

namespace Heimdall {

struct EntityHandle;

class World {
public:
    explicit World(std::unique_ptr<EntityManager> entityManager);

    /*
     * Should be called before the first update, but after instantiation
     */
    void init();

    void update(float delta);

    void render(float alpha);

    EntityHandle createEntity();
    
    void addSystem(std::unique_ptr<System> system);

    void destroyEntity(Entity entity);

    template <typename ComponentType>
    void addCustomComponentManager(std::unique_ptr<ComponentManager<ComponentType>> manager)
    {
        int family = getComponentFamily<ComponentType>();
        if (family >= componentManagers.size()) {
            componentManagers.resize(family + 1);
        }
        componentManagers[family] = manager;
    }

    template <typename ComponentType>
    void addComponent(Entity const &entity, ComponentType &&component)
    {
        ComponentManager<ComponentType> *manager = getComponentManager<ComponentType>();
        manager->addComponent(entity, component);

        ComponentMask oldMask = entityMasks[entity];
        entityMasks[entity].addComponent<ComponentType>();

        updateEntityMask(entity, oldMask);
    }

    template <typename ComponentType>
    void removeComponent(Entity const &entity)
    {
        ComponentManager<ComponentType> *manager = getComponentManager<ComponentType>();
        ComponentHandle<ComponentType> component = manager->lookup(entity);
        component.destroy();

        ComponentMask oldMask = entityMasks[entity];
        entityMasks[entity].removeComponent<ComponentType>();

        updateEntityMask(entity, oldMask);
    }

    template <typename ComponentType, typename... Args>
    void unpack(Entity e, ComponentHandle<ComponentType> &handle, ComponentHandle<Args>&... args)
    {
        typedef ComponentManager<ComponentType> ComponentManagerType;
        auto mgr = getComponentManager<ComponentType>();
        handle = ComponentHandle<ComponentType>(e, mgr->lookup(e), mgr);

        unpack<Args...>(e, args...);
    }

    // Base case
    template <typename ComponentType>
    void unpack(Entity e, ComponentHandle<ComponentType> &handle)
    {
        typedef ComponentManager<ComponentType> ComponentManagerType;
        auto mgr = getComponentManager<ComponentType>();
        handle = ComponentHandle<ComponentType>(e, mgr->lookup(e), mgr);
    }

private:
    std::unique_ptr<EntityManager> entityManager;
    std::vector<std::unique_ptr<System>> systems;
    std::vector<std::unique_ptr<BaseComponentManager>> componentManagers;
    std::map<Entity, ComponentMask> entityMasks;

    void updateEntityMask(Entity const &entity, ComponentMask oldMask);

    template <typename ComponentType>
    ComponentManager<ComponentType> *getComponentManager() {
        int family = getComponentFamily<ComponentType>();

        if (family >= componentManagers.size()) {
            componentManagers.resize(family + 1);
        }

        if (!componentManagers[family]) {
            componentManagers[family] = std::make_unique<ComponentManager<ComponentType>>();
        }

        return static_cast<ComponentManager<ComponentType> *>(componentManagers[family].get());
    }
};

}