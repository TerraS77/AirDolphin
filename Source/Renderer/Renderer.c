/**
 * @file Renderer.c
 * @brief Window manager, simulation renderer, temporisation and menus
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include "renderer.h"
#include "bddManager.h"
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

typedef enum { LEFT, CENTER, RIGHT } textAlign;
typedef enum { MENU_CONTINUE, MENU_SAVE, MENU_OPEN, MENU_QUIT, MENU_NULL} menuAction;
typedef struct {
    Anchor CSG;
    Anchor CID;
    Anchor center;
    menuAction action;
    bool selected;
    char *text;
} button;


void closeWindow();

void SetDrawColor(SDL_Color Color);

void printRectangleWithBorder(Anchor CSG, Anchor CID, SDL_Color couleur, int border);

void printProgress(Anchor CSG, Anchor CID, SDL_Color couleur, int border, float pourcentage);

void printText(char *text, int fontSize, SDL_Color color, Anchor origin, textAlign align);

void interf_AirportToRender(simulation simulation);

void interf_Runway(simulation simulation,runway *runway, Anchor CSG, int w, int h);

void interf_Radar(airport *airport, Anchor left);

void interf_Radar_PrintLine(plane *plane, Anchor left);

void interf_Parking(simulation simulation, Anchor left);

void interf_Parking_PrintLine(sim_planeActor *planeActor, Anchor left, bool isInWFTR);

void interf_launchMenu(simulation *simulation);

button newButton(Anchor center, int h, int w, char *text, menuAction action);

void printButtons(button *buttons, int nButtons);

void updateHoverButtons(button *buttons, int nButtons,int mx,int my);

menuAction getActionButton(button *buttons, int nButtons);

bool isButtonHover(int mx, int my, button button);

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

void updateAirportRenderer(simulation *simulation)
{
    static int frameProcessingDelta = 0;
    static Uint32 frameProcessingTimestamp = 0;
    bool menuActive = false;
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                closeWindow();
                exit(0);
                break;
            case SDL_KEYDOWN :
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        menuActive = !menuActive;
                        break;
                }
                break;
        }
    }

    if(menuActive) interf_launchMenu(simulation);
    SetDrawColor(BLACK);
    SDL_RenderClear(renderer);

    interf_AirportToRender(*simulation);

    SDL_RenderPresent(renderer);
    if(frameProcessingTimestamp) frameProcessingDelta = SDL_GetTicks() - frameProcessingTimestamp;
    int waitTime = simulation->simulationSpeedInMs - frameProcessingDelta;
    if(waitTime > 0) SDL_Delay(waitTime);
    frameProcessingTimestamp = SDL_GetTicks();
}

void interf_launchMenu(simulation *simulation){
    SDL_Event event;
    int mx, my = 0;
    int buttonH = 100;
    int margin = 10;
    int buttonW = 400;
    Anchor center = {wWidth/2, wHeight/2};
    Anchor menuCSG = {center.x - (0.5 * buttonW) - (3.5 * margin), center.y - (2 * buttonH) - (5.5 * margin)};
    Anchor menuCIG = {center.x + (0.5 * buttonW) + (3.5 * margin), center.y + (2 * buttonH) + (5.5 * margin)};
    button buttons[4];
    buttons[0] = newButton((Anchor) {center.x, center.y - 1.5*(margin+buttonH)}, buttonH, buttonW, "RESUME",MENU_CONTINUE);
    buttons[1] = newButton((Anchor) {center.x, center.y - 0.5*(margin+buttonH)}, buttonH, buttonW, "SAVE", MENU_SAVE);
    buttons[2] = newButton((Anchor) {center.x, center.y + 0.5*(margin+buttonH)}, buttonH, buttonW, "OPEN", MENU_OPEN);
    buttons[3] = newButton((Anchor) {center.x, center.y + 1.5*(margin+buttonH)}, buttonH, buttonW, "QUIT", MENU_QUIT);
    while(1){
        SDL_GetMouseState(&mx, &my);
        updateHoverButtons(buttons, 4, mx, my);
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    closeWindow();
                    exit(0);
                    break;
                case SDL_KEYDOWN :
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            return;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN :
                    switch (getActionButton(buttons, 4))
                    {
                        case MENU_CONTINUE:
                            return;
                        case MENU_QUIT:
                            closeWindow();
                            exit(0);
                            return;
                        case MENU_OPEN:
                            openChainFile("database.bdd", simulation);
                            return;
                        case MENU_SAVE:
                            savePlanesInFile(simulation->airport);
                            return;
                    }
                    break;
            }
        }
        // SetDrawColor(BLACK);
        // SDL_RenderClear(renderer);

        // interf_AirportToRender(*simulation);
        printProgress(menuCSG, menuCIG, BLACK, 1, 1);
        printRectangleWithBorder(menuCSG, menuCIG, YELLOW, 2);
        printButtons(buttons, 4);

        SDL_RenderPresent(renderer);
    }
}

button newButton(Anchor center, int h, int w, char *text, menuAction action){
    button newButton;
    newButton.center = center;
    newButton.CSG = (Anchor) {center.x - w/2, center.y - h/2};
    newButton.CID = (Anchor) {center.x + w/2, center.y + h/2};
    newButton.text = text;
    newButton.selected = false;
    newButton.action = action;
    return newButton;
}

void printButtons(button *buttons, int nButtons){
    for(int n = 0; n < nButtons; n++){
        button button = *(buttons + n);
        if(button.selected){
            printProgress(button.CSG, button.CID, YELLOW, 1, 1);
            printText(button.text, 30, BLACK, button.center, CENTER);
        }else{
            printRectangleWithBorder(button.CSG, button.CID, YELLOW, 4);
            printText(button.text, 30, YELLOW, button.center, CENTER);
        }
    }
}

void updateHoverButtons(button *buttons, int nButtons,int mx,int my){
    for(int n = 0; n < nButtons; n++){
        button *button = (buttons + n);
        button->selected = isButtonHover(mx, my, *button);
    }
}

menuAction getActionButton(button *buttons, int nButtons){
    for(int n = 0; n < nButtons; n++){
        button button = *(buttons + n);
        if(button.selected) return button.action;
    }
    return MENU_NULL;
}

bool isButtonHover(int mx, int my, button button){
    bool isHoverX = (mx < button.CID.x) && (mx > button.CSG.x);
    bool isHoverY = (my < button.CID.y) && (my > button.CSG.y);
    return isHoverX && isHoverY;
}

void interf_AirportToRender(simulation simulation){
    interf_Radar(simulation.airport, (Anchor){1450, 25});
    interf_Parking(simulation, (Anchor){15, 25});

    for(int runwayId = 0; runwayId < 6; runwayId++){
        Anchor anchorOfRunway = {610,25};
        if(runwayId > 2) anchorOfRunway.y = 465;
        if(runwayId%3 == 1) anchorOfRunway.x = 880;
        if(runwayId%3 == 2) anchorOfRunway.x = 1150;
        runway *runway = getDataAtIndex(*simulation.airport->runways, runwayId);
        interf_Runway(simulation, runway, anchorOfRunway, 250, 400);
    }
}

void interf_Runway(simulation simulation,runway *runway, Anchor CSG, int w, int h)
{
    if(!runway) return;
    int MGL=20;
    int MGR=10;
    Anchor CID = {CSG.x + w, CSG.y + h};
    printRectangleWithBorder(CSG, CID, WHITE, 4);
    char runnwayMat[10];
    sprintf(runnwayMat, "R%d", runway->id);

    printText(runnwayMat, 40, WHITE, (Anchor){CSG.x + MGL, CSG.y + 30}, RIGHT);
    char runnwayDim[20];
    sprintf(runnwayDim, "w%.0fm l%.0fm", runway->width, runway->length);
    printText(runnwayDim, 15, WHITE, (Anchor){CSG.x + w - MGR, CSG.y + 18}, LEFT);

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

    char indic[2] = "E";
    SDL_Color color = GREY;
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
    printText(indic, 40, color, (Anchor){CSG.x + w - MGR, CSG.y + 140}, LEFT);
    printRectangleWithBorder((Anchor){CSG.x, CSG.y + 65}, (Anchor){CSG.x + w, CSG.y + 160}, color, 4);
    char queueIndic[20];
    sprintf(queueIndic, "Q(%d/%u)", runway->takeoffQueue->length, runway->maxTakeoffQueue);
    printText(queueIndic, 30, WHITE, (Anchor){CSG.x + MGL, CSG.y + 175}, RIGHT);
    for(int p = 0; p < runway->takeoffQueue->length; p++){
        plane *plane = getDataAtIndex(*runway->takeoffQueue, p);
        printText(plane->matriculation, 30, WHITE, (Anchor){CSG.x + 0.5*w, CSG.y + 205+p*25}, CENTER);
    }
}

void interf_Radar_PrintLine(plane *plane, Anchor left)
{
    char idLine[30];
    char typeLet;

    switch (plane->type)
    {
    case AIRLINER:
        typeLet = 'A';
        break;
    case BUSINESS:
        typeLet = 'B';
        break;
    case LIGHT:
        typeLet = 'L';
        break;
    }

    sprintf(idLine, "%c %s", typeLet, plane->matriculation);

    if (plane->status == WAITING_LANDING) {
        printText(idLine, 30, YELLOW, (Anchor){left.x, left.y}, RIGHT);
        printText("WFLR", 30, YELLOW, (Anchor){left.x + 248, left.y}, RIGHT);
    } else {
        printText(idLine, 30, WHITE, (Anchor){left.x, left.y}, RIGHT);
        printText("STDBY", 30, WHITE, (Anchor){left.x + 248, left.y}, RIGHT);
    }
}

void interf_Radar(airport *airport, Anchor left)
{

    int MG = left.x + 10;
    printRectangleWithBorder((Anchor){left.x, left.y}, (Anchor){left.x + 387, left.y + 1030}, WHITE, 4);
    printRectangleWithBorder((Anchor){left.x, left.y}, (Anchor){left.x + 387, left.y + 60}, WHITE, 4);
    printText("RADAR", 40, WHITE, (Anchor){MG, left.y + 30}, RIGHT);

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

    switch (plane->type){
        case AIRLINER:
            typeLet = 'A';
            break;
        case BUSINESS:
            typeLet = 'B';
            break;
        case LIGHT:
            typeLet = 'L';
            break;
    }

    sprintf(idLine, "%c %s", typeLet, plane->matriculation);

    if(isInWFTR){
        printText(idLine, 30, YELLOW, (Anchor){left.x, left.y}, RIGHT);
        printText("WFTR", 30, YELLOW, (Anchor){left.x + 248, left.y}, RIGHT);
    } else { 
        printText(idLine, 30, WHITE, (Anchor){left.x, left.y}, RIGHT);
        printText("STDBY", 30, WHITE, (Anchor){left.x + 248, left.y}, RIGHT);
        float progressBarVal = (planeActor->stateLengthTimeInMs-(float)planeActor->stateRemainTimeInMs)/(float)planeActor->stateLengthTimeInMs;
        if(planeActor->stateRemainTimeInMs < 0) progressBarVal = 1.0;
        printProgress((Anchor){left.x + 379,left.y - 5}, (Anchor){left.x + 522, left.y + 5}, WHITE, 2, progressBarVal);
    }
}

void interf_Parking(simulation simulation, Anchor left)
{
    char strpksize[7];
    int MG = left.x + 10;
    airport *airport = simulation.airport;
    printRectangleWithBorder((Anchor){left.x, left.y}, (Anchor){left.x + 547, left.y + 1030}, WHITE, 4); // Grande case
    printRectangleWithBorder((Anchor){left.x, left.y}, (Anchor){left.x + 547, left.y + 60}, WHITE, 4);   // Case titre
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
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
}

void SetDrawColor(SDL_Color Color) {SDL_SetRenderDrawColor(renderer, Color.r, Color.g, Color.b, Color.a);}

void printRectangleWithBorder(Anchor CSG, Anchor CID, SDL_Color couleur, int border)
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

void closeWindow()
{
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}
