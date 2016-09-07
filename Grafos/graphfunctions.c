#include <stdio.h>
#include <stdlib.h>
#include "headers/graphfunctions.h"
#include "headers/adjlist.h"

int *componentVertex;

searchStruct *dfs, *bfs;

int dfsTime, i;

void printSearch(searchStruct *xfs, int nVertex) {
  for (i = 0; i < nVertex; i++) {
    printf("\nVertex %i: \n", i);
    printf("Ancestor vertex: %i.\n", xfs[i].ancestorVertex);
    printf("Discovery time: %i.\n", xfs[i].timeDiscoveryVertex);
    printf("Final time: %i.\n\n", xfs[i].timeFinalVertex);
  }
}

void printComponents(int *cVertex, int nVertex) {
  for (i = 0; i < nVertex; i++) 
    printf("\nVertex %i in component %i.\n", i, cVertex[i]);
  printf("\n");
}

void dfsVisit(adjListBlock *adjList[], int vertex) {
  dfsTime++;
  dfs[vertex].colorVertex = 1;
  dfs[vertex].timeDiscoveryVertex = dfsTime;
  adjListBlock *auxPointer = adjList[vertex];
  while (auxPointer != NULL) {
    if (dfs[auxPointer->keyValue].colorVertex == 0) {
      dfs[auxPointer->keyValue].ancestorVertex = vertex;
      dfsVisit(adjList, auxPointer->keyValue);
    }
    auxPointer = auxPointer->nextBlock;
  }
  dfs[vertex].colorVertex = 2;
  dfsTime++;
  dfs[vertex].timeFinalVertex = dfsTime;
}

void deepFirstSearch(adjListBlock *adjList[], int nVertex) {
  dfs = malloc(sizeof(dfs) * nVertex);
  for (i = 0; i < nVertex; i++) {
    dfs[i].colorVertex = 0; //0: White, 1: Gray, 2: Black. 
    dfs[i].ancestorVertex = -1;
  }
  dfsTime = 0;
  for (i = 0; i < nVertex; i++) {
    if (dfs[i].colorVertex == 0) {
      dfsVisit(adjList, i);
    }
  }
  printSearch(dfs, nVertex);
}

void breadthFirstSearch(adjListBlock *adjList[], int nVertex, int startVertex) {
  int nextVertex;
  adjListBlock *auxPointer;
  bfs = malloc(sizeof(bfs) * nVertex);
  for (i = 0; i < nVertex; i++) {
    if (i == startVertex)
      continue;
    bfs[i].colorVertex = 0;
    bfs[i].ancestorVertex = -1;
    bfs[i].timeDiscoveryVertex = 0;
  }
  queue q;
  bfs[startVertex].timeDiscoveryVertex = 0;
  bfs[startVertex].ancestorVertex = -1;
  bfs[startVertex].colorVertex = 1;
  startQueue(&q);
  if (q.firstElement == NULL) 
  enqueue(&q, startVertex);
  while (q.firstElement != NULL) {
    nextVertex = dequeue(&q);
    auxPointer = adjList[nextVertex];
    while (auxPointer != NULL) {
      if (bfs[auxPointer->keyValue].colorVertex == 0) {
        bfs[auxPointer->keyValue].colorVertex = 1 ;
        bfs[auxPointer->keyValue].ancestorVertex = nextVertex;
        bfs[auxPointer->keyValue].timeDiscoveryVertex = bfs[nextVertex].timeDiscoveryVertex + 1;
        enqueue(&q, auxPointer->keyValue);
      }
      auxPointer = auxPointer->nextBlock;
    }
    bfs[nextVertex].colorVertex = 2;
  }
  printSearch(bfs, nVertex);
}

void connectecComponentVisit(adjListBlock *adjList[], int vertex, int k) {
  dfsTime++;
  dfs[vertex].colorVertex = 1;
  dfs[vertex].timeDiscoveryVertex = dfsTime;
  adjListBlock *auxPointer = adjList[vertex];
  while (auxPointer != NULL) {
    if (dfs[auxPointer->keyValue].colorVertex == 0) {
      dfs[auxPointer->keyValue].ancestorVertex = vertex;
      dfsVisit(adjList, auxPointer->keyValue);
      componentVertex[auxPointer->keyValue] = k;
    }
    auxPointer = auxPointer->nextBlock;
  }
  dfs[vertex].colorVertex = 2;
  dfsTime++;
  dfs[vertex].timeFinalVertex = dfsTime;
}

void connectedComponent(adjListBlock *adjList[], int nVertex) {
  componentVertex = malloc(sizeof(int) * nVertex);
  int k = 0;
  for (i = 0; i < nVertex; i++) {
    dfs[i].colorVertex = 0; //0: White, 1: Gray, 2: Black. 
    dfs[i].ancestorVertex = -1;
  }
  dfsTime = 0;
  for (i = 0; i < nVertex; i++) {
    if (dfs[i].colorVertex == 0) {
      connectecComponentVisit(adjList, i, k);
      k++;
    }
  }
  printComponents(componentVertex, nVertex);
}


