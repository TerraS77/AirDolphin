#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include "renderer.h"
#include <SDL2/SDL_ttf.h>

SDL_Color YELLOW = {255, 255, 0, 255};
SDL_Color CYAN = {0, 255, 255, 255};
SDL_Color WHITE = {255, 255, 255, 255};
SDL_Color BLACK = {0, 0, 0, 255};
SDL_Color GREY = {155, 155, 155, 255};

int wWidth, wHeight;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

typedef struct
{
    int x;
    int y;
} Anchor;

typedef enum
{
    LEFT,
    CENTER,
    RIGHT
} textAlign;

void drawSquare(int x, int y, int width);
void DrawCircle(int32_t centreX, int32_t centreY, int32_t radius);
void closeWindow();
void SetDrawColor(SDL_Color Color);
void PrintRectangle(Anchor CSG, Anchor CID, SDL_Color couleur, int border);
void printProgress(Anchor CSG, Anchor CID, SDL_Color couleur, int border, float pourcentage);
void printText(char *text, int fontSize, SDL_Color color, Anchor origin, textAlign align);

void interf_Runway(simulation simulation,runway *runway, Anchor CSG, int w, int h);
void interf_Radar(airport *airport, Anchor left);
void interf_Radar_PrintLine(plane *plane, Anchor left);
void interf_Parking(simulation simulation, Anchor left);
void interf_Parking_PrintLine(sim_planeActor *planeActor, Anchor left, bool isInWFTR);

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
    TTF_Init();
}

void updateAirportRenderer(simulation simulation)
{
    SetDrawColor(BLACK);
    SDL_RenderClear(renderer);
    
    int x1 = 1496;
    int y1 = 25;
    int x2 = 34;
    int y2 = 25;
    interf_Radar(simulation.airport, (Anchor){x1, y1});
    interf_Parking(simulation, (Anchor){x2, y2});

    //TODO : PRINT ALL 6 R
    interf_Runway(simulation,((runway *)simulation.airport->runways->first->data), (Anchor){1000, 25}, 250, 400);
    interf_Runway(simulation,((runway *)simulation.airport->runways->last->data), (Anchor){1000, 450}, 250, 400);
    SDL_RenderPresent(renderer);
}

void interf_Runway(simulation simulation,runway *runway, Anchor CSG, int w, int h)
{
    int MGL=20;
    int MGR=10;
    Anchor CID = {CSG.x + w, CSG.y + h};
    PrintRectangle(CSG, CID, WHITE, 4);
    char string[10];
    sprintf(string, "R%d", runway->id);

    printText(string, 40, WHITE, (Anchor){CSG.x + MGL, CSG.y + 30}, RIGHT);
    char string2[20];
    sprintf(string2, "w%.0fm l%.0fm", runway->width, runway->length);
    printText(string2, 15, WHITE, (Anchor){CSG.x + w - MGR, CSG.y + 18}, LEFT);

    switch (runway->type) {
        case SMALL:
            printText("SMALL", 25, WHITE, (Anchor){CSG.x + w - MGR, CSG.y + 40}, LEFT);
            break;
        case MEDIUM:
            printText("MEDIUM", 25, WHITE, (Anchor){CSG.x + w - MGR, CSG.y + 40}, LEFT);
            break;
        case LARGE:
            printText("LARGE", 25, WHITE, (Anchor){CSG.x + w - MGR, CSG.y + 40}, LEFT);
            break;
    }

    char indic[2] = " ";
    SDL_Color color;
    if (runway->planeLT)
    {
        switch (runway->planeLT->status) {
        case LANDING:;
            indic[0] = 'L';
            color = YELLOW;
            break;
        case TAKEOFF:;
            indic[0] = 'T';
            color = CYAN;
            break;
        }
        printText(runway->planeLT->matriculation, 40, color, (Anchor){CSG.x + 0.5 * w, CSG.y + 115}, CENTER); 
        sim_planeActor *actor =  getSimPlaneActorInList(runway->planeLT, &simulation);
        float progressBarVal = ((float)actor->stateLengthTimeInMs-(float)actor->stateRemainTimeInMs)/(float)actor->stateLengthTimeInMs;
        if(actor->stateRemainTimeInMs < 0) progressBarVal = 1.0;
        printProgress((Anchor){CSG.x, CSG.y + 65}, (Anchor){CSG.x + w, CSG.y + 78}, color, 4,progressBarVal);
    }
    else
    {
        indic[0] = 'E';
        color = GREY;
    }

    printText(indic, 40, color, (Anchor){CSG.x + w - MGR, CSG.y + 140}, LEFT);
    PrintRectangle((Anchor){CSG.x, CSG.y + 65}, (Anchor){CSG.x + w, CSG.y + 160}, color, 4);

    char string4[20];
    sprintf(string4, "Q(%d/%u)", runway->takeoffQueue->length, runway->maxTakeoffQueue);
    printText(string4, 30, WHITE, (Anchor){CSG.x + MGL, CSG.y + 175}, RIGHT);
    for(int p = 0; p < runway->takeoffQueue->length; p++){
        plane *plane = getDataAtIndex(*runway->takeoffQueue, p);
        printText(plane->matriculation, 30, WHITE, (Anchor){CSG.x + 0.5*w, CSG.y + 205+p*25}, CENTER);
    }
   
}

