
#include "SDL2/SDL.h"
#include <stdio.h>
#include <time.h>
#include "main.h"
#include "status.h"

#define GRAVITY  0.35f



void initStars(GameState *game)
{
  //init stars
  for(int i = 0; i < NUM_STARS; i++)
  {
    game->stars[i].x = 320+rand()%38400;
    game->stars[i].y = rand()%1080;
  }
}

void loadGame(GameState *game)
{
  SDL_Surface *surface = NULL;

  //Load images and create rendering textures from them
  surface = IMG_Load("star.png");
  if(surface == NULL)
  {
    printf("Cannot find star.png!\n\n");
    SDL_Quit();
    exit(1);
  }

  game->star = SDL_CreateTextureFromSurface(game->renderer, surface);
  SDL_FreeSurface(surface);

  surface = IMG_Load("doomguy1.png");
  if(surface == NULL)
  {
    printf("Cannot find doomguy1.png!\n\n");
    SDL_Quit();
    exit(1);
  }
  game->manFrames[0] = SDL_CreateTextureFromSurface(game->renderer, surface);
  SDL_FreeSurface(surface);

  surface = IMG_Load("doomguy2.png");
  if(surface == NULL)
  {
    printf("Cannot find doomguy2.png!\n\n");
    SDL_Quit();
    exit(1);
  }
  game->manFrames[1] = SDL_CreateTextureFromSurface(game->renderer, surface);
  SDL_FreeSurface(surface);

  surface = IMG_Load("brick.png");
  game->brick = SDL_CreateTextureFromSurface(game->renderer, surface);
  SDL_FreeSurface(surface);

  surface = IMG_Load("fire.png");
  game->fire = SDL_CreateTextureFromSurface(game->renderer, surface);
  SDL_FreeSurface(surface);

  surface = IMG_Load("pillar.png");
  game->pillar = SDL_CreateTextureFromSurface(game->renderer, surface);
  SDL_FreeSurface(surface);

  //Load fonts
  game->font = TTF_OpenFont("AmazDooMLeft2.ttf", 48);
  if(!game->font)
  {
      printf("Cannot find font file!");
      SDL_Quit();
      exit(1);
  }

  //Load sounds
  game->bgMusic = Mix_LoadWAV("only thing.wav");
  if(game->bgMusic != NULL);
  {
      Mix_VolumeChunk(game->bgMusic, 64);
  }
  game->menuMusic = Mix_LoadWAV("at dooms gate.wav");
  if(game->menuMusic != NULL);
  {
      Mix_VolumeChunk(game->menuMusic, 16);
  }
  game->deathSound = Mix_LoadWAV("deathSound.wav");

  game->label = NULL;
  game->cont = NULL;
  game->lifeCounter = NULL;

  game->man.x = 100;
  game->man.y = 240-40;
  game->man.dx = 0;
  game->man.dy = 0;
  game->man.onLedge = 0;
  game->man.animFrame = 0;
  game->man.facingLeft = 1;
  game->man.slowingDown = 0;
  game->man.lives = 3;
  game->man.isDead = 0;
  game->statusState = STATUS_STATE_LIVES;

  init_status_lives(game);

  game->time = 0;
  game->deathCountdown = -1;
  game->scrollX = 0;

  //initStars(game);


  //init ledges
  for(int i = 0; i < NUM_LEDGES; i++)
  {
    game->ledges[i].w = 384;
    game->ledges[i].h = 96;
    game->ledges[i].x = i*384;
    //if(i == 0)
    game->ledges[i].y = 1080-96;
    //else
        //game->ledges[i].y = 300+100-rand()%200;
  }
  game->ledges[99].x = 5*384;
  game->ledges[99].y = 648;
  game->ledges[13].x = 13*384+200;
  game->ledges[13].y = 1080-96;
  game->ledges[98].x = 12*512;
  game->ledges[98].y = 1080-640;
  game->ledges[98].w = 128;
  game->ledges[97].x = 15*512;
  game->ledges[97].y = 1080-640;
  game->ledges[97].w = 384;
  game->ledges[96].x = 16*512;
  game->ledges[96].y = 252;


  //pillars
  for(int i = 0; i < 3; i++)
  {
    game->pillars[i].w = 144;
    game->pillars[i].h = 928;
    game->pillars[i].x = 7*384+i*600;
    game->pillars[i].y = 814-i*85;
  }
  game->pillars[3].x = 7*384+3*600;
  game->pillars[3].y = game->pillars[2].y;
  game->pillars[3].w = 144;
  game->pillars[3].h = 928;
}

