#include "../SmartList/SmartList.h"

typedef struct runway runway;

typedef enum{FLYING, WAITING_LANDING, LANDING, PARKING, WAITING_TAKEOFF, TAKEOFF} planeStatus;
typedef enum{AIRLINER, BUSINESS, LIGHT} planeType;

typedef struct{
    char matriculation[7];
    planeType type;
    unsigned int passengers;
    planeStatus status;
    runway *targetRunway;
}plane;

typedef enum{STANDBY, LANDING, TAKEOFF} runwayStatus;
typedef enum{SMALL, MEDIUM, LARGE} runwayType;

typedef struct{
    char id;
    float length;
    float width;
    runwayType type;
    unsigned int maxTakeofQueue;
    list *takeofQueue;
    plane *planeLT;
}runway;

typedef struct{
    unsigned int parkingSize;
    list *parkingPlanes;
    list *runway;
    list *planesInRange;
}airport;

plane* newPlane(char matriculation[7], planeType type, unsigned int passengers, planeStatus status);
runway* newRunway(char id, float length, float width, runwayType type, unsigned int maxTakeofQueue);
void addPlaneToRunway(runway *runway, plane *plane);
airport* newAirport(unsigned int parkingSize);
void buildAirport(airport* airport, int numberOfSmallRunway, int numberOfMediumRunway, int numberOfLargeRunway);
