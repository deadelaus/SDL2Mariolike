#ifndef sdl2_main_h
#define sdl2_main_h


#define STATUS_STATE_LIVES 0
#define STATUS_STATE_GAME 1
#define STATUS_STATE_GAMEOVER 2
#define STATUS_STATE_WIN 3

#define NUM_STARS 100
#define NUM_LEDGES 120
#define NUM_PILLARS 6

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"

typedef struct
{
  float x, y;
  float dx, dy;
  short lives;
  char *name;
  int onLedge, isDead;

  int animFrame, facingLeft, slowingDown;
} Man;

typedef struct
{
  int x, y;
} Star;

typedef struct
{
  int x, y, w, h;
} Ledge;

typedef struct
{
    int x, y, w, h;
} Pillar;

typedef struct
{
  float scrollX;

  //Players
  Man man;

  //Stars
  Star stars[NUM_STARS];

  //Ledges
  Ledge ledges[NUM_LEDGES];

  //Pillars
  Pillar pillars[NUM_PILLARS];

  //Images
  SDL_Texture *star;
  SDL_Texture *manFrames[2];
  SDL_Texture *brick;
  SDL_Texture *label;
  SDL_Texture *cont;
  SDL_Texture *lifeCounter;
  SDL_Texture *fire;
  SDL_Texture *pillar;
  int labelW, labelH;
  int contW, contH;
  int lifeCounterW, lifeCounterH;

  //Fonts
  TTF_Font *font;

  int time, deathCountdown;
  int statusState;

  //Sounds
  int musicChannel;
  Mix_Chunk *menuMusic, *bgMusic, *jumpSound, *landSound, *deathSound;

  //Renderer
  SDL_Renderer *renderer;
} GameState;




#endif // sdl2_main_h
