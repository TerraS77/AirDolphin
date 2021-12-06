#include "AirManager.h"

#if !defined( BDDMANAGER_H )
#define BDDMANAGER_H
#include "AirSim.h"

char * randomRegistration ();
int randomInt (int min,int max); 
void savePlanesInFile(airport *airport);
void openChainFile(char *fileName,simulation* simulation);

#endif

