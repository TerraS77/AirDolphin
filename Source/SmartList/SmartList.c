/**
 * @file SmartList.c
 * @brief Functions for the management of lists
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include "SmartList.h"

/**
 * @brief init chain item
 * 
 * @param[in] data pointer to data to stock in item
 * @return chainItem pointer to allocated memory for intialized chain item 
 */
chainItem *newChainItem(void *data){
    chainItem *newItem = malloc(sizeof(chainItem));
    newItem->data = data;
    newItem->index = 0;
    newItem->previous = NULL;
    newItem->next = NULL;
    return newItem;
}

list *newList(compareTwoPointersFunction comparatorFunction){
    list *theNewList = malloc(sizeof(list));
    theNewList->first = NULL;
    theNewList->last = NULL;
    theNewList->length = 0;
    theNewList->comparator = comparatorFunction;
    return theNewList;
}

chainItem *getItemAtIndex(list list, int index){
    if(index < 0 || index > list.length - 1) return NULL;
    chainItem *target = list.first;
    int n = 0;
    while(target != NULL && n < index){
        target = target->next;
        n++;
    }
    return target;
}

/**
 * @brief Update indexes of a list
 * 
 * @param[in] list list to update
 */
void updateIndexs(list *list){
    chainItem *item = list->first;
    int index = 0;
    while(item != NULL){
        item->index = index;
        index++;
        item = item->next;
    }
    list->last = getItemAtIndex(*list, list->length-1);
}

void *getDataAtIndex(list list, int index){
    if(index < 0){
        printf("\033[1;31mSmartList_ERROR : Get Data at negative index\n\033[0m");
        exit(1);
    }
    return getItemAtIndex(list, index)->data;
}

/**
 * @brief Put an item at specified index and update the list architecture to support it
 * 
 * @param[in] list 
 * @param[in] item 
 * @param[in] index 
 */
void putItemAtIndex(list *list, chainItem *item, int index){
    if(index < 0){
        printf("\033[1;31mSmartList_ERROR : Put Item at negative index\n\033[0m");
        exit(1);
    }
    if(item == NULL || list->length < index) return;
    item->previous = getItemAtIndex(*list, index-1);
    item->next = getItemAtIndex(*list, index);
    item->index = index;

    if(item->previous != NULL)
        item->previous->next = item;
    if(item->next != NULL)
        item->next->previous = item;

    if(index == 0) list->first = item;
    if(index == list->length-1) list->last = item;
    list->length++;
    updateIndexs(list);
}

void deleteItemAtIndex(list *list, int index){
    if(index < 0){
        printf("\033[1;31mSmartList_ERROR : Delete Item at negative index (%d)\n\033[0m", index);
        exit(1);
    }
    if(list->length < index) return;
    chainItem *item = getItemAtIndex(*list, index);
    if(item->previous)
        item->previous->next = item->next;
    if(item->next)
        item->next->previous = item->previous;
    if(index == 0) list->first = item->next;
    if(index == list->length-1) list->last = item->previous;
    list->length--;
    updateIndexs(list);
    free(item);
}

void pushInList(list *list, void *data){
    chainItem *newItem = newChainItem(data);
    putItemAtIndex(list, newItem, 0);
}

void appendInList(list *list, void *data){
    chainItem *newItem = newChainItem(data);
    putItemAtIndex(list, newItem, list->length);
}

void appendAtInList(list *list, void *data, int index){
    chainItem *newItem = newChainItem(data);
    putItemAtIndex(list, newItem, index);
}

chainItem *searchItemInList(list list, void *data){
    chainItem *item = list.first; 
    while(item != NULL){
        if(list.comparator(item->data, data) == 0) return item;
        item = item->next;
    }
    return NULL;
}

int searchIndexInList(list list, void *data){
    chainItem *result = searchItemInList(list, data);
    if(result == NULL) return -404;
    return result->index;
}

void *searchDataInList(list list, void *data){
    chainItem *result = searchItemInList(list, data);
    if(result == NULL) return NULL;
    return searchItemInList(list, data)->data;
}

void deleteInList(list *list, void *data){
    int index = searchIndexInList(*list, data);
    if(index != -404) deleteItemAtIndex(list, index);
    else printf("\033[1;31mSmartList_ERROR : Item to Delete not found (%p)\n\033[0m", data);
}

void emptyList(list *list){
    while(list->length > 0) deleteItemAtIndex(list, 0);
}

// void saveChainFileAs(chainItem *list, char *fileName){
//     FILE* file = NULL;
//     file = fopen(fileName, "w+");
//     fputc('[', file);
//     chainItem *item = list;
//     while(item != NULL){
//         if(item != list) fputc(',', file);
//         fprintf(file, "{id:%d,name:\"%s\",mass:\"%.2f\"}", item->data.id, item->data.name, item->data.mass);
//         item = item->next;
//     }
//     fputc(']', file);
//     fclose(file);
//     printf("%s a été créé avec succès.", fileName);
//     charCachePurge();charCachePurge();
// }

// chainData sMakeChainData(char buffer[100]){
//     chainData newData = newChainData();
//     char masssBufffer[10];
//     sscanf(buffer, "id:%d,name:\"%[^\"]\",mass:\"%[^\"]", &newData.id, newData.name, masssBufffer);
//     printf("%s\n", masssBufffer);
//     sscanf(masssBufffer, "%f", &newData.mass);
//     return newData;
// }

// chainItem *openChainFile(chainItem *list, char *fileName){
//     FILE* file = NULL;
//     file = fopen(fileName, "r");
//     if(file == NULL){
//         printf("Echec de l'ouverture de %s.", fileName);
//         charCachePurge();charCachePurge();
//         return list;
//     }
//     char cursor = fgetc(file);
//     while(cursor != ']'){
//         if(cursor ==  '{'){
//             char buffer[100];
//             long startPin = ftell(file);
//             long endPin = startPin;
//             while(cursor != '}') {
//                 cursor = fgetc(file);
//                 endPin++;
//             }
//             fseek(file, startPin, SEEK_SET);
//             fgets(buffer, endPin - startPin, file);
//             chainPush(&list, sMakeChainData(buffer));
//         }
//         if(cursor == ',' || cursor == '[' || cursor == '}') cursor = fgetc(file);
//     }
//     fclose(file);
//     return list;
// }
