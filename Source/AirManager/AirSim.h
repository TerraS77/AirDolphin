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
    /** Simulation speed in milleseconds*/
    unsigned int simulationSpeedInMs;
    /** Airport of the simulation*/
    airport *airport;
    /** All of planes actors*/
    list *planeActors;
} simulation;


/**
 * @brief Sstructure of plane Actor
 */

typedef struct{
    /** Plane of planeActor*/
    plane *plane;
    /** State remaining time for the current task in milleseconds*/
    int stateRemainTimeInMs;
    /** State remaining length time for the current task in milleseconds*/
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

/**
 * @fn int cmprPointer(void *data1, void *data2)
 * @brief Comparaison of pointers
 * 
 * @param data1 
 * @param data2 
 * @return int 
 */

/**
 * @fn sim_planeActor *newSimPlaneActor(char matriculation[7], planeType type, unsigned int passengers, unsigned int passengersMax, planeStatus status)
 * @brief Create a new plane for the simulation
 * 
 * @param matriculation 
 * @param type 
 * @param passengers 
 * @param passengersMax 
 * @param status 
 * @return sim_planeActor* 
 */

/**
 * @fn sim_planeActor *getSimPlaneActorInList(plane *plane, simulation *simulation
 * @brief Get the Sim Plane Actor In List object
 * 
 * @param plane 
 * @param simulation 
 * @return sim_planeActor* 
 */

/**
 * @fn simulation initSimulation(int parkingSize, int numberOfSmallRunway, int numberOfMediumRunway, int numberOfLargeRunway, int numberOfPlanes)
 * @brief Initialise the simulation
 * 
 * @param parkingSize 
 * @param numberOfSmallRunway 
 * @param numberOfMediumRunway 
 * @param numberOfLargeRunway 
 * @param numberOfPlanes 
 * @return simulation 
 */

/**
 * @fn void planeNextAction(airport *airport, sim_planeActor *planeActor)
 * @brief Give action to the next plane
 * 
 * @param airport 
 * @param planeActor 
 */

/**
 * @fn int msleep(unsigned int tms) 
 * @brief Function for time
 * 
 * @param tms 
 * @return int 
 */

/**
 * @fn int main()
 * @brief Function main
 * 
 * @return int 
 */