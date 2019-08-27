#ifndef GAME_H_
#define GAME_H_

#include <SDL2/SDL.h>

typedef struct Game Game;

Game *create_game(SDL_Renderer *renderer);

void destroy_game(Game *game);

int game_render(const Game *game);
int game_update(Game *game, float delta_time);

int game_event(Game *game, const SDL_Event *event);
int game_input(Game *game,
                const Uint8 *const keyboard_state,
                SDL_Joystick *joy_stick);

int is_game_over(const Game *game);

#endif // GAME_H_
