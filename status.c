//
// status.c
// sdl2_testing
//
#include <SDL2/SDL.h>
#include <stdio.h>
#include "main.h"


void init_status_lives(GameState *game)
{
  SDL_Color white = {255, 255, 255, 255 };

  SDL_Surface *tmp = TTF_RenderText_Blended(game->font, "DooM", white);
  game->labelW = tmp->w;
  game->labelH = tmp->h;
  game->label = SDL_CreateTextureFromSurface(game->renderer, tmp);
  SDL_FreeSurface(tmp);
  tmp = TTF_RenderText_Blended(game->font, "Press any key to start", white);
  game->contW = tmp->w;
  game->contH = tmp->h;
  game->cont = SDL_CreateTextureFromSurface(game->renderer, tmp);
  SDL_FreeSurface(tmp);
}

void draw_status_lives(GameState *game)
{
  SDL_SetRenderDrawColor(game->renderer, 230, 46, 0, 255);

  //Clear the screen
  SDL_RenderClear(game->renderer);

  //set the drawing color to white
  SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);


  //SDL_Rect textRect = {330-game->labelW, 250-game->labelH, 2*game->labelW, 2*game->labelH};
  SDL_Rect textRect = {960-(3*game->labelW), 540-(3*game->labelH), 6*game->labelW, 6*game->labelH};
  SDL_RenderCopy(game->renderer, game->label, NULL, &textRect);
  //SDL_Rect textRect2 = {505-game->contW, 350-game->contH, game->contW, game->contH};
  SDL_Rect textRect2 = {960-(game->contW), 540+(3*game->labelH), 2*game->contW, 2*game->contH};
  SDL_RenderCopy(game->renderer, game->cont, NULL, &textRect2);
}
void shutdown_status_lives(GameState *game)
{
  SDL_DestroyTexture(game->label);
  game->label = NULL;
}

void init_game_over(GameState *game)
{
    //Create label textures for status screen
    SDL_Color white = { 255, 255, 255, 255};

    SDL_Surface *tmp = TTF_RenderText_Blended(game->font, "GAME OVER!", white);
    game->labelW = tmp->w;
    game->labelH = tmp->h;
    game->label = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
}

void draw_game_over(GameState *game)
{
    SDL_Renderer *renderer = game->renderer;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 225);

    //Clear the screen
    SDL_RenderClear(renderer);

    //Set the drawing color to white
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    SDL_Rect textRect = {960-game->labelW, 540-game->labelH, 2*game->labelW, 2*game->labelH};
    SDL_RenderCopy(renderer, game->label, NULL, &textRect);
}


void shutdown_game_over(GameState *game)
{
    SDL_DestroyTexture(game->label);
    game->label = NULL;
}

void init_game_win(GameState *game)
{
    //Create label textures for status screen
    SDL_Color white ={255, 255, 255, 255};

    SDL_Surface *tmp = TTF_RenderText_Blended(game->font, "You Won", white);
    game->labelW = tmp->w;
    game->labelH = tmp->h;
    game->label = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
}

void draw_game_win(GameState *game)
{
    SDL_Renderer *renderer = game->renderer;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    //Clear screen
    SDL_RenderClear(renderer);

    //set drawing color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect textRect = {320-game->labelW/2, 240-game->labelH/2, game->labelW, game->labelH};
    SDL_RenderCopy(renderer, game->label, NULL, &textRect);
}

void shutdown_game_win(GameState *game)
{
    SDL_DestroyTexture(game->label);
    game->label = NULL;
}
