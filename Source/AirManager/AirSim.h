/**
 * @file AirSim.h
 * @brief Header for the simulation
 * @copyright Copyright (c) 2021
 * 
 */


#include "AirManager.h"
#include "cmdPrint.h"
#if !defined( AIRSIM_H )
#define AIRSIM_H

/**
 * @brief Structure of simulation
 * 
 */

typedef struct{
    unsigned int simulationSpeedInMs;
    airport *airport;
    list *planeActors;
} simulation;


/**
 * @brief Sstructure of plane Actor
 */

typedef struct{
    plane *plane;
    int stateRemainTimeInMs; 
    int stateLengthTimeInMs;
} sim_planeActor;

/**
 * @brief Get the Sim Plane Actor In List object
 * @param plane 
 * @param simulation 
 * @return sim_planeActor* 
 */
sim_planeActor *getSimPlaneActorInList(plane *plane, simulation *simulation);
#endif