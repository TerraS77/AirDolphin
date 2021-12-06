/**
 * @file AirSim.c
 * @brief Manage the simulation of planes dynamic behaviours
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "AirSim.h"
#include "Renderer.h"

int cmprPointer(void *data1, void *data2){
    if(data1 == data2) return 0;
    else return 1;
}

sim_planeActor *newSimPlaneActor(char matriculation[7], planeType type, unsigned int passengers, unsigned int passengersMax, planeStatus status){
    sim_planeActor *planeActor = malloc(sizeof(sim_planeActor));
    planeActor->plane = newPlane(matriculation, type, passengers, passengersMax, status);
    planeActor->stateRemainTimeInMs = -1;
    planeActor->stateLengthTimeInMs = -1;
    return planeActor;
}

sim_planeActor *getSimPlaneActorInList(plane *plane, simulation *simulation){
    for(int p = 0; p < simulation->planeActors->length; p++){
        sim_planeActor *planeActor = getDataAtIndex(*simulation->planeActors, p);
        if(planeActor->plane == plane) return planeActor; 
    }
    return NULL;
}

simulation initSimulation(int parkingSize, int numberOfSmallRunway, int numberOfMediumRunway, int numberOfLargeRunway, int numberOfPlanes){
    simulation newSim;
    newSim.airport = newAirport(parkingSize);
    newSim.planeActors = newList(cmprPointer);
    buildAirport(newSim.airport, numberOfSmallRunway, numberOfMediumRunway, numberOfLargeRunway);
    for(int planeNum = 1; planeNum<=numberOfPlanes; planeNum++){
        char str[7];
        sprintf(str, "FR-%d", planeNum*planeNum*3+1);
        sim_planeActor *planeActors;
        if(planeNum%2)
            planeActors = newSimPlaneActor(str, planeNum%3, 10 + (planeNum%3)*50, 10 + 50*(planeNum%3)*(planeNum%3), FLYING); 
        else
            planeActors = newSimPlaneActor(str, planeNum%3, 10 + (planeNum%3)*50, 10 + 50*(planeNum%3)*(planeNum%3), PARKING); 
        appendInList(newSim.planeActors, planeActors);
        planeActors->stateRemainTimeInMs = 30 + (planeNum%4)*500;
        planeActors->stateLengthTimeInMs = planeActors->stateRemainTimeInMs;
        loadPlaneInAirport(newSim.airport, planeActors->plane);
    }
    return newSim;
}

void planeNextAction(airport *airport, sim_planeActor *planeActor){
    switch (planeActor->plane->status)
    {
    case FLYING:
        addPlaneToLandingQueue(airport, planeActor->plane);
        planeActor->stateRemainTimeInMs = -1;
        planeActor->stateLengthTimeInMs = planeActor->stateRemainTimeInMs;
        break;
    case LANDING:
        planeExitRunway(planeActor->plane->targetRunway, planeActor->plane);
        addPlaneToParking(airport, planeActor->plane);
        planeActor->stateRemainTimeInMs = 5000*(planeActor->plane->type + 1); //TIME WAITING IN PARKING
        planeActor->stateLengthTimeInMs = planeActor->stateRemainTimeInMs;
        break;
    case PARKING:
        addPlaneToAFRQ(airport, planeActor->plane);
        planeActor->stateRemainTimeInMs = -1;
        planeActor->stateLengthTimeInMs = planeActor->stateRemainTimeInMs;
        break;
    case TAKEOFF:
        planeExitRunway(planeActor->plane->targetRunway, planeActor->plane);
        planeActor->stateRemainTimeInMs = 5000*(planeActor->plane->type + 1); //TIME FLYING BEFORE RETURN
        planeActor->stateLengthTimeInMs = planeActor->stateRemainTimeInMs;
        break;
    }
}

int msleep(unsigned int tms) {
  return usleep(tms * 1000);
}

int main(){
    simulation simulation = initSimulation(20,1,2,3,50);
    simulation.simulationSpeedInMs = 30;
    airport *airport = simulation.airport;
    initWindow(1850,900);
    while (1) {
        // Every runway resolve xor request resolve
        printf("└\n");
        for(int rw = 0; rw < airport->runways->length; rw++){
            runway *runway = getDataAtIndex(*airport->runways, rw);
            int TODO = 0; //0 Nothing, 1 getNextLandQ, 2 getNextTakeoffK
            if(isRunwayFree(runway)) {
                if(canAPlaneInLQLandHere(airport, runway)) TODO = 1;
                else if(runway->takeoffQueue->length) TODO = 2;
                switch (TODO) {
                    case 1:
                        grantNextInLQAccessToRunway(airport, runway);
                        break;
                    case 2:
                        grantTakeoffForRunway(runway);
                        break;
                }
                if(TODO){
                    getSimPlaneActorInList(runway->planeLT, &simulation)->stateRemainTimeInMs = runway->length/2;
                    getSimPlaneActorInList(runway->planeLT, &simulation)->stateLengthTimeInMs = runway->length/2;
                }
            }
            if(!isRunwayQueueFull(runway)) grantNextInAFRQAccessToRunway(airport, runway);
        }
        printf("┌\n");
        printf("└\n");
        // Planes get stateRemainTimeInMs decremented
        for(int p = 0; p < simulation.planeActors->length; p++){
            sim_planeActor *planeActor = getDataAtIndex(*simulation.planeActors, p);
            planeActor->stateRemainTimeInMs -= simulation.simulationSpeedInMs;
        }
        // stateRemainTimeInMs <= 0 state is updated 
        for(int p = 0; p < simulation.planeActors->length; p++) {
            sim_planeActor *planeActor = getDataAtIndex(*simulation.planeActors, p);
            if(planeActor->stateRemainTimeInMs <= 0)
                planeNextAction(airport, planeActor);
        }
        printf("┌\n");

        static int debugTimer = 5000;
        debugTimer -= simulation.simulationSpeedInMs;

        if(debugTimer <= 0)
        {
            debugPrintAirport(*simulation.airport);
            debugTimer = 5000;
        } 

        updateAirportRenderer(&simulation);
    }
}