#include "queue.h"

/*
 * QueueNode - internal node of the Queue struct.
 * @member next : void* - the next node the current node references;
 * @member value : int - the value being held by the node.
 */
typedef struct QueueNode {
        void *next;
        size_t value;
} QueueNode;

struct Queue {
        QueueNode *headNode;
};

static QueueNode *newnode(int value)
{
        QueueNode *newNode = malloc(sizeof(QueueNode));
        if(newNode == NULL)
        {
                return(NULL);
        }

        newNode->next = NULL;
        newNode->value = value;

        return(newNode);
}

Queue *createQueue(void)
{
        Queue *newQueue = malloc(sizeof(Queue));
        if(newQueue == NULL)
        {
                return(NULL);
        }

        newQueue->headNode = NULL;

        return(newQueue);
}

char isemptyQueue(Queue *queue)
{
        if(queue == NULL || queue->headNode != NULL)
        {
                return(0);
        }
        return(1);
}

char isfullQueue(Queue *queue)
{
        QueueNode *tempNode = newnode(0);
        if(tempNode == NULL)
        {
                return(1);
        }

        free(tempNode);

        return(0);
}

char enqueueQueue(Queue *queue, size_t value)
{
        if(queue == NULL)
        {
                return(-1);
        }

        // First element of the queue
        QueueNode *node = queue->headNode;
        if(node == NULL)
        {
                queue->headNode = newnode(value);
                return(queue->headNode == NULL ? 1 : 0);
        }

        // Later elements of the queue
        while(node->next != NULL)
        {
                node = node->next;
        }
        node->next = newnode(value);

        return(node->next == NULL ? 1 : 0);
}

char dequeueQueue(Queue *queue, size_t *retValue)
{
        if(queue == NULL || queue->headNode == NULL)
        {
                return(-1);
        }

        QueueNode *tempNode = queue->headNode;
        queue->headNode = tempNode->next;

        if(retValue != NULL)
        {
                *retValue = tempNode->value;
        }
        free(tempNode);

        return(0);
}

char peekfirstQueue(Queue *queue, size_t *retValue)
{
        if(queue == NULL || queue->headNode == NULL)
        {
                return(-1);
        }

        if(retValue != NULL)
        {
                *retValue = queue->headNode->value;
        }

        return(0);
}

size_t getcountQueue(Queue *queue)
{
        if(queue == NULL)
        {
                return(0);
        }
        QueueNode *node = queue->headNode;

        size_t count;
        for(count = 0; node != NULL; node = node->next)
        {
                ++count;
        }

        return(count);
}

void printQueue(Queue *queue)
{
        if(queue == NULL)
        {
                return;
        }
        QueueNode *currentNode = queue->headNode;

        int shouldIndent = 0;
        size_t counter = 0;
        while(currentNode != NULL)
        {
                ++counter;
                printf("Kl%lu", currentNode->value);
                currentNode = currentNode->next;
                if(currentNode != NULL)
                {
                        if(counter == 10)
                        {
                                counter = 0;
                                printf("\n                    ");
                                shouldIndent = 1;
                        }
                        putchar(' ');
                }
        }

        return;
}

char destroyQueue(Queue *queue)
{
        if(queue == NULL)
        {
                return(-1);
        }

        QueueNode *currentNode = queue->headNode,
                  *nextNode;
        while(currentNode != NULL)
        {
                nextNode = currentNode->next;
                free(currentNode);
                currentNode = nextNode;
        }

        free(queue);
        return(0);
}
