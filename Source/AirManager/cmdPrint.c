#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cmdPrint.h"



void debugPrintPlane(plane plane) {
    printf("Informations of plane :\n");
    printf("Matriculation : %s\n", plane->matriculation);
    printf("Number of Passengers : %u\n", plane->passengers);
    printf("Number Max of Passengers : %u\n", plane->passengersMax);
}

void debugPlaneType(plane plane){
    switch(PlaneType) {
        case AIRLINER :
            printf("AIRLINER"); 
            break;
        // A finir
    }


}
