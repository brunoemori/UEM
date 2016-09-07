#ifndef GRAPHFUNCTIONS_H
#define GRAPHFUNCTIONS_H
#include "adjlist.h"

typedef struct searchStruct {
  int colorVertex, ancestorVertex, timeDiscoveryVertex, timeFinalVertex;
} searchStruct;

void printSearch(searchStruct *xfs, int nVertex);

void deepFirstSearch(adjListBlock *adjList[], int nVertex);

void dfsVisit(adjListBlock *adjList[], int vertex);

void breadthFirstSearch(adjListBlock *adjList[], int nVertex, int startVertex);

void printComponents(int cVertex[], int nVertex);

void connectedComponentVisit(adjListBlock *adjList[], int vertex, int k);

void connectedComponent(adjListBlock *adjList[], int nVertex);

#endif
