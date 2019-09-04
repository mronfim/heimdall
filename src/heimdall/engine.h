#pragma once
#include <SDL.h>
#include <memory>

#include "entityManager.h"
#include "world.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

namespace Heimdall {

class Engine {
public:
    static Engine& GetInstance()
    {
        static Engine instance;
        return instance;
    }

    Engine(Engine const &) = delete;
    void operator=(Engine const &) = delete;

    bool Initialize();
    void Start();
    void Stop();
    virtual ~Engine();

    World *getWorld();
    SDL_Renderer *getRenderer() { return m_Renderer; }

private:
    bool m_Initialized{ false };
    bool m_isRunning{ false };

    SDL_Window *m_Window{ NULL };
    SDL_Renderer *m_Renderer{ NULL };
    SDL_Joystick *m_JoyStick{ NULL };

    const Uint8 *m_KeyboardState{ NULL };

    std::unique_ptr<EntityManager> entityManager;
    std::unique_ptr<World> world;

    Engine() {}

    void Run();
    void HandleEvent(SDL_Event *e);
    void Update(float delta);
    void Render(float alpha);
    void Shutdown();
};

}