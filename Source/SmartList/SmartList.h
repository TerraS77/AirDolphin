#include <stdio.h>
#include <stdlib.h>

#ifndef SMART_UI
#define SMART_UI

typedef void (*voidFunction)(void);
typedef int (*compareTwoPointersFunction)(void *data1, void *data2);
typedef void (*voidOnePointersFunction)(void *data);
typedef struct chainItem chainItem;

struct chainItem{
    void *data;
    int index;
    chainItem *previous;
    chainItem *next;
};

typedef struct{
    int length;
    chainItem *first;
    chainItem *last;
    compareTwoPointersFunction comparator;
} list;

list *newList(compareTwoPointersFunction comparatorFunction);
void *getDataAtIndex(list list, int index);
chainItem *getItemAtIndex(list list, int index);

void deleteItemAtIndex(list *list, int index);
void deleteInList(list *list, void *data);

void pushInList(list *list, void *data);
void appendInList(list *list, void *data);
void appendAtInList(list *list, void *data, int index); 

int searchIndexInList(list list, void *data);
void *searchDataInList(list list, void *data);

void emptyList(list *list);
#endif

//Return 0 if equal, a positive int if data1 > data2, a negative int if data2 < data1;