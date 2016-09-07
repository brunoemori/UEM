#include <stdio.h>
#include <stdlib.h>
#include "headers/adjlist.h"

void createAdjList(adjListBlock *adjList[], int nSize) {
  int i;
  for (i = 0; i < nSize; i++) {
    adjList[i] = NULL;
  }
}

void insertAdjList(adjListBlock *adjList[], int pos, int insertValue, int edgeWeight) {
  adjListBlock *newBlock = malloc(sizeof(adjListBlock));
  newBlock->nextBlock = NULL;
  newBlock->keyValue = insertValue;
  newBlock->weight = edgeWeight;

  if (adjList[pos] == NULL)
    adjList[pos] = newBlock;

  else {
    adjListBlock *auxPointer = adjList[pos];
    while (auxPointer->nextBlock != NULL)
      auxPointer = auxPointer->nextBlock;
    
    auxPointer->nextBlock = newBlock;
  }
}

void printAdjList(adjListBlock *adjList[], int pos) {
  adjListBlock *auxPointer = adjList[pos];
  while (auxPointer != NULL) { 
    printf("|(%i) Weight: %i| ", auxPointer->keyValue, auxPointer->weight);
    auxPointer = auxPointer->nextBlock;
  }
}

void startQueue(queue *q) {
  q->firstElement = NULL;
  q->lastElement = NULL;
}

void enqueue(queue *q, int insertElement) {
  queueBlock *newBlock = malloc(sizeof(queueBlock));
  newBlock->keyValue = insertElement;
  newBlock->nextBlock = NULL;

  if (q->lastElement == NULL) {
    q->lastElement = newBlock;
    q->firstElement = newBlock;
  }
  else {
    q->lastElement->nextBlock = newBlock;
    q->lastElement = q->lastElement->nextBlock;
  }
}

int dequeue(queue *q) {
  int removedElement;
  if (q->lastElement != NULL) {
    queueBlock *auxPointer = q->firstElement;
    removedElement = auxPointer->keyValue;
    q->firstElement = q->firstElement->nextBlock;
    if (q->firstElement == NULL)
      q->lastElement == NULL;
  }
  return removedElement;
}

