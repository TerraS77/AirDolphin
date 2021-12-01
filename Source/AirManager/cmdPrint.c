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
void printParkingsList(airport airport, bool completeLogs);

void debugPrintPlane(plane *plane)
{
    printf("│ │ * [%s] : ", plane->matriculation);
    debugPlaneType(plane->type);
    printf("\n");
    printf("│ │ │\033[1m Passengers :\033[0m %u/%u\n", plane->passengers, plane->passengersMax);
    debugPlaneStatus(plane->status);
    if (plane->targetRunway)
        printf(" on runway %d", ((runway *)plane->targetRunway)->id);
    printf("\n│ │ │\n");
}

void debugPlaneType(planeType plane)
{
    switch (plane)
    {
    case AIRLINER:
        printf("AIRLINER");
        break;
    case BUSINESS:
        printf("BUSINESS");
        break;
    case LIGHT:
        printf("LIGHT");
        break;
    }
}

void debugPlaneStatus(planeStatus plane)
{
    switch (plane)
    {
    case FLYING:
        printf("│ │ │ FLYING");
        break;
    case WAITING_LANDING:
        printf("│ │ │ WAITING_LANDING");
        break;
    case LANDING:
        printf("│ │ │ LANDING");
        break;
    case PARKING:
        printf("│ │ │ PARKING");
        break;
    case WAITING_TAKEOFF:
        printf("│ │ │ WAITING_TAKEOFF");
        break;
    case TAKEOFF:
        printf("│ │ │ TAKEOFF");
        break;
    }
}

void debugPrintRunway(runway *runway)
{
    printf("│ │ * [%d] : ", runway->id);
    debugRunwayType(runway->type);
    printf("\n");
    printf("│ │ │ w%.1fm, l%.1fm\n", runway->width, runway->length);
    if(runway->planeLT) printf("│ │ │ \033[1mOn Runway : \033[0m%s\n", runway->planeLT->matriculation);
    printf("│ │ │ \033[1mQueue : \033[0m");
    printf("%d/%u\n", runway->takeoffQueue->length, runway->maxTakeoffQueue);
    if (runway->takeoffQueue->length)
    {
        printf("│ │ │└┐\n");
        for (int n = 0; n < runway->takeoffQueue->length; n++)
        {
            plane *plane = getDataAtIndex(*runway->takeoffQueue, n);
            printf("│ │ │ * \033[3m%s\033[0m\n", plane->matriculation);
        }
        printf("│ │ │┌┘\n");
    }

    printf("│ │ │\n");
}

void debugRunwayType(runwayType runway)
{
    switch (runway)
    {
    case SMALL:
        printf("SMALL");
        break;
    case MEDIUM:
        printf("MEDIUM");
        break;
    case LARGE:
        printf("LARGE");
        break;
    }
}

void debugPrintAirport(airport airport)
{
    printf("\n│\033[1mAirport  \033[0m\n");
    printf("│└┐\n│ \033[1m* In Bound (%d planes)\033[0m\n", airport.planesInRange->length);
    printf("│ │└┐\n");
    printPlanesList(*airport.planesInRange, true);
    printf("│ │┌┘\n");
    printf("│ \033[1m* Parking (%d/%u)\033[0m\n", airport.parkingPlanes->length, airport.parkingSize);
    printf("│ │└┐\n");
    printParkingsList(airport, false);
    printf("│ │┌┘\n");
    printf("│ \033[1m* In Waiting for Landing (%d planes)\033[0m\n", airport.landingQueue->length);
    printf("│ │└┐\n");
    printPlanesList(*airport.landingQueue, true);
    printf("│ │┌┘\n");
    printf("│ \033[1m* Runways (%d runways)\033[0m\n", airport.runways->length);
    printf("│ │└┐\n");
    printRunwaysList(*airport.runways, true);
    printf("│ │┌┘\n");
    printf("│┌┘\n");
    printf("│\n");
}

void printPlanesList(list planesList, bool completeLogs)
{
    for (int n = 0; n < planesList.length; n++)
    {
        plane *plane = getDataAtIndex(planesList, n);
        if (completeLogs)
            debugPrintPlane(plane);
        else
        {
            printf("│ │ │\033[3m %s\033[0m\n", plane->matriculation);
        }
    }
    if (completeLogs)
    {
    }
}

void printRunwaysList(list runwaysList, bool completeLogs)
{
    for (int n = 0; n < runwaysList.length; n++)
    {
        runway *runway = getDataAtIndex(runwaysList, n);
        if (completeLogs)
            debugPrintRunway(runway);
        else
            printf("│ │ │%d\n", runway->id);
    }
    if (completeLogs)
    {
    }
}

void printParkingsList(airport airport, bool completeLogs)
{
    for (int n = 0; n < airport.parkingPlanes->length; n++)
    {
        plane *plane = getDataAtIndex(*airport.parkingPlanes, n);
        if (completeLogs)
            debugPrintPlane(plane);
        else
        {
            if (searchDataInList(*airport.waitForRunwayQueue, plane))
            {
                printf("│ │ * \033[3;33m%s\033[0m\n", plane->matriculation);
            }
            else
            {
                printf("│ │ * \033[3m%s\033[0m\n", plane->matriculation);
            }
        }
    }
    if (completeLogs)
    {
    }
}