#ifndef PRIORITYQUEUE_H_
#define PRIORITYQUEUE_H_ 1

#include <stdlib.h>
#include <errno.h>

#define GUARD_BYTES 8
#define OPTIONAL

typedef struct PriorityQueue PriorityQueue;
typedef struct PriorityQueueNode PriorityQueueNode;

extern PriorityQueue PriorityQueue_new(void);
extern int PriorityQueue_enqueue(PriorityQueue *queue, const long int priority, const void *data);
extern void *PriorityQueue_dequeue(PriorityQueue *queue);
extern void PriorityQueue_passOverAll(const PriorityQueue *queue,
                                      unsigned char (*passFunc)(const long int priority, const void *data));
extern void PriorityQueue_destroy(PriorityQueue *queue,
                                  OPTIONAL void (*dataDestroyFunc)(void *data));

struct PriorityQueue
{
        PriorityQueueNode *start;
};

struct PriorityQueueNode
{
        long int priority;
        void *data;
        PriorityQueueNode *next;
};

PriorityQueue PriorityQueue_new(void)
{
        PriorityQueue result = {0};
        return result;
}

int PriorityQueue_enqueue(PriorityQueue *queue, const long int priority, const void *data)
{
        PriorityQueueNode *newNode, *queueNode;

        newNode = malloc(sizeof(PriorityQueueNode) + GUARD_BYTES);
        if(newNode == NULL) return errno;
        newNode->priority = priority;
        newNode->data = (void*)data;

        queueNode = queue->start;
        if(queueNode == NULL || queueNode->priority > priority)
        {
                newNode->next = queueNode;
                queue->start = newNode;
                return 0;
        }
        while(queueNode->next != NULL && queueNode->next->priority <= priority)
                queueNode = queueNode->next;

        newNode->next = queueNode->next;
        queueNode->next = newNode;

        return 0;
}

void *PriorityQueue_dequeue(PriorityQueue *queue)
{
        PriorityQueueNode *node;
        void *result;

        node = queue->start;
        if(node == NULL) return NULL;

        result = node->data;
        queue->start = node->next;
        free(node);

        return result;
}

unsigned char PriorityQueue_isEmpty(const PriorityQueue *queue)
{
        return queue->start == NULL;
}

void PriorityQueue_passOverAll(const PriorityQueue *queue,
                               unsigned char (*passFunc)(const long int priority, const void *data))
{
        PriorityQueueNode *node;
        node = queue->start;

        while(node != NULL)
        {
                if(passFunc(node->priority, node->data)) break;
                node = node->next;
        }
}

void PriorityQueue_destroy(PriorityQueue *queue,
                           OPTIONAL void (*dataDestroyFunc)(void *data))
{
        while(!PriorityQueue_isEmpty(queue))
        {
                void *data = PriorityQueue_dequeue(queue);
                if(dataDestroyFunc != NULL) dataDestroyFunc(data);
        }
}

#undef GUARD_BYTES
#undef OPTIONAL

#endif // #ifndef PRIORITYQUEUE_H_
