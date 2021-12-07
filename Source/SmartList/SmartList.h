/**
 * @file SmartList.h
 * @brief Manage Lists
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include <stdio.h>
#include <stdlib.h>

#ifndef SMART_UI
#define SMART_UI



typedef void (*voidFunction)(void);
typedef int (*compareTwoPointersFunction)(void *data1, void *data2);
typedef void (*voidOnePointersFunction)(void *data);
typedef struct chainItem chainItem;

/**
 * @brief Item container of list 
 * 
 */
struct chainItem{
    /**Pointer to stored data*/
    void *data; 
    /**Index of item*/
    int index;
    /**Previous item in list, NULL if none*/
    chainItem *previous;
    /**Next item in list, NULL if none */
    chainItem *next; 
};

/**
 * @brief List object
 * 
 */
typedef struct{
    /** Length of list*/
    int length;                             
    /** First item of the list*/
    chainItem *first;                       
    /** Last item of the list*/
    chainItem *last;                        
    /** Function to compare data on the list*/
    compareTwoPointersFunction comparator;  
} list;

/**
 * @brief Allow to create a new list
 * 
 * @param comparatorFunction 
 * @return list
 */
list *newList(compareTwoPointersFunction comparatorFunction);

/**
 * @brief Get the Data at index in list
 * 
 * @param list 
 * @param index 
 * @return pointer to data
 */

void *getDataAtIndex(list list, int index);

/**
 * @brief Get the Item At Index object
 * 
 * @param list 
 * @param index 
 * @return chainItem* 
 */

chainItem *getItemAtIndex(list list, int index);

/**
 * @brief Delete the Item at Index object
 * 
 * @param list 
 * @param index 
 */

void deleteItemAtIndex(list *list, int index);

/**
 * @brief Delete the item in the List
 * 
 * @param list 
 * @param data 
 */

void deleteInList(list *list, void *data);

/**
 * @brief Add the item at the end of the list
 * 
 * @param list 
 * @param data 
 */

void pushInList(list *list, void *data);

/**
 * @brief Add the item at the beginning of the list
 * 
 * @param list 
 * @param data 
 */

void appendInList(list *list, void *data);

/**
 * @brief Add the item at the emplacement of the index in the list
 * 
 * @param list 
 * @param data 
 * @param index 
 */

void appendAtInList(list *list, void *data, int index); 

/**
 * @brief Search the Index in the list
 * 
 * @param list 
 * @param data 
 * @return int 
 */

int searchIndexInList(list list, void *data);

/**
 * @brief Search the data in the List
 * 
 * @param list 
 * @param data 
 * @return void* 
 */
void *searchDataInList(list list, void *data);

/**
 * @brief Delete All Items in the list
 * 
 * @param list 
 */

void emptyList(list *list);
#endif