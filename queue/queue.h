#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdlib.h>
#include <stdio.h>

/**
 * Queue - interface for the queue.
 */
typedef struct Queue Queue;

/**
 * Creates a new queue.
 *
 * @return  pointer to a new Queue struct or NULL on failure
 */
Queue *create(void);

/**
 * Checks whether or not the queue is empty.
 *
 * @param   queue - pointer to a Queue struct
 * @return  1 if empty, 0 otherwise (or if the queue is NULL)
 */
char isempty(Queue *queue);

/**
 * Checks whether or not the queue is full.
 *
 * @param   queue - pointer to a Queue struct
 * @return  1 if full (failed to allocate more memory), 0 otherwise (or if the queue is NULL)
 */
char isfull(Queue *queue);

/**
 * Adds an element to the end of the queue.
 *
 * @param   queue - pointer to a Queue struct
 * @param   value - value to add to the queue
 * @return  0 on success, 1 on failure (queue is NULL or failed to allocate memory)
 */
char enqueue(Queue *queue, int value);

/**
 * Removes an element at the beginning of the queue.
 *
 * @param   queue    - pointer to a Queue struct
 * @param   retValue - optional pointer to an integer to store the dequeued value
 * @return  0 on success, 1 on failure (queue is NULL or queue is empty)
 */
char dequeue(Queue *queue, int *retValue);

/**
 * Peeks at the element at the beginning of the queue.
 *
 * @param   queue    - pointer to a Queue struct
 * @param   retValue - optional pointer to an integer to store the peeked value
 * @return  0 on success, 1 on failure (queue is NULL or queue is empty)
 */
char peekfirst(Queue *queue, int *retValue);

/**
 * Returns the count of elements in the queue.
 *
 * @param   queue - pointer to a Queue struct
 * @return  count of elements in the queue (or 0 if the queue is NULL)
 */
size_t getcount(Queue *queue);

/**
 * Prints the elements within the queue, from start to end.
 *
 * @param   queue - pointer to a Queue struct
 */
void print(Queue *queue);

/**
 * Destroys the queue, freeing all of the memory.
 * DOES NOT SET THE QUEUE POINTER TO NULL!
 *
 * @param   queue - pointer to a Queue struct
 * @return  0 on success, 1 on failure (queue is NULL)
 */
char destroy(Queue *queue);

#endif //#ifndef QUEUE_H_H
