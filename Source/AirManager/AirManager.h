
/**
 * @file AirManager.h
 * @brief Manage planes, runways and airports status and event based behaviours.
 * 
 */

#include <stdbool.h>
#include "SmartList.h"
#if !defined( AIRMANAGER_H )
#define AIRMANAGER_H

/**
 * @brief Enumeration of planes status
 * 
 */
typedef enum{
    FLYING,             /** Plane is in standby, flying */
    WAITING_LANDING,    /** Plane is flying and in landing queue */
    LANDING,            /** Plane is currently landing on a runway */
    PARKING,            /** Plane is currently at parking */
    WAITING_TAKEOFF,    /** Plane is in a runway queue */
    TAKEOFF             /** Plane is curretly taking off on a runway */
} planeStatus;


/**
 * @brief Enumeration of the planed types 
 * 
 */
typedef enum{AIRLINER, BUSINESS, LIGHT} planeType;

/**
 * @brief Airport containing all the planes and runways of the simulation
 */
typedef struct{
    unsigned int parkingSize;   /** Size of the parking */
    list *runways;              /** Runways of Airport */
    list *planesInRange;        /** All the planes in range of the control tower registration system*/
    list *parkingPlanes;        /** Planes in the parking */
    list *landingQueue;         /** All planes in the landing queue */
    list *waitForRunwayQueue;   /** Planes in parking waiting for a landing */
}airport;

/**
 * @brief Plane status and datas
 * 
 */
typedef struct{
    char matriculation[7];         /** Normalized matriculation of the planes */
    planeType type;                /** Class of plane */
    unsigned int passengers;       /** Number of passengers */
    unsigned int passengersMax;    /** Number of max passengers */
    planeStatus status;            /** Status of the plane */
    void *targetRunway;            /** Current runway of the plane, NULL if not on a runway */
}plane;

/**
 * @brief Enumeration of runways classes
 */
typedef enum{
    SMALL,      /** Small runway */
    MEDIUM,     /** Medium runway */
    LARGE       /** Large runway */
} runwayType;

/**
 * @brief Runway of an airport
 * 
 */
typedef struct{
    char id;                        /** Runway identifier */
    float length;                   /** Length of runway in meters */
    float width;                    /** Width of runway in meters */
    runwayType type;                /** Runway class */
    unsigned int maxTakeoffQueue;   /** Size of the take off queue */
    list *takeoffQueue;             /** List of planes in take off queue */
    plane *planeLT;                 /** Plane currently landing or taking off on the runway */
}runway;


/**
 * @brief Create and allocate memory for a plane.
 * 
 * @param[in] matriculation Matriculation of the plane
 * @param[in] type Class of the plane (AIRLINER, LIGHT or BUSINESS)
 * @param[in] passengers Number of passengers currently on board
 * @param[in] passengersMax Maximum number of passengers
 * @param[in] status Status of the plane (FLYING or PARKING only)
 * @return Pointer the created plane
 */
plane* newPlane(char matriculation[7], planeType type, unsigned int passengers, unsigned int passengersMax, planeStatus status);

/**
 * @brief Add a plane to a specified Airport
 * 
 * @param[in] airport Airport to load plane in 
 * @param[in] plane Plane to import
 */
void loadPlaneInAirport(airport* airport, plane *plane);

/**
 * @brief Remove a plane from Airport
 * 
 * @param[in] airport Airport from where the plane should be removed
 * @param[in] plane Plane to remove
 */
void removePlane(airport *airport, plane *plane);

/**
 * @brief Determine if a plane could land on the specified runway, based on his size and the class of the runway
 * 
 * @param[in] plane
 * @param[in] runway 
 * @return true if the plane could land on the runway
 * @return false if he can't
 */
bool canItLandHere(plane *plane, runway *runway);

