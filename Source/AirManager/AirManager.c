#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
    if(!isRunwayFree(runway))
        printf("ERROR : Acces given to used runway (%s on %d)\n", plane->matriculation, runway->id);
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
    plane *planeToTakeoff = runway->takeoffQueue->first->data;
    deleteInList(runway->takeoffQueue, planeToTakeoff);
    addPlaneToRunway(runway, planeToTakeoff);
}

void addPlaneToRunwayQueue(runway *runway, plane *plane){
    if(!isRunwayQueueFull(runway))
        printf("ERROR : Acces given to full runway queue (%s on %d)\n", plane->matriculation, runway->id);
    plane->targetRunway = runway;
    plane->status = WAITING_TAKEOFF;
    appendInList(runway->takeoffQueue, plane);
}

bool isRunwayQueueFull(runway *runway){
    return runway->maxTakeoffQueue <= runway->takeoffQueue->length;
}

airport* newAirport(unsigned int parkingSize){
    airport *airport_new = malloc(sizeof(airport));
    airport_new->planesInRange = newList(comparePointer);
    airport_new->landingQueue = newList(comparePointer);
    airport_new->parkingPlanes = newList(comparePointer);
    airport_new->waitForRunwayQueue = newList(comparePointer);
    airport_new->runways = newList(comparePointer);
    airport_new->parkingSize = parkingSize;
    return airport_new;
}

void buildAirport(airport* airport, int numberOfSmallRunway, int numberOfMediumRunway, int numberOfLargeRunway){
    for(int NOLR = 0; NOLR<numberOfLargeRunway; NOLR++){
        runway *newLargeRunway = newRunway(2900+NOLR*100,100, LARGE, 4);
        appendInList(airport->runways, newLargeRunway);
    }
    for(int NOMR = 0; NOMR<numberOfMediumRunway; NOMR++){
        runway *newMediumRunway = newRunway(1900+NOMR*100,100, MEDIUM, 6);
        appendInList(airport->runways, newMediumRunway);
    }
    for(int NOSR = 0; NOSR<numberOfSmallRunway; NOSR++){
        runway *newSmallRunway = newRunway(1200+NOSR*100,100, SMALL, 8);
        appendInList(airport->runways, newSmallRunway);
    }
}

void addPlaneToParking(airport* airport, plane *plane){
    if(!isParkingFull(airport))
        printf("ERROR : Acces given to full parking (%s)\n", plane->matriculation);
    plane->status = PARKING;
    appendInList(airport->parkingPlanes, plane);
}

bool isParkingFull(airport* airport){
    return airport->parkingSize <= airport->parkingPlanes->length;
}

bool isParkingQueueFull(airport* airport){
    int howManyPlanesAreLandingOnRunways;
    for(int NR = 0; NR < airport->runways->length; NR++){
        runway *runwayN = getDataAtIndex(*airport->runways, NR);
        if(runwayN->planeLT != NULL)
            howManyPlanesAreLandingOnRunways += runwayN->planeLT->status == LANDING;
    }
    return airport->parkingSize <= airport->parkingPlanes->length + howManyPlanesAreLandingOnRunways;
}

//LandingQueue List Requests
void addPlaneToLandingQueue(airport* airport, plane *plane){
    plane->status = WAITING_LANDING;
    appendInList(airport->landingQueue, plane);
}

void grantPlaneInLQAccessToRunway(airport* airport, runway *runway, plane *plane){
    if(!isRunwayFree(runway)) printf("ERROR : Landing clearance to used runway (%s on %d)\n", plane->matriculation, runway->id);
    if(!isParkingFull(airport)) printf("ERROR : Landing clearance when Parking Queue full (%s on %d)\n", plane->matriculation, runway->id);
    deleteInList(airport->landingQueue, plane);
    plane->status = LANDING;
    plane->targetRunway = runway;
    runway->planeLT = plane;
}

//AskForRunwayQueue List Requests
void addPlaneToAFRQ(airport* airport, plane *plane){
    appendInList(airport->waitForRunwayQueue, plane);
}

void grantPlaneInAFRQAccessToRunway(airport* airport, runway *runway, plane *plane){
    deleteInList(airport->waitForRunwayQueue, plane);
    deleteInList(airport->parkingPlanes, plane);
    addPlaneToRunwayQueue(runway, plane);
}