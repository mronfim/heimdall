#pragma once
#include "componentManager.h"
#include "entity.h"

namespace Heimdall {

template <typename ComponentType>
struct ComponentHandle {
    using ExposedComponentType = typename ComponentManager<ComponentType>::LookupType;

    Entity owner;
    ExposedComponentType *component;
    ComponentManager<ComponentType> *manager;

    // Empty constructor used for World::unpack()
    ComponentHandle() {};
    ComponentHandle(Entity owner, ExposedComponentType *component, ComponentManager<ComponentType> *manager)
    {
        this->owner = owner;
        this->component = component;
        this->manager = manager;
    }

    // handle->member is the same as handle.component->member
    ExposedComponentType *operator->() const { return component; }

    void destroy() { manager->destroyComponent(owner); }
};

}