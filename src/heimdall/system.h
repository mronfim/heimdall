#pragma once
#include <bitset>
#include <vector>
#include "componentMask.h"
#include "entity.h"

namespace Heimdall {

class World;

class System {
public:
    System() = default;
    virtual ~System() = default;
    System(const System &) = default;
    System &operator=(const System &) = default;
    System(System &&) = default;
    System &operator=(System &&) = default;

    /*
     * Called before the engine starts but after the world initializes
     */
    virtual void init() {};

    /*
     * Called every game update
     */
    virtual void update(float delta) {};

    /*
     * Called every frame
     */
    virtual void render(float alpha) {};

    /*
     * When a system is added to the world, the world will register itself
     */
    void registerWorld(World *world);

    /*
     * When a component is added such that this system should begin acting on it,
     * register will be called
     */
    void registerEntity(Entity const &entity);

    /*
     * When a component is removed from an entity such that the system should stop
     * acting on it, unRegister will be called
     */
    void unRegisterEntity(Entity const &entity);

    ComponentMask getSignature();

protected:
    std::vector<Entity> registeredEntities;
    World *parentWorld;
    ComponentMask signature;
};

}