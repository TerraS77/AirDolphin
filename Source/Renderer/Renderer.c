#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

int wWidth, wHeight;  
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

void initWindow(int width, int height);
void drawSquare(int x, int y, int width);
void DrawCircle(int32_t centreX, int32_t centreY, int32_t radius);

int main()
{
    initWindow(1920,1080);
    SDL_SetRenderDrawColor(renderer, 255,127,80,255);
    drawSquare(500, 500, 100);
    DrawCircle(200, 200, 20);
    SDL_RenderPresent(renderer);
    sleep(5);
    return 0;
}

void initWindow(int width, int height){
    wWidth = width;
    wHeight = height;
    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        window = NULL;
        renderer = NULL;
        SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

void drawSquare(int x, int y, int width){
    int  x1=x-0.5*width;
    int y1=y-0.5*width;
    printf("%d,%d", x1, y1);
    SDL_Rect rect;
    rect.x=x1;
    rect.y=y1;
    rect.h=width;
    rect.w=width;
    SDL_RenderDrawRect(renderer, &rect);
}

void DrawCircle(int32_t centreX, int32_t centreY, int32_t radius)
{
    // for (int w = 0; w < radius * 2; w++) {
    //     for (int h = 0; h < radius * 2; h++) {
    //         int dx = radius - w; // horizontal offset
    //         int dy = radius - h; // vertical offset
    //         if ((dx*dx + dy*dy) <= (radius * radius))
    //         {
    //             SDL_RenderDrawPoint(renderer, centreX + dx, centreY + dy);
    //         }
    //     }
    // }

    const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}

void closeWindow(){
    SDL_DestroyWindow(window);
    SDL_Quit();
}