/**
 * @brief Determine if a plane currently in Landing Queue can land on the specified runway.\n This function take into account the size of the plane, the class of the runway and if the parking is full or is going to be full.
 * 
 * @param[in] airport Airport containing the Landing Queue
 * @param[in] runway Runway to test
 * @return true if one of the planes in the landing queue can land on the runway
 * @return false if no plane in LQ can land on the runway
 */
bool canAPlaneInLQLandHere(airport *airport, runway *runway);

/**
 * @brief Create and allocate memory for a runway.
 * 
 * @param[in] length Length of runway in meters.
 * @param[in] width Width of runway in meters.
 * @param[in] type Class of runway (SMALL, MEDIUM, LARGE).
 * @param[in] maxTakeoffQueue Size of take off queue.
 * @return Pointer to the created plane. 
 */
runway* newRunway(float length, float width, runwayType type, unsigned int maxTakeoffQueue);

/**
 * @brief Add Plane to the runway, updating it status.\n Used to allow an airplane to land or takeoff on a runway.
 * 
 * @param[in] runway Runway where the plane should take place on.
 * @param[in] plane The plane.
 */
void addPlaneToRunway(runway *runway, plane *plane);

/**
 * @brief Remove plane from runway, updating it status.\n Used to finish the action started on runway, either landing or taking off.
 * 
 * @param[in] runway Runway from wich the plane exit.
 * @param[in] plane The plane.
 */
void planeExitRunway(runway *runway, plane *plane);

/**
 * @brief Determine if a runway is curently used by a plane or not.
 * 
 * @param[in] runway
 * @return true if the runway is empty
 * @return false if the runway is currently used
 */
bool isRunwayFree(runway* runway);

/**
 * @brief Allow next in runway queue for take off
 * 
 * @param[in] runway 
 */
void grantTakeoffForRunway(runway *runway);

/**
 * @brief Add plane in the take off queue for a specified runway
 * 
 * @param[in] runway 
 * @param[in] plane 
 */
void addPlaneToRunwayQueue(runway *runway, plane *plane);

/**
 * @brief Determine if the take off queue of a runway is full.
 * 
 * @param[in] runway 
 * @return true if the take off queue is full
 * @return false if the take off queue is not full
 */
bool isRunwayQueueFull(runway *runway);

/**
 * @brief Create an aiport and allocate adequate memory
 * 
 * @param[in] parkingSize Size of the parking
 * @return airport pointer to the new airport
 */
airport* newAirport(unsigned int parkingSize);

/**
 * @brief Build the runways for an airport
 * 
 * @param[in] airport 
 * @param[in] numberOfSmallRunway 
 * @param[in] numberOfMediumRunway 
 * @param[in] numberOfLargeRunway 
 */
void buildAirport(airport* airport, int numberOfSmallRunway, int numberOfMediumRunway, int numberOfLargeRunway);

/**
 * @brief Add plane to Parking
 * 
 * @param[in] airport 
 * @param[in] plane 
 */
void addPlaneToParking(airport* airport, plane *plane);

/**
 * @brief Determine if the parking is full
 * 
 * @param airport containg the parking
 * @return true if the parking is full
 * @return false if the parking is not full
 */
bool isParkingFull(airport* airport);

/**
 * @brief Determine if the parking is full or if the planes currently landing will fill the parking.
 * 
 * @param airport 
 * @return true if the parking is or will be full
 * @return false if it isn't
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
 * @brief Grant next plane in landing queue acess to runway
 * 
 * @param airport 
 * @param runway 
 */
void grantNextInLQAccessToRunway(airport* airport, runway *runway);

/**
 * @brief Add plane to the queue for acces to runway
 * 
 * @param airport 
 * @param plane 
 */
void addPlaneToAFRQ(airport* airport, plane *plane);

/**
 * @brief Grant the next plane waiting for acces to runway access to runway
 * 
 * @param airport 
 * @param runway 
 */
void grantNextInAFRQAccessToRunway(airport* airport, runway *runway);

#endif