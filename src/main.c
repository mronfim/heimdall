#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "system/log.h"
#include "system/lt.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

const float MS_PER_UPDATE = 1000.0f / 30.0f;

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

    Game *const game = PUSH_LT(
        lt,
        create_game(renderer),
        destroy_game);
    if (game == NULL) {
        RETURN_LT(lt, -1);
    }

    const Uint8 *const keyboard_state = SDL_GetKeyboardState(NULL);

    SDL_StopTextInput();
    SDL_Event e;

    int64_t begin_frame_time = (int64_t) SDL_GetTicks();
    float frame_lag = 0.0f;
    while (!is_game_over(game)) {
        int64_t current_frame_time = (int64_t) SDL_GetTicks();
        int64_t elapsed_frame_time = current_frame_time - begin_frame_time;
        begin_frame_time = current_frame_time;
        frame_lag += elapsed_frame_time;

        while (!is_game_over(game) && SDL_PollEvent(&e)) {
            if (game_event(game, &e) < 0) {
                RETURN_LT(lt, -1);
            }
        }


        while (frame_lag >= MS_PER_UPDATE) {
            if (game_update(game, (float) elapsed_frame_time / 1000.0f) < 0) {
                RETURN_LT(lt, -1);
            }
            frame_lag -= MS_PER_UPDATE;
        }

        // TODO: add frame lag to render function for render interpolation
        if (game_render(game) < 0) {
            RETURN_LT(lt, -1);
        }
    }

    return 0;
}
