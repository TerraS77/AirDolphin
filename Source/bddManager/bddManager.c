#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../AirManager/AirManager.h"


void creationBdd();



void savePlaneInFile(airport *airport) {
    chainItem* PlaneContainer = airport->planeInRange->first;
	FILE* bdd = fopen("planes.bdd","w");

	while(PlaneContainer != NULL) {
        plane *planes = PlaneContainer.data;
        char type[10];
        char status[17];
        switch((planes->type)) {
            case AIRLINER :
                strcpy(type,"AIRLINER");
                break;
            case BUSINESS :
                strcpy(type,"BUSINESS");
                break;
            case LIGHT :
                strcpy(type,"LIGHT");
                break;
        }
        switch((planes->status)) {
            case FLYING :
                strcpy(status,"FLYING");
                break;
            default:
                strcpy(status,"PARKING");
                break;
        }
		fprintf(bdd,"%s - %s | %d/%d | %s\n", planes->matriculation, type, planes->passengers, planes->passengersMax, status);
		PlaneContainer = PlaneContainer->next;
	}
	fclose(bdd);
	
	
}

