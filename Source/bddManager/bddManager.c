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



plane* sMakeChainData(char buffer[100]){
    int type,status;
    plane* newData = newChainData();
    // char masssBufffer[10];
    sscanf(buffer, "%[^ ]- %[^ ]| %d/%d | %s", newData->id, type,newData->passengers,newData->passengersMax,status);
// inverse que la save, cad que %s->%d
 switch((type)) {
            case strcmp(AIRLINER,type) :
                strcpy(type,"AIRLINER");
                break;
            case strcmp(BUSINESS,type) :
                strcpy(type,"BUSINESS");
                break;
            case strcmp(LIGHT,type) :
                strcpy(type,"LIGHT");
                break;
        }
        switch((planes->status)) {
            case strcmp(FLYING,status) :
                strcpy(status,"FLYING");
                break;
            default:
                strcpy(status,"PARKING");
                break;
        }

    return newData;


}

// recup type et stat need to put in string and compare value to enter right type and shit 
// need to trabsforme dat in right flyin EG flyung 0 (( STRCMP ))
// CAD comme flying est un int en string  faut faire strcmp et si flying == flying alors statu == flying )


chainItem *openChainFile(chainItem *list, char *fileName){
    FILE* file = NULL;
    file = fopen(fileName, "r");
    if(file == NULL){
        printf("Echec de l'ouverture de %s.", fileName);
        charCachePurge();charCachePurge();
        return list;
    }
    char cursor = fgetc(file);
    while(cursor != ']'){
        if(cursor ==  '{'){
            char buffer[100];
            long startPin = ftell(file);
            long endPin = startPin;
            while(cursor != '}') {
                cursor = fgetc(file);
                endPin++;
            }
            fseek(file, startPin, SEEK_SET);
            fgets(buffer, endPin - startPin, file);
            appendInList(&list, sMakeChainData(buffer));
        }
        if(cursor == ',' || cursor == '[' || cursor == '}') cursor = fgetc(file);
    }
    fclose(file);
    return list;
}

