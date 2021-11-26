#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cmdPrint.h"

void debugPlaneType(planeType plane);
void debugPlaneStatus(planeStatus plane);
void debugRunwayType(runwayType runway);
void printPlanesList(list planesList, bool completeLogs);
void printRunwaysList(list runwaysList, bool completeLogs);

void debugPrintPlane(plane *plane)
{
    printf("\n\t\t     #############################");
    printf("\n\t\t     |                           |");
    printf("\n\t\t     |\033[1m   Informations of plane  \033[0m |");
    printf("\n\t\t     |                           |");
    printf("\n\t\t     #############################\n\n\n");
    printf("\t\t\t\033[1mMatriculation :\033[0m %s\n", plane->matriculation);
    printf("\t\t\t\033[1mNumber of Passengers :\033[0m %u\n", plane->passengers);
    printf("\t\t\t\033[1mNumber Max of Passengers :\033[0m %u\n", plane->passengers);
    debugPlaneType(plane->type);
    debugPlaneStatus(plane->status);
    if(plane->targetRunway) printf("\t\t\t\033[1mTargetRunway :\033[0m %d\n", ((runway*) plane->targetRunway)->id);
}

void debugPlaneType(planeType plane)
{
    switch (plane)
    {
    case AIRLINER:
        printf("\t\t\t\033[1mPlaneType :\033[0m AIRLINER\n");
        break;
    case MEDIUM:
        printf("\t\t\t\033[1mPlaneType :\033[0m MEDIUM\n");
        break;
    case LARGE:
        printf("\t\t\t\033[1mPlaneType :\033[0m LARGE\n");
        break;
    }
}

void debugPlaneStatus(planeStatus plane)
{
    switch (plane)
    {
    case FLYING:
        printf("\t\t\t\033[1mPlaneStatus :\033[0m FLYING\n");
        break;
    case WAITING_LANDING:
        printf("\t\t\t\033[1mPlaneStatus :\033[0m WAITING_LANDING\n");
        break;
    case LANDING:
        printf("\t\t\t\033[1mPlaneStatus :\033[0m LANDING\n");
        break;
    case PARKING:
        printf("\t\t\t\033[1mPlaneStatus :\033[0m PARKING\n");
        break;
    case WAITING_TAKEOFF:
        printf("\t\t\t\033[1mPlaneStatus :\033[0m WAITING_TAKEOFF\n");
        break;
    case TAKEOFF:
        printf("\t\t\t\033[1mPlaneStatus :\033[0m TAKEOFF\n");
        break;
    }
}

void debugPrintRunway(runway *runway)
{   
    printf("\n\t\t     =============================");
    printf("\n\t\t    -|                           |-");
    printf("\n\t\t   | |\033[1m  Informations of runway\033[0m   | |");
    printf("\n\t\t    -|                           |-");
    printf("\n\t\t     =============================\n\n\n");
    printf("\t\t\t\033[1mID :\033[0m %d\n", runway->id);
    printf("\t\t\t\033[1mLenght :\033[0m %f\n", runway->length);
    printf("\t\t\t\033[1mWidth:\033[0m %f\n", runway->width);
    printf("\t\t\t\033[1mMax Take Off Queue:\033[0m %u\n", runway->maxTakeoffQueue);
    debugRunwayType(runway->type);
    printPlanesList(*runway->takeoffQueue, false);
    printf("\t\t\t\033[1mMatriculation :\033[0m %s\n", runway->planeLT->matriculation);
}

void debugRunwayType(runwayType runway)
{
    switch (runway)
    {
    case SMALL:
        printf("\t\t\t\033[1mRunwayType :\033[0m SMALL \n");
        break;
    case MEDIUM:
        printf("\t\t\t\033[1mRunwayType :\033[0m MEDIUM\n");
        break;
    case LARGE:
        printf("\t\t\t\033[1mRunwayType :\033[0m LARGE \n");
        break;
    }
}

void debugPrintAirport(airport airport)
{
    printf("\n\t\t     ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤");
    printf("\n\t\t    ¤|                           |¤");
    printf("\n\t\t  ¤ ¤|\033[1m  Informations of airport  \033[0m|¤ ¤");
    printf("\n\t\t    ¤|                           |¤");
    printf("\n\t\t     ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤\n\n\n");
    printf("\t\t\t\033[1mSize of parking :\033[0m %u\n", airport.parkingSize);
    printf("\n\n\t\t    ~~~~ \033[4mPlanes in the parking\033[0m ~~~~\n\n");
    printPlanesList(*airport.parkingPlanes, false); // afficher la liste des matricules parkings
    printf("\n\n\t\t       ~~~~ \033[4mActives Runways\033[0m ~~~~\n\n");
    printRunwaysList(*airport.runways, true); // afficher la liste des pistes en état DebugRunway
    printf("\n\n\t\t       ~~~~ \033[4mPlanes in range\033[0m ~~~~\n\n");
    printPlanesList(*airport.parkingPlanes, true); // afficher la liste des matricules de tous les avions DebugPlane
    printf("\n\n\t\t     ~~~~ \033[4mPlanes Landing Queue\033[0m ~~~~\n\n");
    printPlanesList(*airport.landingQueue, false); // afficher la liste des matricules dans la takeoffqueue
    printf("\n\n\t     ~~~~ \033[4mPlanes In Waiting Of Take Off Queue\033[0m ~~~~\n\n"); 
    printPlanesList(*airport.waitForRunwayQueue, false); // afficher la liste des matricules en attente d'être dans la takeoffqueue
}

void printPlanesList(list planesList, bool completeLogs)
{
    for (int n = 0; n < planesList.length; n++)
    {
        plane *plane = getDataAtIndex(planesList, n);
        if(completeLogs) debugPrintPlane(plane);
        else printf("\t\t\t\t%s\n", plane->matriculation);
    }
    if(completeLogs) {}
}

void printRunwaysList(list runwaysList, bool completeLogs)
{
    for (int n = 0; n < runwaysList.length; n++)
    {
        runway *runway = getDataAtIndex(runwaysList, n);
        if(completeLogs) debugPrintRunway(runway);
        else printf("\t\t\t\t%d\n", runway->id);
    }
    if(completeLogs) {}
}
