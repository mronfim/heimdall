#include <SDL2/SDL.h>

#include <stdio.h>

#include "system/log.h"
#include "system/lt.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int argc, char* argv[])
{
    Lt *lt = create_lt();

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        log_fail("Could not initialize SDL: %s\n", SDL_GetError());
        RETURN_LT(lt, -1);
    }
    PUSH_LT(lt, 42, SDL_Quit);

    SDL_Window *const window = PUSH_LT(
        lt,
        SDL_CreateWindow(
            "Heimdall",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN),
        SDL_DestroyWindow);
    if (window == NULL) {
        log_fail("Could not create SDL window: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Renderer *const renderer = PUSH_LT(
        lt,
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED),
        SDL_DestroyRenderer);
    if (renderer == NULL) {
        log_fail("Could not create SDL renderer: %s\n", SDL_GetError());
        RETURN_LT(lt, -1);
    }
    if (SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) < 0) {
        log_fail("Could not set up blending mode for the renderer: %s\n", SDL_GetError());
        RETURN_LT(lt, -1);
    }

    SDL_Joystick *joy_stick = NULL;
    if (SDL_NumJoysticks() > 0) {
        joy_stick = PUSH_LT(lt, SDL_JoystickOpen(0), SDL_JoystickClose);

        if (joy_stick == NULL) {
            log_fail("Could not open 0th joy stick: %s\n", SDL_GetError());
            RETURN_LT(lt, -1);
        }

        log_info("Opened Joystick 0\n");
        log_info("Name: %s\n", SDL_JoystickNameForIndex(0));
        log_info("Number of Axes: %d\n", SDL_JoystickNumAxes(joy_stick));
        log_info("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy_stick));
        log_info("Number of Balls: %d\n", SDL_JoystickNumBalls(joy_stick));

        SDL_JoystickEventState(SDL_ENABLE);
    } else {
        log_warn("Could not find any joy sticks\n");
    }

    // ---------------------------------------------------------------------------------------

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Delay(3000);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}