#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bool.h>

#include "AirManager.h"

int comparePointer(void *data1, void *data2){
    if(data1 == data2) return 0;
    else return 1;
}


plane* newPlane(char matriculation[7], planeType type, unsigned int passengers, unsigned int passengersMax, planeStatus status){
    plane *plane_new = malloc(sizeof(plane));
    strcpy(plane_new->matriculation, matriculation);
    plane_new->passengers = passengers;
    plane_new->passengersMax = passengersMax;
    plane_new->status = status;
    plane_new->targetRunway = NULL;
    plane_new->type = type;
    return plane_new;
}

void loadPlainInAirport(airport* airport, plane *plane){
    appendInList(airport->planesInRange, plane);
    if(plane->status == PARKING) appendInList(airport->parkingPlanes, plane);
}

runway* newRunway(float length, float width, runwayType type, unsigned int maxTakeoffQueue){
    runway* runway_new = malloc(sizeof(runway));
    static char id = 0;
    runway_new->id =  id;
    id++;
    runway_new->length = length;
    runway_new->width = width;
    runway_new->type = type;
    runway_new->maxTakeoffQueue = maxTakeoffQueue;
    runway_new->takeoffQueue = newList(comparePointer);
    runway_new->planeLT = NULL;
    return runway_new;
}

bool isRunwayFree(runway* newRunway){
    return newRunway->planeLT == NULL;
}

//Runway Slot Manager
void addPlaneToRunway(runway *runway, plane *plane){
    runway->planeLT = plane;
    if(plane->status == WAITING_LANDING)
        plane->status = LANDING;
    else plane->status = TAKEOFF;
    plane->targetRunway = runway;
}

void planeExitRunway(runway *runway, plane *plane){
    runway->planeLT = NULL;
    if(plane->status == TAKEOFF)
        plane->status = FLYING;
    else plane->status = PARKING;
    plane->targetRunway = NULL;
}

//Runway Queue Manager
void grantTakeoffForRunway(runway *runway){
    if(runway->takeoffQueue->length == 0) return;
    plane *planeToTakeoff = runway->takeoffQueue->first;
    deleteInList(runway->takeoffQueue, planeToTakeoff);
    addPlaneToRunway(runway, planeToTakeoff);
}

void addPlaneToRunwayQueue(runway *runway, plane *plane){
    plane->targetRunway = runway;
    plane->status = WAITING_TAKEOFF;
    appendInList(runway->takeoffQueue, plane);
}


airport* newAirport(unsigned int parkingSize);
void buildAirport(airport* airport, int numberOfSmallRunway, int numberOfMediumRunway, int numberOfLargeRunway);

void addPlaneToParking(airport* airport, plane *plane);

//LandingQueue List Requests
void addPlaneToLandingQueue(airport* airport, plane *plane);
void grantPlaneInLQAccessToRunway(airport* airport, runway *runway, plane *plane);

//AskForRunwayQueue List Requests
void addPlaneToAFRQ(airport* airport, plane *plane);
void grantPlaneInAFRQAccessToRunway(airport* airport, runway *runway, plane *plane);