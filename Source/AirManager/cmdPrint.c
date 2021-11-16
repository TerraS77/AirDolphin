#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cmdPrint.h"

void debugPrintPlane(plane plane)
{
    printf("Informations of plane :\n");
    printf("Matriculation : %s\n", plane.matriculation);
    printf("Number of Passengers : %u\n", plane.passengers);
    printf("Number Max of Passengers : %u\n", plane.passengersMax);
    debugPlaneType(plane.type);
    printf("\n");
    debugPlaneStatus(plane.status);
    // runway *targetRunway;
}

void debugPlaneType(plane plane)
{
    switch (planeType)
    {
    case AIRLINER:
        printf("AIRLINER");
        break;
    case MEDIUM:
        printf("MEDIUM");
        break;
    case LARGE:
        printf("LARGE");
        break;
    }
}

void debugPlaneStatus(plane plane)
{
    switch (planeStatus)
    {
    case FLYING:
        printf("FLYING");
        break;
    case WAITING_LANDING:
        printf(" WAITING_LANDING");
        break;
    case LANDING:
        printf("LANDING");
        break;
    case PARKING:
        printf("PARKING");
        break;
    case WAITING_TAKEOFF:
        printf(" WAITING_TAKEOFF");
        break;
    case TAKEOFF:
        printf("TAKEOFF");
        break;
    }
}

void debugPrintRunway(runway runway)
{
    printf("Informations of runway :\n");
    printf("ID : %s\n", runway.id);
    printf("Lenght : %f\n", runway.lenght);
    printf("Width: %f\n", runway.width);
    printf("Max Take Off Queue: %u\n", runway.maxTakeofQueue);
    debugRunwayType(runway.type);
    printf("\n");
    // debugRunwayStatus(runway.status); En attente de réponse

    //list *takeofQueue;
    //plane *planeLT;
}

void debugRunwayType(runway runway)
{
    switch (runwayType)
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

void debugRunwayStatus(runway runway) // Fonction à peut-être placer
{
    switch (runwayStatus)
    {
    case STANDBY:
        printf("STANDBY");
        break;
    case LANDING:
        printf("LANDING");
        break;
    case TAKEOFF:
        printf("TAKEOFF");
        break;
    }
}

void debugPrintAirport(airport airport)
{
    printf("Informations of airport :\n");
    printf("Size of parking : %u\n", airport.parkingSize);
    
    //list *parkingPlanes;
    //list *runway;
    //list *planesInRange;
}