void interf_Radar_PrintLine(plane *plane, Anchor left)
{
    char idLine[30];
    char typeLet;

    switch ((plane->type))
    {
    case AIRLINER:
        typeLet = 'A';
        sprintf(idLine, "%c %s", typeLet, plane->matriculation);
        break;
    case BUSINESS:
        typeLet = 'B';
        sprintf(idLine, "%c %s", typeLet, plane->matriculation);
        break;
    case LIGHT:
        typeLet = 'L';
        sprintf(idLine, "%c %s", typeLet, plane->matriculation);
        break;
    }

    if (plane->status == WAITING_LANDING)
    {
        printText(idLine, 30, YELLOW, (Anchor){left.x, left.y}, RIGHT);
        printText("WFLR", 30, YELLOW, (Anchor){left.x + 248, left.y}, RIGHT);
    }
    else if (plane->status == FLYING)
    {
        printText(idLine, 30, WHITE, (Anchor){left.x, left.y}, RIGHT);
        printText("STDBY", 30, WHITE, (Anchor){left.x + 248, left.y}, RIGHT);
    }
    else
    {
        printText(idLine, 30, CYAN, (Anchor){left.x, left.y}, RIGHT);
        printText("ERROR", 30, CYAN, (Anchor){left.x + 248, left.y}, RIGHT);
    }
}

void interf_Radar(airport *airport, Anchor left)
{

    int MG = left.x + 10;
    PrintRectangle((Anchor){left.x, left.y}, (Anchor){left.x + 387, left.y + 1030}, WHITE, 4); // Grande case
    PrintRectangle((Anchor){left.x, left.y}, (Anchor){left.x + 387, left.y + 60}, WHITE, 4);   // Case titre
    printText("RADAR", 40, WHITE, (Anchor){MG, left.y + 30}, RIGHT);                           // Affichage titre

    Anchor anchorText = {left.x + 10, left.y + 90};
    int deltaY = 30;
    for (int p = 0; p < airport->landingQueue->length; p++)
    {
        plane *plane = getDataAtIndex(*airport->landingQueue, p);
        interf_Radar_PrintLine(plane, anchorText);
        anchorText.y += deltaY;
    }
    for (int p = 0; p < airport->planesInRange->length; p++)
    {

        plane *plane = getDataAtIndex(*airport->planesInRange, p);
        if (plane->status == FLYING)
        {
            interf_Radar_PrintLine(plane, anchorText);
            anchorText.y += deltaY;
        }
    }
}

void interf_Parking_PrintLine(sim_planeActor *planeActor, Anchor left, bool isInWFTR)
{
    plane *plane = planeActor->plane;
    char idLine[30];
    char typeLet;

    switch (plane->type)
    {
    case AIRLINER:
        typeLet = 'A';
        sprintf(idLine, "%c %s", typeLet, plane->matriculation);
        break;
    case BUSINESS:
        typeLet = 'B';
        sprintf(idLine, "%c %s", typeLet, plane->matriculation);
        break;
    case LIGHT:
        typeLet = 'L';
        sprintf(idLine, "%c %s", typeLet, plane->matriculation);
        break;
    }

    if(isInWFTR){
        printText(idLine, 30, YELLOW, (Anchor){left.x, left.y}, RIGHT);
        printText("WFTR", 30, YELLOW, (Anchor){left.x + 248, left.y}, RIGHT);
    } else { 
        printText(idLine, 30, WHITE, (Anchor){left.x, left.y}, RIGHT);
        printText("STDBY", 30, WHITE, (Anchor){left.x + 248, left.y}, RIGHT);
        float progressBarVal = (planeActor->stateLengthTimeInMs-(float)planeActor->stateRemainTimeInMs)/(float)planeActor->stateLengthTimeInMs;
        if(planeActor->stateRemainTimeInMs < 0) progressBarVal = 1.0;
        printProgress((Anchor){left.x + 379,left.y - 5}, (Anchor){left.x + 522, left.y + 5}, WHITE, 2, 0.5);
    }
}

