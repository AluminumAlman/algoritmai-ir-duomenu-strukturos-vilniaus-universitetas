#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define TRUE 1
#define FALSE 0
#define BOOLEAN int

#define SET_CELL_AT_AS(x, y, toc) do { triangleBoard[triangleLength * ( y ) + ( x )] = ( toc ); } while(0)

size_t steps = 0;
size_t totalQueens = 0;

FILE *outputFile = NULL;

enum TYPE_OF_CELL
{
        TOC_EMPTY = 0,
        TOC_QUEEN = 1
};

const unsigned char TOC_TO_CHAR[] = "_Q";

void flushStdin(void)
{
        char tempChar;
        while((tempChar = getchar()) != '\n' && tempChar != '\0')
                ;

        return;
}

size_t getUserNumber(char *msg)
{
        size_t retNumber = 0;
        char lastChar = 0;
        printf("%s", msg);
        while(1)
        {
                scanf("%lu%c", &retNumber, &lastChar);
                if(lastChar == '\n')
                        break;

                flushStdin();
                printf("%s", msg);
        }

        return(retNumber);
}

size_t getNumberLen(size_t number)
{
        size_t result = 1;
        while(number > 9)
        {
                number /= 10;
                ++result;
        }

        return(result);
}

BOOLEAN isCellSafe(unsigned char *triangleBoard, size_t triangleLength, size_t x, size_t y)
{
        for(size_t testY = y-1; testY != (size_t)(-1); --testY)
                if(triangleBoard[triangleLength * testY + x + testY - y] == TOC_QUEEN ||
                                triangleBoard[triangleLength * testY + x] == TOC_QUEEN)
                        return(FALSE);

        return(TRUE);
}

BOOLEAN tryPlacingQueens(unsigned char *triangleBoard, size_t triangleLength, const size_t x, const size_t y, size_t queenCount, size_t depth)
{
        ++steps;
        printf("\n% 8lu) ", steps);

        for(size_t i = 0; i < depth; ++i)
                putchar('-');

        printf("Bandome valdove %lu X=%lu Y=%lu.", totalQueens-queenCount, x+1, triangleLength-y);

        if(!isCellSafe(triangleBoard, triangleLength, x, y))
        {
                printf(" Kertama. Liko valdoviu %lu.", queenCount);
                goto EarlyReturn;
        }
        printf(" Statome. Liko valdoviu %lu.", queenCount);

        //if(!isCellSafe(triangleBoard, triangleLength, x, y))
        //{
        //        if(x == triangleLength-1 && y == triangleLength-1)
        //                printf("Kertama. Griztame nes nesekme.\n");
        //        else
        //                printf("Kertama.\n");

        //        return(FALSE);
        //}

        //printf("Statome.%s\n", x == triangleLength-1 && y == triangleLength-1 && queenCount > 0 ? " Griztame nes nesekme." : "");
        SET_CELL_AT_AS(x, y, TOC_QUEEN);

        if(queenCount == 0)
                return(TRUE);

        for(size_t yBoard = y+1; yBoard < triangleLength; ++yBoard)
        {
                for(size_t xBoard = 0; xBoard < yBoard + 1; ++xBoard)
                {
                        if(tryPlacingQueens(triangleBoard, triangleLength, xBoard, yBoard, queenCount-1, depth + 1))
                                return(TRUE);
                }
        }

EarlyReturn:
        if(x == triangleLength-1 && y == triangleLength-1)
        {
                printf(" Griztame nes nesekme.");
        }
        SET_CELL_AT_AS(x, y, TOC_EMPTY);
        return(FALSE);
}

BOOLEAN tryFillTriangleBoard(unsigned char *triangleBoard, size_t triangleLength, size_t queenCount)
{
        if(queenCount == 0)
                return(TRUE);

        for(size_t y = 0; y < triangleLength; ++y)
                if(tryPlacingQueens(triangleBoard, triangleLength, 0, y, queenCount-1, 0))
                        return(TRUE);

        return(FALSE);
}

void *cloneArray(void *array, size_t size)
{
        void *newArray = malloc(size);
        if(newArray == NULL)
                return(NULL);

        return memcpy(newArray, array, size);
}

void printDummyTriangleBoard(size_t triangleLength)
{
        size_t numLength = getNumberLen(triangleLength);

        printf(" % *s ^\n", (int)numLength, "Y");
        for(size_t y = 0; y < triangleLength; ++y)
        {
                printf(" % *lu | ", (int)numLength, triangleLength - y);
                for(size_t padding = 0; padding < triangleLength - y - 1; ++padding)
                        printf("% *s", (int)numLength, "");

                for(size_t x = 0; x <= y; ++x)
                {
                        printf("% *lu% *s", (int)numLength, x+1, (int)numLength, "");
                }
                putchar('\n');
        }

        for(size_t i = 0; i < 2 + numLength; ++i)
                putchar(' ');

        putchar('-');
        for(size_t i = 0; i < triangleLength * numLength * 2; ++i)
                putchar('-');

        putchar('-');
        putchar('>');
        putchar(' ');
        putchar('X');

        putchar('\n');
        return;
}

