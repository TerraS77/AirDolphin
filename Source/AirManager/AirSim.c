#include <stdio.h>
#include <stdlib.h>
#include "AirSim.h"

sim_planeActor *newSimPlaneActor(char matriculation[7], planeType type, unsigned int passengers, unsigned int passengersMax, planeStatus status){
    sim_planeActor *planeActor = malloc(sizeof(sim_planeActor));
    planeActor->plane = newPlane(matriculation, type, passengers, passengersMax, status);
    planeActor->stateRemainTimeInMs = -1;
    return planeActor;
}

sim_planeActor *getSimPlaneActorInList(plane *plane, simulation *simulation){
    for(int p = 0; p < simulation->planeActors->length; p++){
        sim_planeActor *planeActor = getDataAtIndex(simulation->planeActors, p);
        if(planeActor->plane == plane) return planeActor; 
    }
    return NULL;
}

simulation initSimulation(int parkingSize, int numberOfSmallRunway, int numberOfMediumRunway, int numberOfLargeRunway, int numberOfPlanes){
    simulation newSim;
    newSim.airport = newAirport(parkingSize);
    buildAirport(newSim.airport, numberOfSmallRunway, numberOfMediumRunway, numberOfLargeRunway);
    for(int planeNum = 1; planeNum<=numberOfPlanes; planeNum++){
        char str[7];
        sprintf(str, "FR-%d", planeNum*planeNum*3+1);
        sim_planeActor *planeActors;
        if(planeNum%2)
            planeActors = newSimPlaneActor(str, planeNum%3, 10 + (planeNum%3)*50, 10 + 50*(planeNum%3)*(planeNum%3), FLYING); 
        else {
            planeActors = newSimPlaneActor(str, planeNum%3, 10 + (planeNum%3)*50, 10 + 50*(planeNum%3)*(planeNum%3), PARKING); 
            appendInList(newSim.airport->parkingPlanes, planeActors);
        }
        appendInList(newSim.planeActors, planeActors);
        appendInList(newSim.airport->planesInRange, planeActors->plane);
        planeActors->stateRemainTimeInMs = 30 + (planeNum%4)*500;
    }
    return newSim;
}

void planeNextAction(airport *airport, sim_planeActor *planeActor){
    switch (planeActor->plane->status)
    {
    case FLYING:
        addPlaneToLandingQueue(airport, planeActor->plane);
        planeActor->stateRemainTimeInMs = -1;
        break;
    case LANDING:
        planeExitRunway(airport, planeActor->plane);
        planeActor->stateRemainTimeInMs = 5000*(planeActor->plane->type + 1); //TIME WAITING IN PARKING
        break;
    case PARKING:
        addPlaneToAFRQ(airport, planeActor->plane);
        planeActor->stateRemainTimeInMs = -1;
        break;
    case TAKEOFF:
        planeExitRunway(airport, planeActor->plane);
        planeActor->stateRemainTimeInMs = 50000; //TIME FLYING BEFORE RETURN
        break;
    }
}

int main(){
    simulation simulation = initSimulation(10,2,3,2,4);
    airport *airport = simulation.airport;
    //Cycle
    while (1) {
        // Every runway resolve xor request resolve
        for(int rw = 0; rw <= airport->runways->length; rw++){
            runway *runway = getDataAtIndex(airport->runways, rw);
            int TODO = 0; //0 Nothing, 1 getNextLandQ, 2 getNextTakeoffK
            if(isRunwayFree(runway)) {
                if(runway->takeoffQueue->length) {
                    if(canAPlaneInLQLandHere(airport, runway)) TODO = 2; //!NEED CHOICE
                    else TODO = 2;
                } else if(canAPlaneInLQLandHere(airport, runway)) TODO = 1;
                switch (TODO) {
                    case 1:
                        grantNextInLQAccessToRunway(airport, runway);
                        break;
                    case 2:
                        grantTakeoffForRunway(airport, runway);
                        break;
                }
                if(TODO)
                    getSimPlaneActorInList(simulation, runway->planeLT)->stateRemainTimeInMs = 500*runway->length;
            }
        }
        
        // Planes get stateRemainTimeInMs decremented
        for(int p = 0; p < simulation->planeActors->length; p++){
            sim_planeActor *planeActor = getDataAtIndex(simulation->planeActors, p);
            planeActor->stateRemainTimeInMs -= simulation.simulationSpeedInMs; 
        }

        // stateRemainTimeInMs <= 0 state is updated 
        for(int p = 0; p < simulation->planeActors->length; p++){
            sim_planeActor *planeActor = getDataAtIndex(simulation->planeActors, p);
            if(planeActor->stateRemainTimeInMs <= 0)
                planeNextAction(airport, planeActor);
        }
    }
}