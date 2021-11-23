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

void removePlane(airport *airport, plane *plane){
    deleteInList(airport->planesInRange, plane);
    free(plane);
}

void loadPlainInAirport(airport* airport, plane *plane){
    appendInList(airport->planesInRange, plane);
    if(plane->status == PARKING) appendInList(airport->parkingPlanes, plane);
}

bool canItLandHere(plane *plane, runway *runway){
    if(plane->type == AIRLINER)
        return runway->type == LARGE;
    if(plane->type == LIGHT)
        return runway->type == SMALL;
    return true;
}

bool canAPlaneInLQLandHere(airport *airport, runway *runway){
    if(isParkingQueueFull(airport)) return false;
    int nWhoCanLand = 0;
    for(int p = 0; p<airport->landingQueue->length; p++){
        plane *plane = getDataAtIndex(*airport->landingQueue, p);
        nWhoCanLand += canItLandHere(plane, runway);
    }
    return nWhoCanLand > 0;
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
        printf("\033[1;31mERROR : Acces given to used runway (%s on %d, used by %s)\033[0m\n", plane->matriculation, runway->id, runway->planeLT->matriculation);
    runway->planeLT = plane;
    if(plane->status == WAITING_LANDING) plane->status = LANDING;
    else plane->status = TAKEOFF;
    printf("\033[0;34mCONTROL : %s, you are clear for %s on runway %d.\033[0m\n", plane->matriculation, plane->status == LANDING ? "landing" : "takeoff", runway->id);
    plane->targetRunway = runway;
}

void planeExitRunway(runway *runway, plane *plane){
    runway->planeLT = NULL;
    if(plane->status == TAKEOFF){
        plane->status = FLYING;
        printf("\033[0;34m%s : Airborn.\033[0m\n", plane->matriculation);
    
    }else{
        plane->status = PARKING;
        printf("\033[0;34m%s : Landing succesful, going to parking.\033[0m\n", plane->matriculation);
    }
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
    if(isRunwayQueueFull(runway))
        printf("\033[1;31mERROR : Acces given to full runway queue (%s on %d)\033[0m\n", plane->matriculation, runway->id);
    plane->targetRunway = runway;
    plane->status = WAITING_TAKEOFF;
    printf("\033[0;34mCONTROL : %s, take place on queue for takeoff on runway %d.\033[0m\n", plane->matriculation, runway->id);
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
    if(isParkingFull(airport))
        printf("\033[1;31mERROR : Acces given to full parking (%s)\033[0m\n", plane->matriculation);
    plane->status = PARKING;
    appendInList(airport->parkingPlanes, plane);
}

bool isParkingFull(airport* airport){
    return airport->parkingSize <= airport->parkingPlanes->length;
}

bool isParkingQueueFull(airport* airport){
    int howManyPlanesAreLandingOnRunways = 0;
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
    printf("\033[0;34m%s : Request for landing.\033[0m\n", plane->matriculation);
}

void grantPlaneInLQAccessToRunway(airport* airport, runway *runway, plane *plane){
    if(!isRunwayFree(runway)) printf("\033[1;31mERROR : Landing clearance to used runway (%s on %d)\033[0m\n", plane->matriculation, runway->id);
    if(isParkingFull(airport)) printf("\033[1;31mERROR : Landing clearance when Parking Queue full (%s on %d)\033[0m\n", plane->matriculation, runway->id);
    deleteInList(airport->landingQueue, plane);
    addPlaneToRunway(runway, plane);
}

void grantNextInLQAccessToRunway(airport* airport, runway *runway){
    for(int p = 0; p<airport->landingQueue->length; p++){
        plane *plane = getDataAtIndex(*airport->landingQueue, p);
        if(canItLandHere(plane, runway)){
            grantPlaneInLQAccessToRunway(airport, runway, plane);
            return;
        }
    }
    printf("\033[1;31mERROR : Landing clearance to uncompatible runway, or no plane in LQ (%d)\033[0m\n", runway->id);
}

//AskForRunwayQueue List Requests
void addPlaneToAFRQ(airport* airport, plane *plane){
    if(searchDataInList(*airport->waitForRunwayQueue, plane))
        return;
    appendInList(airport->waitForRunwayQueue, plane);
    printf("\033[0;34m%s : Request for a takeoff runway.\033[0m\n", plane->matriculation);
}

void grantPlaneInAFRQAccessToRunway(airport* airport, runway *runway, plane *plane){
    deleteInList(airport->waitForRunwayQueue, plane);
    deleteInList(airport->parkingPlanes, plane);
    addPlaneToRunwayQueue(runway, plane);
}

void grantNextInAFRQAccessToRunway(airport* airport, runway *runway){
    for(int p = 0; p<airport->waitForRunwayQueue->length; p++){
        plane *plane = getDataAtIndex(*airport->waitForRunwayQueue, p);
        if(canItLandHere(plane, runway)){
            grantPlaneInAFRQAccessToRunway(airport, runway, plane);
            return;
        }
    }
}