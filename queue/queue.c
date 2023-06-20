#include "queue.h"

/*
 * QueueNode - internal node of the Queue struct.
 * @member next : void* - the next node the current node references;
 * @member value : int - the value being held by the node.
 */
typedef struct QueueNode {
        void *next;
        int value;
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

Queue *create(void)
{
        Queue *newQueue = malloc(sizeof(Queue));
        if(newQueue == NULL)
        {
                return(NULL);
        }

        newQueue->headNode = NULL;

        return(newQueue);
}

char isempty(Queue *queue)
{
        if(queue == NULL || queue->headNode != NULL)
        {
                return(0);
        }
        return(1);
}

char isfull(Queue *queue)
{
        QueueNode *tempNode = newnode(0);
        if(tempNode == NULL)
        {
                return(1);
        }

        free(tempNode);

        return(0);
}

char enqueue(Queue *queue, int value)
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

char dequeue(Queue *queue, int *retValue)
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

char peekfirst(Queue *queue, int *retValue)
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

size_t getcount(Queue *queue)
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

void print(Queue *queue)
{
        if(queue == NULL)
        {
                return;
        }
        QueueNode *currentNode = queue->headNode;
        while(currentNode != NULL)
        {
                printf("%d ", currentNode->value);
                currentNode = currentNode->next;
        }
        printf("\n");

        return;
}

char destroy(Queue *queue)
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