//useful utility function to see if two rectangles are colliding at all
int collide2d(float x1, float y1, float x2, float y2, float wt1, float ht1, float wt2, float ht2)
{
  return (!((x1 > (x2+wt2)) || (x2 > (x1+wt1)) || (y1 > (y2+ht2)) || (y2 > (y1+ht1))));
}

void process(GameState *game)
{
  //add time
  game->time++;

  if(game->statusState == STATUS_STATE_GAMEOVER)
  {
    Mix_HaltChannel(game->musicChannel);
    if(game->time > 190)
    {
        SDL_Quit();
        exit(0);
    }
  }

  if(game->statusState == STATUS_STATE_GAME)
  {
   if(game->man.isDead == 0)
   {
  //man movement
  Man *man = &game->man;
  man->x += man->dx;
  man->y += man->dy;

  if(man->dx != 0 && man->onLedge && !man->slowingDown)
  {
    if(game->time % 20 == 0)
    {
      if(man->animFrame == 0)
      {
        man->animFrame = 1;
      }
      else
      {
        man->animFrame = 0;
      }
    }
  }

  if(man->x >38320)
  {
      init_game_win(game);
      game->statusState = STATUS_STATE_WIN;
  }

  man->dy += GRAVITY;
   }

  if(game->man.isDead && game->deathCountdown < 0)
  {
      game->deathCountdown = 120;
  }
  if(game->deathCountdown >= 0)
  {
      game->deathCountdown--;
      if(game->deathCountdown < 0)
      {
          //init_game_over(game);
          //game->statusState = STATUS_STATE_GAMEOVER;

          game->man.lives--;

          if(game->man.lives >= 0)
          {
              init_status_lives(game);
              game->statusState = STATUS_STATE_LIVES;
              game->time = 0;

              //reset
              game->man.isDead = 0;
              game->man.x = 100;
              game->man.y = 240-40;
              game->man.dx = 0;
              game->man.dy = 0;
              game->man.onLedge = 0;
              //initStars(game);
          }
          else
          {
              init_game_over(game);
              game->statusState = STATUS_STATE_GAMEOVER;
              game->time = 0;
          }
       }
    }
  }
  game->scrollX = -game->man.x+320;
  if(game->scrollX > 0)
  {
      game->scrollX = 0;
  }
  if(game->scrollX < -38000+320)
  {
      game->scrollX = -38000+320;
  }
  if(game->man.x < 0)
    game->man.x = 0;
  if(game->man.y < 0)
    game->man.y = 0;
}