void interf_Parking(simulation simulation, Anchor left)
{
    char strpksize[7];
    int MG = left.x + 10;
    airport *airport = simulation.airport;
    PrintRectangle((Anchor){left.x, left.y}, (Anchor){left.x + 547, left.y + 1030}, WHITE, 4); // Grande case
    PrintRectangle((Anchor){left.x, left.y}, (Anchor){left.x + 547, left.y + 60}, WHITE, 4);   // Case titre
    printText("PARKING", 40, WHITE, (Anchor){MG, left.y + 30}, RIGHT);                         // Affichage titre

    sprintf(strpksize, "%d/%u", airport->parkingPlanes->length, airport->parkingSize);
    printText(strpksize, 40, WHITE, (Anchor){MG + 517, left.y + 30}, LEFT);

    Anchor anchorText = {left.x + 10, left.y + 90};
    int deltaY = 30;
    for (int p = 0; p < airport->waitForRunwayQueue->length; p++)
    {
        plane *plane = getDataAtIndex(*airport->parkingPlanes, p);
        sim_planeActor *actor =  getSimPlaneActorInList(plane, &simulation);
        interf_Parking_PrintLine(actor, anchorText, true);
        anchorText.y += deltaY;
    }
    for (int p = 0; p < airport->parkingPlanes->length; p++)
    {
        plane *plane = getDataAtIndex(*airport->parkingPlanes, p);
        if (!searchDataInList(*airport->waitForRunwayQueue, plane)) {
            sim_planeActor *actor =  getSimPlaneActorInList(plane, &simulation);
            interf_Parking_PrintLine(actor, anchorText, false);
            anchorText.y += deltaY;
        }
    }
}

void printText(char *text, int fontSize, SDL_Color color, Anchor origin, textAlign align)
{
    int w, h;
    TTF_Font *font = NULL;
    font = TTF_OpenFont("scv.ttf", fontSize);
    TTF_SizeText(font, text, &w, &h);
    SDL_Rect position_texte;
    switch (align)
    {
    case LEFT:
        position_texte = (SDL_Rect){origin.x - w, origin.y - 0.5 * h, w, h};
        break;
    case CENTER:
        position_texte = (SDL_Rect){origin.x - 0.5 * w, origin.y - 0.5 * h, w, h};
        break;
    case RIGHT:
        position_texte = (SDL_Rect){origin.x, origin.y - 0.5 * h, w, h};
        break;
    }
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, &position_texte);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
}

void SetDrawColor(SDL_Color Color)
{
    SDL_SetRenderDrawColor(renderer, Color.r, Color.g, Color.b, Color.a);
}

void PrintRectangle(Anchor CSG, Anchor CID, SDL_Color couleur, int border)
{

    SetDrawColor(couleur);

    SDL_Rect rect1 = {(CSG.x), CSG.y, abs(CID.x - CSG.x), border};
    SDL_RenderFillRect(renderer, &rect1);

    SDL_Rect rect2 = {(CSG.x), CID.y - border, abs(CID.x - CSG.x), border};
    SDL_RenderFillRect(renderer, &rect2);

    SDL_Rect rect3 = {(CSG.x), CSG.y, border, abs(CID.y - CSG.y)};
    SDL_RenderFillRect(renderer, &rect3);

    SDL_Rect rect4 = {(CID.x) - border, CSG.y, border, abs(CID.y - CSG.y)};
    SDL_RenderFillRect(renderer, &rect4);
}

void printProgress(Anchor CSG, Anchor CID, SDL_Color couleur, int border, float pourcentage)
{

    SetDrawColor(couleur);

    SDL_Rect rect1 = {(CSG.x), CSG.y, abs(CID.x - CSG.x), border};
    SDL_RenderFillRect(renderer, &rect1);

    SDL_Rect rect2 = {(CSG.x), CID.y - border, abs(CID.x - CSG.x), border};
    SDL_RenderFillRect(renderer, &rect2);

    SDL_Rect rect3 = {(CSG.x), CSG.y, border + pourcentage * ((CID.x - border) - (CSG.x + border)), abs(CID.y - CSG.y)};
    SDL_RenderFillRect(renderer, &rect3);

    SDL_Rect rect4 = {(CID.x) - border, CSG.y, border, abs(CID.y - CSG.y)};
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

void closeWindow()
{
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}
