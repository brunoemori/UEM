#ifndef GRAPHFUNCTIONS_H
#define GRAPHFUNCTIONS_H
#include "adjlist.h"

typedef struct searchStruct {
  int colorVertex, ancestorVertex, timeDiscoveryVertex, timeFinalVertex;
} searchStruct;

void printSearch(searchStruct *xfs, int nVertex);

void printComponents(int *cVertex, int nVertex);

void printShortestPath(searchStruct *initSingleSource, int nVertex, int startVertex);

void deepFirstSearch(adjListBlock *adjList[], int nVertex);

void dfsVisit(adjListBlock *adjList[], int vertex);

void bfsVisit(adjListBlock *adjList[], int nVertex, int startVertex);

void breadthFirstSearch(adjListBlock *adjList[], int nVertex, int startVertex);

void printComponents(int cVertex[], int nVertex);

void connectedComponentVisit(adjListBlock *adjList[], int vertex, int k);

void connectedComponent(adjListBlock *adjList[], int nVertex);

void initializeSingleSource(adjListBlock *adjList[], int nVertex, int startVertex);

void relaxEdge(int sVertex, int dVertex, int edgeWeight);

void dijkstra(adjListBlock *adjList[], int nVertex, int startVertex);

#endif
