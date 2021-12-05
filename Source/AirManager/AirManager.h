
/**
 * @file AirManager.h
 * @brief Manager 
 * @version 0.1
 * @date 2021-12-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include <stdbool.h>
#include "SmartList.h"
#if !defined( AIRMANAGER_H )
#define AIRMANAGER_H

/**
 * @brief enumeration of the status for the plane
 * 
 */
typedef enum{FLYING, WAITING_LANDING, LANDING, PARKING, WAITING_TAKEOFF, TAKEOFF} planeStatus;


/**
 * @brief enumeration of planeType
 * 
 */
typedef enum{AIRLINER, BUSINESS, LIGHT} planeType;

/**
 * @brief structure of airport
 * 
 */

typedef struct{
    unsigned int parkingSize;
    list *runways;
    list *planesInRange;
    list *parkingPlanes;
    list *landingQueue;
    list *waitForRunwayQueue;
}airport;

/**
 * @brief structure of plane
 * 
 */

typedef struct{
    char matriculation[7];
    planeType type;
    unsigned int passengers;
    unsigned int passengersMax;
    planeStatus status;
    void *targetRunway; //runway
}plane;

/**
 * @brief enumeration of the status for the plane
 * 
 */
typedef enum{SMALL, MEDIUM, LARGE} runwayType;

typedef struct{
    char id;
    float length;
    float width;
    runwayType type;
    unsigned int maxTakeoffQueue;
    list *takeoffQueue;
    plane *planeLT;
}runway;


/**
 * @brief Create a plane
 * 
 * @param matriculation 
 * @param type 
 * @param passengers 
 * @param passengersMax 
 * @param status 
 * @return plane* 
 */
plane* newPlane(char matriculation[7], planeType type, unsigned int passengers, unsigned int passengersMax, planeStatus status);

/**
 * @brief Load plane in the airport
 * 
 * @param airport 
 * @param plane 
 */
void loadPlainInAirport(airport* airport, plane *plane);

/**
 * @brief Remove the plane
 * 
 * @param airport 
 * @param plane 
 */
void removePlane(airport *airport, plane *plane);

/**
 * @brief Ask if it can land ?
 * 
 * @param plane 
 * @param runway 
 * @return true 
 * @return false 
 */

bool canItLandHere(plane *plane, runway *runway);

/**
 * @brief Yes or No for the landing question
 * 
 * @param airport 
 * @param runway 
 * @return true 
 * @return false 
 */

bool canAPlaneInLQLandHere(airport *airport, runway *runway);

/**
 * @brief Create a runway
 * 
 * @param length 
 * @param width 
 * @param type 
 * @param maxTakeoffQueue 
 * @return runway* 
 */
runway* newRunway(float length, float width, runwayType type, unsigned int maxTakeoffQueue);

/**
 * @brief Add Plane to the runway
 * 
 * @param runway 
 * @param plane 
 */


void addPlaneToRunway(runway *runway, plane *plane);

/**
 * @brief Plane exit the runway
 * 
 * @param runway 
 * @param plane 
 */

void planeExitRunway(runway *runway, plane *plane);

/**
 * @brief Is the runway free ?
 * 
 * @param newRunway 
 * @return true 
 * @return false 
 */

bool isRunwayFree(runway* newRunway);

/**
 * @brief Grant Take off for runway
 * 
 * @param runway 
 */
void grantTakeoffForRunway(runway *runway);

/**
 * @brief Add plane in the take off queue
 * 
 * @param runway 
 * @param plane 
 */

void addPlaneToRunwayQueue(runway *runway, plane *plane);

/**
 * @brief Is the runway queue full ?
 * 
 * @param runway 
 * @return true 
 * @return false 
 */

bool isRunwayQueueFull(runway *runway);

/**
 * @brief Know the parking Size
 * 
 * @param parkingSize 
 * @return airport* 
 */

airport* newAirport(unsigned int parkingSize);

/**
 * @brief Create an airport
 * 
 * @param airport 
 * @param numberOfSmallRunway 
 * @param numberOfMediumRunway 
 * @param numberOfLargeRunway 
 */

void buildAirport(airport* airport, int numberOfSmallRunway, int numberOfMediumRunway, int numberOfLargeRunway);

/**
 * @brief Add plane to Parking
 * 
 * @param airport 
 * @param plane 
 */
void addPlaneToParking(airport* airport, plane *plane);

/**
 * @brief Is the parking full ?
 * 
 * @param airport 
 * @return true 
 * @return false 
 */
bool isParkingFull(airport* airport);

/**
 * @brief Is the queue for parking full ?
 * 
 * @param airport 
 * @return true 
 * @return false 
 */
bool isParkingQueueFull(airport* airport);

/**
 * @brief Add plane to landing queue
 * 
 * @param airport 
 * @param plane 
 */
void addPlaneToLandingQueue(airport* airport, plane *plane);

/**
 * @brief grant INLQ acess to runway
 * 
 * @param airport 
 * @param runway 
 */

void grantNextInLQAccessToRunway(airport* airport, runway *runway);
/**
 * @brief Add plane for AFRQ
 * 
 * @param airport 
 * @param plane 
 */

void addPlaneToAFRQ(airport* airport, plane *plane);

/**
 * @brief Grant the next AFRQ Access to runway
 * 
 * @param airport 
 * @param runway 
 */

void grantNextInAFRQAccessToRunway(airport* airport, runway *runway);

#endif