#include <SDL2/SDL.h>
#include "system/stacktrace.h"
#include <stdio.h>

#include "game.h"
#include "system/log.h"
#include "system/lt.h"
#include "system/h_alloc.h"

typedef enum Game_state {
    GAME_STATE_RUNNING = 0,
    GAME_STATE_PAUSE,
    GAME_STATE_QUIT
} Game_state;

typedef struct Game {
    Lt *lt;

    Game_state state;
    SDL_Renderer *renderer;
} Game;

Game *create_game(SDL_Renderer *renderer)
{
    Lt *lt = create_lt();

    Game *game = PUSH_LT(lt, h_alloc(1, sizeof(Game)), free);
    if (game == NULL) {
        RETURN_LT(lt, NULL);
    }
    game->lt = lt;

    game->state = GAME_STATE_RUNNING;
    game->renderer = renderer;

    return game;
}

void destroy_game(Game *game)
{
    trace_assert(game);
    RETURN_LT0(game->lt);
}

int game_render(const Game *game)
{
    trace_assert(game);

    SDL_SetRenderDrawColor(game->renderer, 85, 0, 85, SDL_ALPHA_OPAQUE);

    switch (game->state) {
        case GAME_STATE_RUNNING:
        case GAME_STATE_PAUSE: {
            SDL_RenderClear(game->renderer);
            SDL_RenderPresent(game->renderer);
            break;
        }
        case GAME_STATE_QUIT:
            return 0;
    }

    return 0;
}

int game_update(Game *game, float delta_time)
{
    trace_assert(game);
    trace_assert(delta_time > 0.0f);

    switch (game->state) {
        case GAME_STATE_RUNNING: {
        } break;
        case GAME_STATE_PAUSE: {
        } break;
        case GAME_STATE_QUIT: {
            return 0;
        }
    }

    return 0;
}

int game_event(Game *game, const SDL_Event *event)
{
    trace_assert(game);
    trace_assert(event);

    switch(event->type) {
        case SDL_QUIT: {
            game->state = GAME_STATE_QUIT;
            return 0;
        } break;
        
        case SDL_MOUSEMOTION: {
        } break;
    }

    // Run event based on game state

    return 0;
}

int game_input(Game *game,
                const Uint8 *const keyboard_state,
                SDL_Joystick *joy_stick)
{
    trace_assert(game->state);
    trace_assert(keyboard_state);

    switch (game->state) {
        case GAME_STATE_QUIT:
        case GAME_STATE_PAUSE:
            return 0;
        case GAME_STATE_RUNNING:
            return 0;
    }

    return 0;
}

int is_game_over(const Game *game)
{
    return game->state == GAME_STATE_QUIT;
}
