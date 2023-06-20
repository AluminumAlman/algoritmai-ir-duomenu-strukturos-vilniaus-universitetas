#include <stdio.h>
#include "queue.h"

#define MAX_QUEUES 3

void FlushStdin(void);
int GetInputNumber(char *message);
void IsQueueEmpty(Queue *queue);
void IsQueueFull(Queue *queue);
void AddToQueue(Queue *queue);
void RemoveFromQueue(Queue *queue);
void PeekIntoQueue(Queue *queue);
void GetLengthOfQueue(Queue *queue);
void PrintQueue(Queue *queue);
void SelectQueue(Queue *queuesArray[], size_t *queueIndex);
void DeleteQueue(Queue *queueArray[], size_t queueIndex);


int main()
{
        Queue *queuesArray[MAX_QUEUES] = {0};
        size_t queueIndex = 0;

        int selection;
        while(1)
        {
                printf("Pasirinkta eile numeriu %lu. Galimos operacijos:\n"
                       "1) Patikrinti, ar eile tuscia;\n"
                       "2) Patikrinti, ar eile pilna;\n"
                       "3) Prideti nauja elementa prie eiles;\n"
                       "4) Pasalinti elementa is eiles;\n"
                       "5) Pazvelgti i eiles elementa;\n"
                       "6) Gauti eiles ilgi;\n"
                       "7) Atspausdinti eile;\n"
                       "8) Pasirinkti/sukurti kita eile;\n"
                       "9) Istrinti eile;\n"
                       "10) Baigti programos darba;\n", queueIndex+1);
                selection = GetInputNumber("Iveskite pasirinkimo numeri: ");
                switch(selection)
                {
                case(1):
                        IsQueueEmpty(queuesArray[queueIndex]);
                        break;
                case(2):
                        IsQueueFull(queuesArray[queueIndex]);
                        break;
                case(3):
                        AddToQueue(queuesArray[queueIndex]);
                        break;
                case(4):
                        RemoveFromQueue(queuesArray[queueIndex]);
                        break;
                case(5):
                        PeekIntoQueue(queuesArray[queueIndex]);
                        break;
                case(6):
                        GetLengthOfQueue(queuesArray[queueIndex]);
                        break;
                case(7):
                        PrintQueue(queuesArray[queueIndex]);
                        break;
                case(8):
                        SelectQueue(queuesArray, &queueIndex);
                        break;
                case(9):
                        DeleteQueue(queuesArray, queueIndex);
                        break;
                case(10):
                        goto Exit;
                        break;
                default:
                        printf("Neteisingas pasirinkimo numeris.\n");
                }
        }
Exit:
        return(0);
}

void FlushStdin(void)
{
        char tempChar;
        while((tempChar = getchar()) != '\n' && tempChar != '\0')
                ;
        return;
}

int GetInputNumber(char *message)
{
        int retNumber = 0;
        char lastChar = 0;
        printf("%s", message);
        while(1)
        {
                scanf("%d%c", &retNumber, &lastChar);
                if(lastChar == '\n') break;
                FlushStdin();
                printf("%s", message);
        }
        return(retNumber);
}

void IsQueueEmpty(Queue *queue)
{
        if(queue == NULL)
        {
                printf("Pasirinktos eiles nera.\n");
                return;
        }
        if(isempty(queue))
        {
                printf("Eile yra tuscia.\n");
                return;
        }

        printf("Eile nera tuscia.\n");
        return;
}

void IsQueueFull(Queue *queue)
{
        if(queue == NULL)
        {
                printf("Pasirinktos eiles nera.\n");
                return;
        }
        if(isfull(queue))
        {
                printf("Eile yra pilna.\n");
                return;
        }

        printf("Eile nera pilna.\n");
        return;
}

void AddToQueue(Queue *queue)
{
        if(queue == NULL)
        {
                printf("Pasirinktos eiles nera.\n");
                return;
        }
        int numberToAdd = GetInputNumber("Iveskite skaiciu, kuri norite ideti i eile: ");
        if(enqueue(queue, numberToAdd))
        {
                printf("Ivyko klaida idedant skaiciu i eile; eile nepakeista.\n");
        }
        return;
}

void RemoveFromQueue(Queue *queue)
{
        if(queue == NULL)
        {
                printf("Pasirinktos eiles nera.\n");
                return;
        }
        int resultNumber;
        if(dequeue(queue, &resultNumber))
        {
                printf("Ivyko klaida pasalinant pirmaji elementa: eile neturi elementu.\n");
                return;
        }
        printf("Pasalintas pirmasis eiles elementas: skaicius %d.\n", resultNumber);
        return;
}

void PeekIntoQueue(Queue *queue)
{
        if(queue == NULL)
        {
                printf("Pasirinktos eiles nera.\n");
                return;
        }
        int resultNumber;
        if(peekfirst(queue, &resultNumber))
        {
                printf("Ivyko klaida pazvelgiant i pirmaji elementa eileje: eile neturi elementu.\n");
                return;
        }
        printf("Pirmasis eiles elementas: skaicius %d,\n", resultNumber);
        return;
}

void GetLengthOfQueue(Queue *queue)
{
        if(queue == NULL)
        {
                printf("Pasirinktos eiles nera.\n");
                return;
        }
        printf("Eiles ilgis yra %lu.\n", getcount(queue));
        return;
}

void PrintQueue(Queue *queue)
{
        if(queue == NULL)
        {
                printf("Pasirinktos eiles nera.\n");
                return;
        }
        if(!getcount(queue))
        {
                printf("Eile yra tuscia\n");
                return;
        }
        printf("Eile yra sudaryta is elementu:");
        print(queue);
        printf(".\n");
        return;
}

void SelectQueue(Queue *queuesArray[], size_t *queueIndex)
{
        for(size_t index = 0; index < MAX_QUEUES; ++index)
        {
                if(queuesArray[index] == NULL)
                {
                        printf("%lu) sukurti nauja eile numeriu %lu;\n", index+1, index+1);
                        continue;
                }
                printf("%lu) pasirinkti eile numeriu %lu;\n", index+1, index+1);
        }

        int selection = GetInputNumber("Iveskite pasirinkimo numeri: ")-1;
        if(selection < 0 || selection >= MAX_QUEUES)
        {
                printf("Neteisingas pasirinkimo numeris.\n");
                return;
        }
        if(queuesArray[selection] == NULL)
        {
                queuesArray[selection] = create();
        }

        *queueIndex = selection;
}

void DeleteQueue(Queue *queuesArray[], size_t queueIndex)
{
        if(queuesArray[queueIndex] == NULL)
        {
                printf("Pasirinktos eiles nera.\n");
                return;
        }
        destroy(queuesArray[queueIndex]);
        queuesArray[queueIndex] = NULL;
        printf("Istrinta eile numeriu %lu.\n", queueIndex+1);
}
