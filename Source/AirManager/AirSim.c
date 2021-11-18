#include "AirSim.h"
#include <stdio.h>
#include <stdlib.h>

sim_planeActor *newSimPlaneActor(char matriculation[7], planeType type, unsigned int passengers, unsigned int passengersMax, planeStatus status){
    sim_planeActor *planeActor = malloc(sizeof(sim_planeActor));
    planeActor->plane = newPlane(matriculation, type, passengers, passengersMax, status);
    planeActor->stateRemainTimeInMs = -1;
    return planeActor;
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

int main(){
    simulation simulation = initSimulation(10,2,3,2,4);
    airport *airport = simulation.airport;
    //Cycle
    while (1)
    {
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
            }
        }
        // Planes get stateRemainTimeInMs decremented
        // stateRemainTimeInMs <= 0 state is updated 
    }
}