void printTriangleBoard(unsigned char *triangleBoard, size_t triangleLength)
{
        size_t numLength = getNumberLen(triangleLength);

        printf("    % *s ^\n", (int)numLength, "Y");
        for(size_t y = 0; y < triangleLength; ++y)
        {
                printf("    % *lu | ", (int)numLength, triangleLength - y);
                for(size_t padding = 0; padding < triangleLength - y - 1; ++padding)
                        putchar(' ');

                for(size_t x = 0; x <= y; ++x)
                {
                        putchar(TOC_TO_CHAR[ triangleBoard[y * triangleLength + x] ]);
                        putchar(' ');
                }
                putchar('\n');
        }

        for(size_t i = 0; i < 5 + numLength; ++i)
                putchar(' ');

        putchar('-');
        for(size_t i = 0; i < triangleLength * 2; ++i)
                putchar('-');

        putchar('-');
        putchar('>');
        putchar(' ');
        putchar('X');

        putchar('\n');
        return;
}

int main(int argc, char *argv[])
{
        printf("PIRMA DALIS.\n"
                        "2 uzduotis, 14 variantas. Almantas Mecele, 1 PS kursas, 3 grupe, 2 pogrupis.\n"
                        "\n"
                        "1.1. SALYGA.\n"
                        "Rasti N valdoviu issidestyma M ilgio trikampeje lentoje, kad valdoves viena kitos nekirstu.\n"
                        "\n"
                        "1.2. SPRENDIMO IDEJA.\n"
                        "Spresti pradedama nuo lentos virsaus ir einama zemyn, statant valdoves nuo kaires i desine kiekvienoje eiluteje.\n"
                        "Pries pastatant kiekviena valdove tikrinama, ar jos nekerta kitos.\n"
                        "Jeigu kerta, tuomet bandoma statyti kitose vietose toje pacioje eiltuje arba, pasiekus eilutes pabaiga, einama eilute zemyn.\n"
                        "Sitaip kiekvienoje eiluteje ieskoma vieta naujai valdovei. Neradus vietos, ivyksta sugrizimas ir tikrinamas kitas valdoviu issidestymas.\n"
                        "Sustacius visas valdoves sprendimas baigiasi.\n"
                        "\n"
                        "1.3. PRADINIAI DUOMENYS.\n");

        FILE *inputStream = stdin;
        switch(argc)
        {
        case(1):
                printf("Testu rinkinys vedamas is konsoles.\n");
                break;
        default:
                printf("Testu rinkinys: %s\n", argv[1]);
                inputStream = fopen(argv[1], "r");
                if(inputStream == NULL)
                {
                        printf("Klaida; nepavyko atverti failo %s: %s.\n", argv[1], strerror(errno));
                        goto earlyExit;
                }
        }

        size_t queenCount, triangleLength;

        if(inputStream != stdin)
        {
                if(fscanf(inputStream, "%lu %lu", &queenCount, &triangleLength) != 2)
                {
                        printf("Klaida; faile nera dvieju neneigiamu skaiciu.\n");
                        goto earlyExit;
                }
        }

        if(inputStream == stdin)
        {
                queenCount = getUserNumber("Iveskite neneigiama sveika skaiciu N - valdoviu skaiciu: ");
                triangleLength = getUserNumber("Iveskite neneigiama sveika skaiciu M - trikampes lentos ilgi: ");
        }
        else
        {
                printf("Valdoviu skaicius N = %lu.\n"
                                "Trikampes lentos ilgis M = %lu.\n",
                                queenCount, triangleLength);
        }

        printf("Lentos issidestymas:\n");
        printDummyTriangleBoard(triangleLength);


        printf("\nANTRA DALIS. Protokolas");

        unsigned char *triangleBoard = malloc(triangleLength * triangleLength);
        if(triangleBoard == NULL)
        {
                printf("Klaida; %s.\n", strerror(errno));
                goto earlyExit;
        }
        memset(triangleBoard, TOC_EMPTY, triangleLength * triangleLength);

        totalQueens = queenCount;
        BOOLEAN result = tryFillTriangleBoard(triangleBoard, triangleLength, queenCount);

        printf("\n\nTRECIA DALIS. Rezultatai\n"
                        "1) %s issidestymas. Bandymu %lu.\n",
                        result ? "Rastas" : "Nerastas", steps);
        if(result)
        {
                printf("2) Issidestymas lentoje:\n");
                printTriangleBoard(triangleBoard, triangleLength);
        }

earlyExit:
        if(inputStream != NULL && inputStream != stdin)
                fclose(inputStream);

        if(triangleBoard != NULL)
                free(triangleBoard);

        return(0);
}
