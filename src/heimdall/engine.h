#pragma once
#include <SDL.h>

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

    void Start();
    void Stop();
    virtual ~Engine();

private:
    bool m_isRunning{ false };

    SDL_Window *m_Window{ NULL };
    SDL_Renderer *m_Renderer{ NULL };
    SDL_Joystick *m_JoyStick{ NULL };

    const Uint8 *m_KeyboardState{ NULL };

    Engine() {}

    bool Initialize();
    void Run();
    void HandleEvent(SDL_Event *e);
    void Update(float delta);
    void Render(float alpha);
    void Shutdown();
};

}