void collisionDetect(GameState *game)
{
   for(int i = 0; i < NUM_STARS; i++)
  {
      if(collide2d(game->man.x, game->man.y, game->stars[i].x, game->stars[i].y, 108, 108, 128, 128))
      {
        if(!game->man.isDead)
        {
          game->man.isDead = 1;
          Mix_HaltChannel(game->musicChannel);
          Mix_PlayChannel(-1, game->deathSound, 0);
          }
          break;
      }
  }

  //Check for falling
  if(game->man.y > 1080)
  {
      if(!game->man.isDead)
    {
      game->man.isDead = 1;
      Mix_HaltChannel(game->musicChannel);
      Mix_PlayChannel(-1, game->deathSound, 0);
    }
  }


  //Check for collision with any ledges (brick blocks)
  for(int i = 0; i < NUM_LEDGES; i++)
  {
    float mw = 108, mh = 108;
    float mx = game->man.x, my = game->man.y;
    float bx = game->ledges[i].x, by = game->ledges[i].y, bw = game->ledges[i].w, bh = game->ledges[i].h;

    if(mx+mw/2 > bx && mx+mw/2<bx+bw)
    {
      //are we bumping our head?
      if(my < by+bh && my > by && game->man.dy < 0)
      {
        //correct y
        game->man.y = by+bh;
        my = by+bh;

        //bumped our head, stop any jump velocity
        game->man.dy = 0;
        game->man.onLedge = 1;
      }
    }
    if(mx+mw > bx && mx<bx+bw)
    {
      //are we landing on the ledge
      if(my+mh > by && my < by && game->man.dy > 0)
      {
        //correct y
        game->man.y = by-mh;
        my = by-mh;

        //landed on this ledge, stop any jump velocity
        game->man.dy = 0;
        game->man.onLedge = 1;
      }
    }

    if(my+mh > by && my<by+bh)
    {
      //rubbing against right edge
      if(mx < bx+bw && mx+mw > bx+bw && game->man.dx < 0)
      {
        //correct x
        game->man.x = bx+bw;
        mx = bx+bw;

        game->man.dx = 0;
      }
      //rubbing against left edge
      else if(mx+mw > bx && mx < bx && game->man.dx > 0)
      {
        //correct x
        game->man.x = bx-mw;
        mx = bx-mw;

        game->man.dx = 0;
      }
    }
  }
  for(int i = 0; i < NUM_PILLARS; i++)
  {
    float mw = 108, mh = 108;
    float mx = game->man.x, my = game->man.y;
    float bx = game->pillars[i].x, by = game->pillars[i].y, bw = game->pillars[i].w, bh = game->pillars[i].h;

    if(mx+mw/2 > bx && mx+mw/2<bx+bw)
    {
      //are we bumping our head?
      if(my < by+bh && my > by && game->man.dy < 0)
      {
        //correct y
        game->man.y = by+bh;
        my = by+bh;

        //bumped our head, stop any jump velocity
        game->man.dy = 0;
        game->man.onLedge = 1;
      }
    }
    if(mx+mw > bx && mx<bx+bw)
    {
      //are we landing on the ledge
      if(my+mh > by && my < by && game->man.dy > 0)
      {
        //correct y
        game->man.y = by-mh;
        my = by-mh;

        //landed on this ledge, stop any jump velocity
        game->man.dy = 0;
        game->man.onLedge = 1;
      }
    }

    if(my+mh > by && my<by+bh)
    {
      //rubbing against right edge
      if(mx < bx+bw && mx+mw > bx+bw && game->man.dx < 0)
      {
        //correct x
        game->man.x = bx+bw;
        mx = bx+bw;

        game->man.dx = 0;
      }
      //rubbing against left edge
      else if(mx+mw > bx && mx < bx && game->man.dx > 0)
      {
        //correct x
        game->man.x = bx-mw;
        mx = bx-mw;

        game->man.dx = 0;
      }
    }
  }
}

int processEvents(SDL_Window *window, GameState *game)
{
  SDL_Event event;
  int done = 0;

  while(SDL_PollEvent(&event))
  {
    switch(event.type)
    {
      case SDL_WINDOWEVENT_CLOSE:
      {
        if(window)
        {
          SDL_DestroyWindow(window);
          window = NULL;
          done = 1;
        }
      }
      break;
      case SDL_KEYDOWN:
      {
        if(game->statusState == STATUS_STATE_LIVES)
         {
         Mix_HaltChannel(game->musicChannel);
         game->statusState = STATUS_STATE_GAME;
         shutdown_status_lives(game);
         game->musicChannel = Mix_PlayChannel(-1, game->bgMusic, -1);
         }
        switch(event.key.keysym.sym)
        {
          case SDLK_ESCAPE:
            done = 1;
          break;
          case SDLK_UP:
            if(game->man.onLedge)
            {
              game->man.dy = -11;
              game->man.onLedge = 0;
            }
          break;
        }
      }
      break;
      case SDL_QUIT:
        //quit out of the game
        done = 1;
      break;
    }
  }

  //More jumping
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if(state[SDL_SCANCODE_UP])
  {
    game->man.dy -= 0.17f;
  }

  //Walking
  if(state[SDL_SCANCODE_LEFT])
  {
    game->man.dx -= 1.5;
    if(game->man.dx < -6)
    {
      game->man.dx = -6;
    }
    game->man.facingLeft = 1;
    game->man.slowingDown = 0;
  }
  else if(state[SDL_SCANCODE_RIGHT])
  {
    game->man.dx += 1.5;
    if(game->man.dx > 6)
    {
      game->man.dx = 6;
    }
    game->man.facingLeft = 0;
    game->man.slowingDown = 0;
  }
  else
  {
    game->man.animFrame = 0;
    game->man.dx *= 0.8f;
    game->man.slowingDown = 1;
    if((game->man.dx) < 0.1f)
    {
      game->man.dx = 0;
    }
  }

//  if(state[SDL_SCANCODE_UP])
//  {
//    game->man.y -= 10;
//  }
//  if(state[SDL_SCANCODE_DOWN])
//  {
//    game->man.y += 10;
//  }

  return done;
}

