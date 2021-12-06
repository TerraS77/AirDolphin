/**
 * @file bddManager.h
 *
 * @brief groups the function declarations of bddManager.c 
 * 
 */
#include "AirManager.h"

#if !defined( BDDMANAGER_H )
#define BDDMANAGER_H
#include "AirSim.h"

/**
 * @brief Allows an automatic generation of matriculation
 *   
 * 
 * @return char* 
 */
char * randomRegistration ();
/**
 * @brief a customised randomInt used in randomRegistration 
 * 
 * @param min 
 * @param max 
 * @return int 
 */
int randomInt (int min,int max); 
/**
 * @brief function that allows to save the airport in a file for later use
 * 
 * @param airport 
 */
void savePlanesInFile(airport *airport);
/**
 * 
 * 
 * @brief function that allows to load the DB
 * @brief it takes a .txt file and cuts each line, sends it to sMakeChainData which will process it and put each plane in the right place in the right list while putting the right pointers 
 * 
 * @param fileName 
 * @param simulation 
 */
void openChainFile(char *fileName,simulation* simulation);

#endif
/**
 * @fn int CmpPtr(void *data1, void *data2)
 * @brief compares two function pointers and return 0 or 1
 * 
 * @param data1 
 * @param data2 
 * @return int 
 */
/**
 * @fn plane *sMakeChainData(char buffer[100])
 * @brief retrieves the buffer and puts the info contained in it into a plane which is then returned 
 * 
 * @param buffer 
 * @return plane* 
 */
