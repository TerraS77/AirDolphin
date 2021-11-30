#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include "renderer.h"
#include <SDL2/SDL_ttf.h>

SDL_Color YELLOW = {255,255,0,255};
SDL_Color CYAN = {0,255,255,255};
SDL_Color WHITE = {255,255,255,255};
SDL_Color BLACK = {0,0,0,255};
SDL_Color GREY = {155,155,155,255};

int wWidth, wHeight;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

typedef struct{
    int x;
    int y;
}Anchor;

void drawSquare(int x, int y, int width);
void DrawCircle(int32_t centreX, int32_t centreY, int32_t radius);
void closeWindow();
void SetDrawColor(SDL_Color Color);
void PrintRectangle(Anchor CSG, Anchor CID,SDL_Color couleur,int border);
void printProgress(Anchor CSG, Anchor CID,SDL_Color couleur,int border,float pourcentage);

void initWindow(int width, int height)
{
    wWidth = width;
    wHeight = height;
    if (SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        window = NULL;
        renderer = NULL;
        SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
        SetDrawColor(BLACK);
        SDL_RenderClear(renderer);
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

void updateAirportRenderer(simulation simulation){
    SetDrawColor(BLACK);
    SDL_RenderClear(renderer);
    SetDrawColor(YELLOW);
    drawSquare(500, 500, 100);
    DrawCircle(200, 200, 20);
  

    PrintRectangle((Anchor){1000,400},(Anchor){1300,600},CYAN,4);
    printProgress((Anchor){200,400},(Anchor){400,405},CYAN,1,0.5);
    SDL_RenderPresent(renderer);

    TTF_Init();    
    TTF_Font *police = TTF_OpenFont("scv.ttf",125);
    // On indique la couleur du texte
    SDL_Color rouge={255,0,0,255}; // R,G,B,A
    // On indique la position du texte
    SDL_Rect position_texte={500,500,300,50}; // position x , position y, longueur, largeur
    // On colle le texte dans une surface
    SDL_Surface *surface=TTF_RenderUTF8_Blended(police, "Runways", rouge);
    // On colle la surface sur une texture
    SDL_Texture *texture=SDL_CreateTextureFromSurface(renderer,surface);
    // On colle la texture sur l'écran avec la position renseignée
    SDL_RenderCopy(renderer,texture,NULL,&position_texte);
    // On rafraichit l'écran
    SDL_RenderPresent(renderer);
    // On libère la mémoire
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_Quit();


}

void SetDrawColor(SDL_Color Color){
    SDL_SetRenderDrawColor(renderer, Color.r,Color.g,Color.b,Color.a);    
}

void PrintRectangle(Anchor CSG, Anchor CID,SDL_Color couleur,int border){
    
    SetDrawColor(couleur);

    SDL_Rect rect1 = {(CSG.x),CSG.y,abs(CID.x-CSG.x),border};
    SDL_RenderFillRect(renderer, &rect1);

    SDL_Rect rect2 = {(CSG.x),CID.y-border,abs(CID.x-CSG.x),border};
    SDL_RenderFillRect(renderer, &rect2);



    SDL_Rect rect3 = {(CSG.x),CSG.y,border,abs(CID.y-CSG.y)};
    SDL_RenderFillRect(renderer, &rect3);

    SDL_Rect rect4 = {(CID.x)-border,CSG.y,border,abs(CID.y-CSG.y)};
    SDL_RenderFillRect(renderer, &rect4);
     }
    

    void printProgress(Anchor CSG, Anchor CID,SDL_Color couleur,int border,float pourcentage){

    SetDrawColor(couleur);

    SDL_Rect rect1 = {(CSG.x),CSG.y,abs(CID.x-CSG.x),border};
    SDL_RenderFillRect(renderer, &rect1);

    SDL_Rect rect2 = {(CSG.x),CID.y-border,abs(CID.x-CSG.x),border};
    SDL_RenderFillRect(renderer, &rect2);



    SDL_Rect rect3 = {(CSG.x),CSG.y,border+pourcentage*((CID.x-border)-(CSG.x+border)),abs(CID.y-CSG.y)};
    SDL_RenderFillRect(renderer, &rect3);

    SDL_Rect rect4 = {(CID.x)-border,CSG.y,border,abs(CID.y-CSG.y)};
    SDL_RenderFillRect(renderer, &rect4);

    }















void drawSquare(int x, int y, int width)
{
    int x1 = x - 0.5 * width;
    int y1 = y - 0.5 * width;
    printf("%d,%d", x1, y1);
    SDL_Rect rect;
    rect.x = x1;
    rect.y = y1;
    rect.h = width;
    rect.w = width;
    SDL_RenderDrawRect(renderer, &rect);
}

void DrawCircle(int32_t centreX, int32_t centreY, int32_t radius)
{
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

void closeWindow()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}
