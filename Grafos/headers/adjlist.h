#ifndef ADJLIST_H
#define ADJLIST_H

typedef struct adjListBlock {
  int keyValue, weight;
  struct adjListBlock *nextBlock;
} adjListBlock;

typedef struct queueBlock {
  int keyValue;
  struct queueBlock *nextBlock;
} queueBlock;

typedef struct queue {
  queueBlock *firstElement, *lastElement;
} queue;

void createAdjList(adjListBlock *adjList[], int nSize);

void inserAdjList(adjListBlock *adjList[], int insertValue);

void printAdjList(adjListBlock *adjList[], int pos);

void startQueue(queue *q);

void enqueue(queue *q, int insertKey);

int dequeue(queue *q);

#endif
