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
Queue *createQueue(void);

/**
 * Checks whether or not the queue is empty.
 *
 * @param   queue - pointer to a Queue struct
 * @return  1 if empty, 0 otherwise (or if the queue is NULL)
 */
char isemptyQueue(Queue *queue);

/**
 * Checks whether or not the queue is full.
 *
 * @param   queue - pointer to a Queue struct
 * @return  1 if full (failed to allocate more memory), 0 otherwise (or if the queue is NULL)
 */
char isfullQueue(Queue *queue);

/**
 * Adds an element to the queue (First In First Out).
 *
 * @param   queue - pointer to a Queue struct
 * @param   value - value to add to the queue
 * @return  0 on success, 1 on failure (queue is NULL or failed to allocate memory)
 */
char enqueueQueue(Queue *queue, size_t value);

/**
 * Removes an element from the queue (First In First Out).
 *
 * @param   queue    - pointer to a Queue struct
 * @param   retValue - optional pointer to an integer to store the dequeued value
 * @return  0 on success, 1 on failure (queue is NULL or queue is empty)
 */
char dequeueQueue(Queue *queue, size_t *retValue);

/**
 * Peeks at the first element (First In First Out).
 *
 * @param   queue    - pointer to a Queue struct
 * @param   retValue - optional pointer to an integer to store the peeked value
 * @return  0 on success, 1 on failure (queue is NULL or queue is empty)
 */
char peekfirstQueue(Queue *queue, size_t *retValue);

/**
 * Returns the count of elements in the queue.
 *
 * @param   queue - pointer to a Queue struct
 * @return  count of elements in the queue (or 0 if the queue is NULL)
 */
size_t getcountQueue(Queue *queue);

/**
 * Prints the elements within the queue, from start to end;
 * Does not print a new line and leaves no trailing whitespace.
 *
 * @param   queue - pointer to a Queue struct
 */
void printQueue(Queue *queue);

/**
 * Destroys the queue, freeing all of the memory used by the queue;
 * DOES NOT SET THE QUEUE POINTER TO NULL!
 *
 * @param   queue - pointer to a Queue struct
 * @return  0 on success, 1 on failure (queue is NULL)
 */
char destroyQueue(Queue *queue);

#endif //#ifndef QUEUE_H_H