void doRender(SDL_Renderer *renderer, GameState *game)
{
  if(game->statusState == STATUS_STATE_LIVES)
  {
      game->musicChannel = Mix_PlayChannel(-1, game->menuMusic, -1);
      draw_status_lives(game);
  }
  else if(game->statusState == STATUS_STATE_GAMEOVER)
  {
      draw_game_over(game);
  }
  else if(game->statusState == STATUS_STATE_WIN)
  {
      draw_game_win(game);
  }
  else if(game->statusState == STATUS_STATE_GAME)
  {
  //set the drawing color to blue
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

  //Clear the screen (to blue)
  SDL_RenderClear(renderer);

  //set the drawing color to white
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    //draw the pillar image
  for(int i = 0; i < 4; i++)
   {
    SDL_Rect pillarRect = { game->scrollX+game->pillars[i].x, game->pillars[i].y, game->pillars[i].w, game->pillars[i].h };
    SDL_RenderCopy(renderer, game->pillar, NULL, &pillarRect);
   }

  for(int i = 0; i < NUM_LEDGES; i++)
  {
    SDL_Rect ledgeRect = { game->scrollX+game->ledges[i].x, game->ledges[i].y, game->ledges[i].w, game->ledges[i].h };
    SDL_RenderCopy(renderer, game->brick, NULL, &ledgeRect);
  }

  //draw a rectangle at man's position
  SDL_Rect rect = { game->scrollX+game->man.x, game->man.y, 108, 108 };//rect x,y position; width height
  SDL_RenderCopyEx(renderer, game->manFrames[game->man.animFrame],
                   NULL, &rect, 0, NULL, (game->man.facingLeft));

  if(game->man.isDead)
  {
  SDL_Rect rect = { game->scrollX+game->man.x+36-20, game->man.y+36-50, 60, 150 };//rect x,y position; width height
  SDL_RenderCopyEx(renderer, game->fire,
                   NULL, &rect, 0, NULL, (game->time%20 <10));
  }

  //draw the star image
  for(int i = 0; i < NUM_STARS; i++)
   {
    SDL_Rect starRect = { game->scrollX+game->stars[i].x, game->stars[i].y, 128, 128 };
    SDL_RenderCopy(renderer, game->star, NULL, &starRect);
   }
  }
  //We are done drawing, "present" or show to the screen what we've drawn
  SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{
  GameState gameState;
  SDL_Window *window = NULL;                    // Declare a window
  SDL_Renderer *renderer = NULL;                // Declare a renderer

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);              // Initialize SDL2

  srand((int)time(NULL));

  //Create an application window with the following settings:
  window = SDL_CreateWindow("Game Window",                     // window title
                            SDL_WINDOWPOS_UNDEFINED,           // initial x position
                            SDL_WINDOWPOS_UNDEFINED,           // initial y position
                            1920,                               // width, in pixels
                            1080,                               // height, in pixels
                            0                                  // flags
                            );
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  gameState.renderer = renderer;

  TTF_Init(); //initialize font system

  Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);

  loadGame(&gameState);

  // The window is open: enter program loop (see SDL_PollEvent)
  int done = 0;

  //Event loop
  while(!done)
  {
    //Check for events
    done = processEvents(window, &gameState);

    process(&gameState);
    collisionDetect(&gameState);

    //Render display
    doRender(renderer, &gameState);

    //don't burn up the CPU
    SDL_Delay(10);
  }


  //Shutdown game and unload all memory
  SDL_DestroyTexture(gameState.star);
  SDL_DestroyTexture(gameState.manFrames[0]);
  SDL_DestroyTexture(gameState.manFrames[1]);
  SDL_DestroyTexture(gameState.brick);
  SDL_DestroyTexture(gameState.pillar);
  if(gameState.label != NULL)
  {
      SDL_DestroyTexture(gameState.label);
  }
  if(gameState.cont != NULL)
  {
      SDL_DestroyTexture(gameState.cont);
  }
  if(gameState.fire != NULL)
  {
      SDL_DestroyTexture(gameState.fire);
  }
  TTF_CloseFont(gameState.font);

  Mix_FreeChunk(gameState.bgMusic);
  Mix_FreeChunk(gameState.deathSound);
  Mix_FreeChunk(gameState.menuMusic);

  // Close and destroy the window
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);

  TTF_Quit();
  Mix_CloseAudio();

  // Clean up
  SDL_Quit();
  return 0;
}
