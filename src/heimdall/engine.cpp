#include "engine.h"
#include "system/stacktrace.h"
#include "system/log.h"

namespace Heimdall {

const float MS_PER_UPDATE = 1000.0f / 30.0f;

Engine::~Engine()
{
    Shutdown();
}

void Engine::Start()
{
    if (m_isRunning) {
        log_warn("Engine already running.\n");
        return;
    }

    if (m_Initialized) {
        Run();
    } else {
        log_fail("World cannot start, world was not initialized.\n");
    }
}

void Engine::Stop()
{
    if (!m_isRunning) {
        log_warn("Engine already stopped.\n");
        return;
    }

    m_isRunning = false;
}

bool Engine::Initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        log_fail("Could not initialize SDL: %s\n", SDL_GetError());
        return false;
    }

    m_Window = SDL_CreateWindow(
        "Heimdall",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (m_Window == NULL) {
        log_fail("Could not create SDL window: %s\n", SDL_GetError());
        return false;
    }

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
    if (m_Renderer == NULL) {
        log_fail("Could not create SDL renderer: %s\n", SDL_GetError());
        return false;
    }

    if (SDL_SetRenderDrawBlendMode(m_Renderer, SDL_BLENDMODE_BLEND) < 0) {
        log_fail("Could not set up blending mode for the renderer: %s\n", SDL_GetError());
        return false;
    }

    if (SDL_NumJoysticks() > 0) {
        m_JoyStick = SDL_JoystickOpen(0);

        if (m_JoyStick == NULL)
        {
            log_fail("Could not open 0th joy stick: %s\n", SDL_GetError());
            return false;
        }

        log_info("Opened Joystick 0\n");
        log_info("Name: %s\n", SDL_JoystickNameForIndex(0));
        log_info("Number of Axes: %d\n", SDL_JoystickNumAxes(m_JoyStick));
        log_info("Number of Buttons: %d\n", SDL_JoystickNumButtons(m_JoyStick));
        log_info("Number of Balls: %d\n", SDL_JoystickNumBalls(m_JoyStick));

        SDL_JoystickEventState(SDL_ENABLE);
    }
    else {
        log_warn("Could not find any joy sticks\n");
    }

    entityManager = std::make_unique<EntityManager>();
    world = std::make_unique<World>(std::move(entityManager));

    m_Initialized = true;
    return true;
}

void Engine::Shutdown()
{
    if (m_JoyStick)
        SDL_JoystickClose(m_JoyStick);
    if (m_Renderer)
        SDL_DestroyRenderer(m_Renderer);
    if (m_Window)
        SDL_DestroyWindow(m_Window);
    SDL_Quit();
}

void Engine::Run()
{
    world->init();

    m_KeyboardState = SDL_GetKeyboardState(NULL);

    SDL_StopTextInput();
    SDL_Event e;

    m_isRunning = true;
    int64_t begin_frame_time = (int64_t)SDL_GetTicks();
    float frame_lag = 0.0f;
    while (m_isRunning) {
        int64_t current_frame_time = (int64_t)SDL_GetTicks();
        int64_t elapsed_frame_time = current_frame_time - begin_frame_time;
        begin_frame_time = current_frame_time;
        frame_lag += elapsed_frame_time;

        while (m_isRunning && SDL_PollEvent(&e))
        {
            HandleEvent(&e);
        }

        while (frame_lag >= MS_PER_UPDATE)
        {
            Update((float)elapsed_frame_time / 1000.0f);
            frame_lag -= MS_PER_UPDATE;
        }

        Render(frame_lag / MS_PER_UPDATE);
    }
}

void Engine::HandleEvent(SDL_Event *event)
{
    trace_assert(event);

    switch(event->type) {
    case SDL_QUIT:
        m_isRunning = false;
    }
}

void Engine::Update(float delta)
{
    world->update(delta);
}

void Engine::Render(float alpha)
{
    SDL_SetRenderDrawColor(m_Renderer, 85, 0, 85, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_Renderer);

    world->render(alpha);

    SDL_RenderPresent(m_Renderer);
}

World *Engine::getWorld()
{
    return world.get();
}

}