#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "AirSim.h"


int cmprPointer(void *data1, void *data2){
    if(data1 == data2) return 0;
    else return 1;
}

sim_planeActor *newSimPlaneActor(char matriculation[7], planeType type, unsigned int passengers, unsigned int passengersMax, planeStatus status){
    sim_planeActor *planeActor = malloc(sizeof(sim_planeActor));
    planeActor->plane = newPlane(matriculation, type, passengers, passengersMax, status);
    planeActor->stateRemainTimeInMs = -1;
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
        loadPlainInAirport(newSim.airport, planeActors->plane);
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
        planeExitRunway(planeActor->plane->targetRunway, planeActor->plane);
        addPlaneToParking(airport, planeActor->plane);
        planeActor->stateRemainTimeInMs = 5000*(planeActor->plane->type + 1); //TIME WAITING IN PARKING
        break;
    case PARKING:
        addPlaneToAFRQ(airport, planeActor->plane);
        planeActor->stateRemainTimeInMs = -1;
        break;
    case TAKEOFF:
        planeExitRunway(planeActor->plane->targetRunway, planeActor->plane);
        planeActor->stateRemainTimeInMs = 50000; //TIME FLYING BEFORE RETURN
        break;
    }
}


int msleep(unsigned int tms) {
  return usleep(tms * 1000);
}

void unitaryTestCycle(airport *airport, plane *plane){

}


void testCycle(){
    airport *airport = newAirport(5);
    buildAirport(airport, 1,1,1);
    unitaryTestCycle(airport, newPlane("T_LIGHT", LIGHT, 50, 50, FLYING));
    unitaryTestCycle(airport, newPlane("T_VIP", BUSINESS, 50, 50, FLYING));
    unitaryTestCycle(airport, newPlane("T_AIRL", AIRLINER, 50, 50, FLYING));
}

void debugPlaneStatus(plane plane)
{
    switch (plane.status)
    {
    case FLYING:
        printf("FLYING");
        break;
    case WAITING_LANDING:
        printf("WAITING_LANDING");
        break;
    case LANDING:
        printf("LANDING");
        break;
    case PARKING:
        printf("PARKING");
        break;
    case WAITING_TAKEOFF:
        printf("WAITING_TAKEOFF");
        break;
    case TAKEOFF:
        printf("TAKEOFF");
        break;
    }
}

int main(){
    simulation simulation = initSimulation(10,2,3,2,5);
    simulation.simulationSpeedInMs = 500;
    // testCycle();
    airport *airport = simulation.airport;
    //Main Cycle
    while (1) {
        printf("================= NewTurn ================\n");
        // Every runway resolve xor request resolve
        for(int rw = 0; rw < airport->runways->length; rw++){
            runway *runway = getDataAtIndex(*airport->runways, rw);
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
                        grantTakeoffForRunway(runway);
                        break;
                }
                if(TODO)
                    getSimPlaneActorInList(runway->planeLT, &simulation)->stateRemainTimeInMs = runway->length/2;
            }
            if(!isRunwayQueueFull(runway))
                grantNextInAFRQAccessToRunway(airport, runway);
        }
        // Planes get stateRemainTimeInMs decremented
        runway *run0 = getDataAtIndex(*airport->runways, 0);
        for(int p = 0; p < simulation.planeActors->length; p++){
            sim_planeActor *planeActor = getDataAtIndex(*simulation.planeActors, p);
            planeActor->stateRemainTimeInMs -= simulation.simulationSpeedInMs;
            debugPlaneStatus(*planeActor->plane);
            printf(" | %s : %d\n", planeActor->plane->matriculation, planeActor->stateRemainTimeInMs);
        }
        // stateRemainTimeInMs <= 0 state is updated 
        for(int p = 0; p < simulation.planeActors->length; p++) {
            sim_planeActor *planeActor = getDataAtIndex(*simulation.planeActors, p);
            if(planeActor->stateRemainTimeInMs <= 0)
                planeNextAction(airport, planeActor);
        }
        msleep(simulation.simulationSpeedInMs);
    }
}