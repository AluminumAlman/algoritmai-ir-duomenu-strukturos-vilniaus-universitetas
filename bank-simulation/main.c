#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "BigInt.h"
#include "queue.h"


#define RETURN_FAILURE(value) do { result = (value); goto ReturnFailure; } while(0)

#define STR_VAR(var) #var
#define TO_STRING(var) STR_VAR(var)

#define BIGINT_CHARBUF_CAP 32
#define BIGINT_CHARBUF_CAP_STR TO_STRING(BIGINT_CHARBUF_CAP)
#define INDENT "    "

const char *SIMULATION_STATE_ERRORS[] = {
        "jokios klaidos nebuvo",
        "nepavyko nuskaityti kasininku skaiciaus",
        "nepavyko nuskaityti kliento aptarnavimo laiko",
        "nepavyko nuskaityti kliento atvykimo tikimybes",
        "nepavyko gauti atminties kasininkams saugoti",
        "nepavyko gauti atminties zingsniu skaiciui saugoti",
        "nepavyko gauti atminties kitam sustojimo zingsniui saugoti",
        "nepavyko gauti atminties klientu eilei saugoti"
};

typedef struct Clerk
{
        size_t BusyTimer;
        size_t ClientId;
} Clerk;

typedef int ERRNO;

static FILE *actualStdin = NULL;


size_t SimState_ClerksCount;
Clerk *SimState_Clerks;

size_t SimState_ClientServeSteps;
size_t SimState_ClientChancePerStep;

BigInt *SimState_Steps;
BigInt *SimState_NextPauseStep;

Queue *SimState_ClientsQueue;
size_t SimState_ClientsCount;

size_t SimState_TotalClientsServed;


void printResults(void)
{
        printf("TRECIA DALIS. REZULTATAI\n"
                        "Aptarnauta klientu: %lu klientu(-ai).\n", SimState_TotalClientsServed);
}

void cleanupSimulationState(void)
{
        if(SimState_Clerks != NULL) free(SimState_Clerks);

        if(SimState_Steps != NULL) BigInt_free(SimState_Steps);

        if(SimState_NextPauseStep != NULL) BigInt_free(SimState_NextPauseStep);

        if(SimState_ClientsQueue != NULL) destroyQueue(SimState_ClientsQueue);

        return;
}


ERRNO setupSimulationState(const char *fileName)
{
        srand(time(NULL));

        ERRNO result = 0;
        fputs("PIRMA DALIS.\n"
                "3 uzduotis, 1 variantas. Almantas Mecele, 1 PS kursas, 3 grupe, 2 pogrupis.\n"
                "\n"
                "1.1. SALYGA\n"
                "Procesas: klientai ateina i banka ir kreipiasi i bet kuri laisva kasininka,\n"
                "jei visi kasininkai uzsieme, klientai stoja i bendra eile.\n"
                "Tikslas: patyrineti, kaip tikslingiau bankui elgtis - tureti mazai\n"
                "greitai aptarnaujancius kasininkus ar daug letai aptarnaujanciu kasininku.\n"
                "\n"
                "1.2. PRADINIAI DUOMENYS\n", stdout);

        if(stdin == actualStdin) fputs("Pradiniai duomenys vedami is konsoles\n", stdout);
        else
        {
                fputs("Pradiniu duomenu failas yra \"", stdout);
                fputs(fileName, stdout);
                fputs("\"\n", stdout);
        }

        printf("Kasininku skaicius: ");
        if(scanf("%lu", &SimState_ClerksCount) != 1 || ferror(stdin)) RETURN_FAILURE(1);
        if(stdin != actualStdin) printf("% 21lu kasininku(-ai).\n", SimState_ClerksCount);

        printf("Kliento aptarnavimo laikas: ");
        if(scanf("%lu", &SimState_ClientServeSteps) != 1 || ferror(stdin)) RETURN_FAILURE(2);
        if(stdin != actualStdin) printf("% 13lu min.\n", SimState_ClientServeSteps);

        printf("Kliento atvykimo tikimybe procentais: ");
        if(scanf("%lu", &SimState_ClientChancePerStep) != 1 || ferror(stdin)) RETURN_FAILURE(3);
        if(stdin != actualStdin) printf("% 3lu %%\n", SimState_ClientChancePerStep);

        SimState_Clerks = malloc(sizeof(Clerk) * SimState_ClerksCount + 16);
        if(SimState_Clerks == NULL) RETURN_FAILURE(4);

        SimState_Steps = BigInt_construct(0);
        if(SimState_Steps == NULL) RETURN_FAILURE(5);

        SimState_NextPauseStep = BigInt_construct(0);
        if(SimState_NextPauseStep == NULL) RETURN_FAILURE(6);

        SimState_ClientsQueue = createQueue();
        if(SimState_ClientsQueue == NULL) RETURN_FAILURE(7);

        SimState_ClientsCount = 0;
        SimState_TotalClientsServed = 0;

        return 0;

ReturnFailure:
        cleanupSimulationState();
        return result;
}

