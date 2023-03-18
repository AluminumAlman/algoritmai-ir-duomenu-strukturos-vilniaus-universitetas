#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define TRUE 1
#define FALSE 0
#define BOOLEAN int

#define SET_CELL_AT_AS(x, y, toc) do { triangleBoard[triangleLength * ( y ) + ( x )] = ( toc ); } while(0)

size_t steps = 0;

enum TYPE_OF_CELL
{
        TOC_EMPTY = 0,
        TOC_QUEEN = 1
};

const unsigned char TOC_TO_CHAR[] = "_Q";

/**
 * Flushes stdin by repeatedly calling getchar() until a null byte or a new line.
 */
void flushStdin(void)
{
        char tempChar;
        while((tempChar = getchar()) != '\n' && tempChar != '\0')
                ;

        return;
}

/**
 * Gets a number from stdin; contains input validation.
 *
 * @return user input number
 */
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

/**
 * "Fires two rays" up from current position (excluding it)
 * to check whether or not they hit a queen; if so, returns false.
 *
 * @return true (1) or false (0)
 */
static BOOLEAN isCellSafe(unsigned char *triangleBoard, size_t triangleLength, size_t x, size_t y)
{
        for(size_t testY = y-1; testY != (size_t)(-1); --testY)
                if(triangleBoard[triangleLength * testY + x + testY - y] == TOC_QUEEN ||
                                triangleBoard[triangleLength * testY + x] == TOC_QUEEN)
                        return(FALSE);

        return(TRUE);
}

/**
 * Recursively places queens on the board.
 *
 * @return true (1) on success or false (0) on failure
 */
static BOOLEAN tryPlacingQueens(unsigned char *triangleBoard, size_t triangleLength, const size_t x, const size_t y, size_t queenCount, size_t depth)
{
        ++steps;
        printf("% 8lu) ", steps);

        for(size_t i = 0; i < depth; ++i)
                putchar('-');

        printf("X=%lu Y=%lu N=%lu. ", x+1, y+1, queenCount);

        if(!isCellSafe(triangleBoard, triangleLength, x, y))
        {
                if(x == triangleLength-1 && y == triangleLength-1)
                        printf("Uzblokuota. Backtrack.\n");
                else
                        printf("Uzblokuota.\n");

                return(FALSE);
        }

        printf("Laisva.\n");
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

        SET_CELL_AT_AS(x, y, TOC_EMPTY);
        return(FALSE);
}

/**
 * Entry function for filling in the triangle board recursively.
 *
 * @return true (1) on success or false (0) on failure
 */
BOOLEAN tryFillTriangleBoard(unsigned char *triangleBoard, size_t triangleLength, size_t queenCount)
{
        if(queenCount == 0)
                return(TRUE);

        for(size_t y = 0; y < triangleLength; ++y)
                if(tryPlacingQueens(triangleBoard, triangleLength, 0, y, queenCount-1, 0))
                        return(TRUE);

        return(FALSE);
}

/**
 * Prints the triangle board.
 *
 * Example: 4 length triangle board
 *    _
 *   _ _
 *  _ _ _
 * _ _ _ _
 */
void printTriangleBoard(unsigned char *triangleBoard, size_t triangleLength)
{
        for(size_t y = 0; y < triangleLength; ++y)
        {
                for(size_t padding = 0; padding < triangleLength - y - 1; ++padding)
                        putchar(' ');

                for(size_t x = 0; x <= y; ++x)
                {
                        putchar(TOC_TO_CHAR[ triangleBoard[y * triangleLength + x] ]);
                        putchar(' ');
                }
                putchar('\n');
        }

        return;
}

int main(int argc, char *argv[])
{
        printf("Autorius - Almantas Mecele, 1 PS kursas, 3 grupe, 2 pogrupis.\n");

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

        printf("\n"
                        "2 pagrindine uzduotis, 14 variantas.\n"
                        "Randa N valdoviu issidestyma M ilgio trikampeje lentoje, kad valdoves viena kitos nekirstu.\n"
                        "\nPIRMA DALIS. Duomenys\n");

        if(inputStream == stdin)
        {
                queenCount = getUserNumber("Iveskite neneigiama sveika skaiciu N - valdoviu skaiciu: ");
                triangleLength = getUserNumber("Iveskite neneigiama sveika skaiciu M - trikampes lentos ilgi: ");
        }
        else
        {
                printf("N - valdoviu skaicius: %lu.\n"
                                "M - trikampes lentos ilgis: %lu.\n",
                                queenCount, triangleLength);
        }


        printf("\nANTRA DALIS. Vykdymas\n");

        unsigned char *triangleBoard = malloc(triangleLength * triangleLength);
        if(triangleBoard == NULL)
        {
                printf("Klaida; %s.\n", strerror(errno));
                goto earlyExit;
        }
        memset(triangleBoard, TOC_EMPTY, triangleLength * triangleLength);


        BOOLEAN result = tryFillTriangleBoard(triangleBoard, triangleLength, queenCount);

        printf("\nTRECIA DALIS. Rezultatai\n"
                        "1) Issidestymas %s. Bandymu: %lu\n",
                        result ? "rastas" : "nerastas", steps);
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
