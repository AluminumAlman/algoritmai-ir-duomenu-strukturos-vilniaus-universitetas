#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "priorityqueue.h"
#include "bitmap.h"

#define INDENT "    "
#define MAX_CITIES 128
#define CITIES_BIT_MAP_ARRAY_SIZE 16 // MAX_CITIES / 8
#define MAX_CITY_NAME_LEN 24
#define MAX_LINE_LENGTH 96

#define STR_VAR(var) #var
#define TO_STRING(var) STR_VAR(var)
#define EARLY_RETURN(ret) do {result = ( ret ); goto FunctionEnd; } while(0)

typedef struct DijkstraInfo {
        int dist;
        int prev;
} DijkstraInfo;

typedef struct PathQueueInfo {
        int dist;
        int prev;
        int next;
} PathQueueInfo;

int cityCount = 0;
char cityNames[MAX_CITIES][MAX_CITY_NAME_LEN] = {0};
int cityTravelCosts[MAX_CITIES][MAX_CITIES] = {0};

unsigned char cityVisitedBitMapArray[CITIES_BIT_MAP_ARRAY_SIZE] = {0};
BitMap cityVisitedMap = {.size = CITIES_BIT_MAP_ARRAY_SIZE, .map = cityVisitedBitMapArray};

DijkstraInfo dijkstraCityInfos[MAX_CITIES] = {0};
PriorityQueue cityPathQueue = {0};

int cityTravelPathCount = 0;
int cityTravelPath[MAX_CITIES] = {0};

size_t steps = 0;
int cityNamesLongestStrLength = 0;
int biggestTravelCostStrLength = 0;

int numLength(int num)
{
    int result = 0;
    do
    {
        num /= 10;
        ++result;
    } while(num != 0);
    return result;
}

int searchForCity(const char *cityName)
{
        for(int i = 0; i < cityCount; ++i)
        {
                if(strcmp(cityNames[i], cityName) == 0) return i;
        }
        return -1;
}

int pathQueueNewPrint = 1;
int pathQueuePrintedChars = 0;
unsigned char pathQueuePrintFunc(const long int priority, const void *data)
{
    if(pathQueueNewPrint)
    {
        pathQueuePrintedChars = pathQueueNewPrint = 0;
        pathQueuePrintedChars += printf("         " INDENT " Prioritetines eiles elementai:");
    }
    if(pathQueuePrintedChars >= MAX_LINE_LENGTH)
        pathQueuePrintedChars = printf("\n         " INDENT INDENT);

    const PathQueueInfo *info = data;
    pathQueuePrintedChars += printf(" (%s, %s, %d)", cityNames[info->prev], cityNames[info->next], info->dist);

    return 0;
}