int askForNextPauseStep(void)
{
        int result;
        static char stringBuff[BIGINT_CHARBUF_CAP] = "";

        if(stdin == actualStdin) printf("Ties kelintu zingsniu sustoti? ");

        if((result = scanf(" %" TO_STRING(BIGINT_CHARBUF_CAP) "[0-9]", stringBuff)) != 1
                        || result == EOF) return 0;

        BigInt_free(SimState_NextPauseStep);
        SimState_NextPauseStep = BigInt_from_string(stringBuff);

        if(SimState_NextPauseStep == NULL)
        {
                printf("Ivyko klaida: nepavyko sukurti ilgo sveiko skaiciaus.\n");
                return 0;
        }

        if(BigInt_compare(SimState_Steps, SimState_NextPauseStep) != -1) return 0;

        return 1;
}

void doStep(char skip)
{
        printf("T = ");
        BigInt_print(SimState_Steps);
        printf(" min.\n");

        if(!skip && (size_t)((double)rand() / RAND_MAX * 100) <= SimState_ClientChancePerStep)
        {
                ++SimState_ClientsCount;
                enqueueQueue(SimState_ClientsQueue, SimState_ClientsCount);
                printf(INDENT "Atvyko naujas klientas Kl%lu.\n", SimState_ClientsCount);
        }

        for(size_t i = 0; i < SimState_ClerksCount; ++i)
        {

                char flags = 0;
                size_t clientId = SimState_Clerks[i].ClientId;


                if(SimState_Clerks[i].BusyTimer > 0)
                {
                        --SimState_Clerks[i].BusyTimer;
                }

                if(SimState_Clerks[i].BusyTimer == 0 && clientId != 0)
                {
                        flags |= 1;
                        ++SimState_TotalClientsServed;
                        SimState_Clerks[i].ClientId = 0;
                }

                if(SimState_Clerks[i].ClientId == 0)
                {
                        if(isemptyQueue(SimState_ClientsQueue))
                        {
                                goto PrintClerkStatus;
                        }
                        flags |= 2;
                        dequeueQueue(SimState_ClientsQueue, &(SimState_Clerks[i].ClientId));
                        SimState_Clerks[i].BusyTimer = SimState_ClientServeSteps;
                }

PrintClerkStatus:
                printf(INDENT "Kasininkas Ka%lu", i + 1);

                switch(flags)
                {
                        case(0):
                                if(SimState_Clerks[i].ClientId == 0)
                                {
                                        printf(" laukia kliento.");
                                        break;
                                }
                                printf(" aptarnauja klienta Kl%lu. Aptarnavimo laiko liko %lu min.",
                                        clientId, SimState_Clerks[i].BusyTimer);
                                break;
                        case(1):
                                printf(" baiges aptarnauti klienta Kl%lu.", clientId);
                                break;
                        case(2):
                                printf(" priime klienta Kl%lu. Aptarnavimo laiko liko %lu min.",
                                        SimState_Clerks[i].ClientId, SimState_Clerks[i].BusyTimer);
                                break;
                        case(1 | 2):
                                printf(" baiges aptarnauti klienta Kl%lu priime klienta Kl%lu."
                                        " Aptarnavimo laiko liko %lu min.",
                                        clientId, SimState_Clerks[i].ClientId, SimState_Clerks[i].BusyTimer);
                                break;
                }
                putchar('\n');
        }

        if(isemptyQueue(SimState_ClientsQueue))
        {
                printf(INDENT "Nera klientu eileje.\n\n");
                return;
        }
        printf(INDENT "Klientai eileje: ");
        printQueue(SimState_ClientsQueue);
        fputs(".\n\n", stdout);

        return;
}


int main(int argc, char *argv[])
{
        int result = EXIT_SUCCESS;

        //
        // Initialize the input stream
        // By overwriting stdin with an open file, we can call scanf instead of fscanf to read from the file
        //
        actualStdin = stdin;
        const char *inputFileName = argv[1];
        if(argc >= 2)
        {
                stdin = fopen(inputFileName, "rb");
                if(stdin == NULL)
                {
                        printf("Ivyko klaida atveriant faila %s skaitymui: %s\n", argv[1], strerror(errno));
                        result = EXIT_FAILURE;
                        goto ReturnFailure;
                }
        }

        //
        // Dump the obligatory first part of protocol and initialize the simulation state
        //
        if((result = setupSimulationState(inputFileName)))
        {
                printf("\nIvyko klaida kuriant pradine busena: %s.\n", SIMULATION_STATE_ERRORS[result]);
                result = EXIT_FAILURE;
                goto ReturnFailure;
        }

        //
        // Print the start of the obligatory second part of the protocol
        //
        printf("\n"
                        "ANTRA DALIS. Protokolas\n");

        //
        // Do stepping and printing.
        //
        doStep(1); // Fake step for T=0
        while(askForNextPauseStep())
        {
                while(BigInt_compare_digits(SimState_Steps, SimState_NextPauseStep) == -1)
                {
                        BigInt_add_int(SimState_Steps, 1);
                        doStep(0);
                }
                //printInfo();
        }

        printResults();

ReturnFailure:
        cleanupSimulationState();

        if(stdin != actualStdin && stdin != NULL) fclose(stdin);

        stdin = actualStdin;

        return result;
}
