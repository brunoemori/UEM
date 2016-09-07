#include <stdio.h>
#include <stdlib.h>
#include "headers/adjlist.h"

//Made by Bruno Emori (RA 88736) & Christian Nakata (RA90558)
//Algoritmos em Grafos - Prof Rodrigo Calvo
//Universidade Estadual de Maringá - 2016
int main() {
  int nVertex, nEdges, graphType, v1, v2, edgeWeight, count;
  fscanf(stdin, "%i", &nVertex);
  fscanf(stdin, "%i", &nEdges);
  fscanf(stdin, "%i", &graphType);

  if ((graphType != 0) && (graphType != 1)) {
    printf("Graph type must be (0) - Undirected graph, or (1) - Directed graph.\n");
    return 0;
  }
 
  adjListBlock *adjList[nVertex];
  createAdjList(adjList, nVertex);

  while (fscanf(stdin, "%i", &v1) != EOF) {
    fscanf(stdin, "%i", &v2);
    fscanf(stdin, "%i", &edgeWeight);
    insertAdjList(adjList, v1, v2, edgeWeight);
    if (!graphType)
     insertAdjList(adjList, v2, v1, edgeWeight);
  }

  printf("Number of vertices: %i.\n", nVertex);
  printf("Number of edges: %i.\n", nEdges);
  if (!graphType)
    printf("Graph Type: Undirected Graph\n");
  else
    printf("Graph Type: Directed Graph\n");
  
  printf("\nEdges:\n");
  for (count = 0; count < nVertex; count++) {
    printf("Vertex %i: ", count);
    printAdjList(adjList, count);
    printf("\n");
  }

  printf("\n\nDeep First Search:\n");
  deepFirstSearch(adjList, nVertex);

  printf("\n\nBreadth First Search:\n");
  breadthFirstSearch(adjList, nVertex, 0);

  printf("\n\nComponents:\n");
  connectedComponent(adjList, nVertex);
  
  if (graphType) {
    printf("\n\nShortest path: (Using Dijkstra's Algorithm)\n");
    dijkstra(adjList, nVertex, 0);
  }
  
  printf("End Program.\n");
  return 0;
}