int main(int argc, char *argv[])
{
        int result = 0;
        FILE *inFile = NULL;

        char firstCityName[MAX_CITY_NAME_LEN] = {0}, secondCityName[MAX_CITY_NAME_LEN] = {0};
        int firstI, secondI;
        int dist;

        // Protokolo spausdinimas

        printf("PIRMA DALIS\n"
               "4 uzduotis, 3 variantas. Almantas Mecele, 1 PS kursas, 3 grupe, 2 pogrupis.\n"
               "\n"
               "1.1. SALYGA\n"
               "Duotas keliu sarasas: miestas, miestas, atstumas.\n"
               "Rasti trumpiausia keliones is vieno duoto miesto i kita duota miesta marsruta ir jo ilgi.\n"
               "Numatyti atveji, kad toks marsrutas neegzistuoja.\n"
               "(Grafo realizacija paremta kaimynystes matrica)\n"
               "\n"
               "1.2. SPRENDIMO IDEJA\n"
               "Naudojantis Dijkstros algoritmu ir prioritetine eile surasti trumpiausia marsruta.\n"
               "Neradus marsruto teigti, kad jo nera.\n"
               "\n"
               "1.3. PRADINIAI DUOMENYS\n");

        if (argc < 2)
        {
                printf("Nepateiktas joks pradiniu duomenu failas.\n");
                EARLY_RETURN(1); 
        }
        inFile = fopen(argv[1], "rb");
        if(inFile == NULL)
        {
                printf("Nepavyko atverti pradiniu duomenu failo \"%s\": %s\n", argv[1], strerror(errno));
        }
        printf("Pradiniu duomenu failas \"%s\"\n", argv[1]);

        // Matricos paruosimas
        
        //memset(cityTravelCosts, -1, sizeof(cityTravelCosts));

        // Duomenu nuskaitymas

        int scanfResult = 0;
        while((scanfResult = fscanf(inFile, "%"TO_STRING(MAX_CITY_NAME_LEN)"[A-Za-z], "
                                            "%"TO_STRING(MAX_CITY_NAME_LEN)"[A-Za-z], "
                                            "%d ",
                                            firstCityName, secondCityName, &dist)) == 3)
        {
                if((firstI = searchForCity(firstCityName)) == -1)
                {
                        strcpy(cityNames[firstI = cityCount], firstCityName);
                        ++cityCount;
                }
                if((secondI = searchForCity(secondCityName)) == -1)
                {
                        strcpy(cityNames[secondI = cityCount], secondCityName);
                        ++cityCount;
                }
                cityTravelCosts[firstI][secondI] = cityTravelCosts[secondI][firstI] = dist;
        }

        if(scanfResult != 2)
        {
                printf("Netinkamai surasyti duomenys.\n");
                goto FunctionEnd;
        }

        for(int i = 0; i < cityCount; ++i)
        {
            cityTravelCosts[i][i] = 0;
        }

        // Pranesimas apie ieskoma marsruta

        printf("Ieskoma marsruto is %s i %s.\n", firstCityName, secondCityName);

        if((firstI = searchForCity(firstCityName)) == -1)
        {
                printf("Nera sarase pradinio miesto.\n");
                goto FunctionEnd;
        }
        if((secondI = searchForCity(secondCityName)) == -1)
        {
                printf("Nera sarase siekiamo miesto.\n");
                goto FunctionEnd;
        }

        // Ilgiausiu simboliu eiluciu skaiciavimas

        for(int i = 0; i < cityCount; ++i)
        {
            int length = strlen(cityNames[i]);
            if(length > cityNamesLongestStrLength)
                cityNamesLongestStrLength = length;

            for(int j = 0; j < cityCount; ++j)
            {
                int travelCostStrLength = numLength(cityTravelCosts[i][j]);
                if(travelCostStrLength > biggestTravelCostStrLength)
                    biggestTravelCostStrLength = travelCostStrLength;
            }
        }

        // Kaiminystes matrica

        int maxStringLength = cityNamesLongestStrLength > biggestTravelCostStrLength
                ? cityNamesLongestStrLength
                : biggestTravelCostStrLength;
        ++maxStringLength;

        printf("Miestu kaimynystes matrica:\n");
        int printedChars = 0;
        printedChars += printf("% *s|", maxStringLength, "");
        for(int i = 0; i < cityCount; ++i)
            printedChars += printf("% *s ", maxStringLength, cityNames[i]);
        putchar('\n');
        for(int i = 0; i < printedChars; ++i)
            putchar('-');
        putchar('\n');
        for(int i = 0; i < cityCount; ++i)
        {
            printf("% *s|", maxStringLength, cityNames[i]);
            for(int j = 0; j < cityCount; ++j)
                printf("% *d ", maxStringLength, cityTravelCosts[j][i]);

            putchar('\n');
        }

        // Marsrutu sarasas

        printf("Marsrutu sarasas:\n");
        for(int i = 0; i < cityCount; ++i)
        {
                printf("% *s:\n", cityNamesLongestStrLength, cityNames[i]);
                for(int j = 0; j < cityCount; ++j)
                {
                        if(cityTravelCosts[i][j] <= 0) continue;

                        printf(INDENT "% *s, % *d;\n", cityNamesLongestStrLength, cityNames[j], biggestTravelCostStrLength + 1, cityTravelCosts[j][i]);
                }
        }

        cityPathQueue = PriorityQueue_new();

        for(int i = 0; i < cityCount; ++i) memset(dijkstraCityInfos+i, -1, sizeof(DijkstraInfo));

        PathQueueInfo *info;
        info = malloc(sizeof(PathQueueInfo) + 8);
        if(info == NULL)
        {
                printf("Ivyko klaida: %s\n", strerror(errno));
                goto FunctionEnd;
        }

        printf("\n"
               "ANTRA DALIS. Protokolas\n");

        info->dist = 0;
        info->prev = -1;
        info->next = firstI;
        PriorityQueue_enqueue(&cityPathQueue, info->dist, info);

        char found;
        found = 0;
        while(!PriorityQueue_isEmpty(&cityPathQueue) && !found)
        {
                ++steps;
                info = PriorityQueue_dequeue(&cityPathQueue);

                if(info->prev != -1)
                    printf("% 8lu) Tikrinamas miestas %s su keliu is %s ir atstumu %d:\n",
                            steps, cityNames[info->next], cityNames[info->prev], info->dist);
                else
                    printf("% 8lu) Tikrinamas miestas %s:\n", steps, cityNames[info->next]);

                int printedChars = 0;
                printedChars += printf("         "INDENT" Neaplankyti miestai:");
                for(int i = 0; i < cityCount; ++i)
                {
                    if (BitMap_isSetAt(&cityVisitedMap, i)) continue;
                    if (printedChars >= MAX_LINE_LENGTH)
                        printedChars = printf("\n         " INDENT INDENT);

                    printedChars += printf(" %s", cityNames[i]);
                }
                putchar('\n');
                printedChars = printf("         "INDENT" Aplankyti miestai:");
                for(int i = 0; i < cityCount; ++i)
                {
                    if (!BitMap_isSetAt(&cityVisitedMap, i)) continue;
                    if (printedChars >= MAX_LINE_LENGTH)
                        printedChars = printf("\n         " INDENT INDENT);

                    printedChars += printf(" %s", cityNames[i]);
                }
                putchar('\n');

                if(BitMap_isSetAt(&cityVisitedMap, info->next))
                {
                        if(info->next != firstI)
                            printf("         "INDENT" Miestas anksciau aplankytas su keliu is miesto %s su atstumu %d.\n",
                                cityNames[dijkstraCityInfos[info->next].prev],
                                dijkstraCityInfos[info->next].dist);
                        else
                            printf("         " INDENT " Miestas anksciau aplankytas su atstumu %d.\n",
                                    dijkstraCityInfos[info->next].dist);

                        goto Continue;
                }
                BitMap_setAt(&cityVisitedMap, info->next);
                dijkstraCityInfos[info->next].dist = info->dist;
                dijkstraCityInfos[info->next].prev = info->prev;
                if(info->prev != -1)
                    printf("         "INDENT" Miestas pazymetas kaip aplankytas keliu is miesto %s su atstumu %d.\n",
                            cityNames[info->prev],
                            info->dist);
                else
                    printf("         " INDENT " Miestas pazymetas kaip aplankytas su atstumu %d.\n",
                            info->dist);


                if(info->next == secondI)
                {
                        found = 1;
                        printf("         "INDENT" Rasta marsruto pabaiga.\n");
                        goto Continue;
                }


                PathQueueInfo *newInfo;
                unsigned char checkedOne = 0;
                for(int i = 0; i < cityCount; ++i)
                {
                    if(cityTravelCosts[i][info->next] <= 0) continue;

                    checkedOne = 1;

                    newInfo = malloc(sizeof(PathQueueInfo) + 8);
                    if(newInfo == NULL)
                    {
                            printf("Ivyko klaida: %s\n", strerror(errno));
                            goto FunctionEnd;
                    }

                    newInfo->dist = info->dist + cityTravelCosts[i][info->next];
                    newInfo->prev = info->next;
                    newInfo->next = i;
                    PriorityQueue_enqueue(&cityPathQueue, newInfo->dist, newInfo);
                    printf("         "INDENT" I eile pridetas kelias is %s i %s su atstumu %d;\n",
                            cityNames[newInfo->prev], cityNames[newInfo->next], newInfo->dist);
                }

                if(!checkedOne) printf("         "INDENT" Miestas neturi keliu i kitus miestus.\n");
Continue:
                pathQueueNewPrint = 1;
                PriorityQueue_passOverAll(&cityPathQueue, &pathQueuePrintFunc);
                putchar('\n');
                free(info);
        }

        printf("\n"
               "TRECIA DALIS. Rezultatai\n");
        if(found)
        {
                printf("Rastas marsrutas is %s i %s su atstumu %d:\n", cityNames[firstI], cityNames[secondI],
                        dijkstraCityInfos[secondI].dist);

                int cityI;
                cityI = secondI;
                while(cityI != -1)
                {
                        cityTravelPath[cityTravelPathCount] = cityI;
                        ++cityTravelPathCount;
                        cityI = dijkstraCityInfos[cityI].prev;
                }
                for(cityI = cityTravelPathCount; cityI > 1; )
                {
                        --cityI;
                        printf(INDENT"% *s (atstumas% *d),\n", cityNamesLongestStrLength,
                                cityNames[cityTravelPath[cityI]],
                                biggestTravelCostStrLength + 1,
                                dijkstraCityInfos[cityTravelPath[cityI]].dist);
                }
                --cityI;
                printf(INDENT"% *s (atstumas% *d)\n", cityNamesLongestStrLength,
                        cityNames[cityTravelPath[cityI]],
                        biggestTravelCostStrLength + 1,
                        dijkstraCityInfos[cityTravelPath[cityI]].dist);
        }
        else printf("Nerastas marsrutas is %s i %s.\n", cityNames[firstI], cityNames[secondI]);


FunctionEnd:
        PriorityQueue_destroy(&cityPathQueue, &free);
        if(inFile != NULL) fclose(inFile);

        return result == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
