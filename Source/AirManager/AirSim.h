#include "AirManager.h"

typedef struct{
    unsigned int simulationSpeedInMs;
    airport *airport;
    list *planeActors;
} simulation;

typedef struct{
    plane *plane;
    int stateRemainTimeInMs; 
} sim_planeActor