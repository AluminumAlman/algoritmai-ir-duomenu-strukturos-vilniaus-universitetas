#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Type of cell definitions
enum TYPE_OF_CELL
{
        TOC_EMPTY = 0,
        TOC_QUEEN = 1
};
const char TOC_TO_CHAR[] = "_Q";


// Flushes the standard input stream by repeatedly calling getchar() until a new line or null character is found
void flushStdin(void)
{
        char tempChar;
        while((tempChar = getchar()) != '\n' && tempChar != '\0')
                ;
        return;
}

// Prints the string by calling putchar()
void printString(char *str)
{
        while(*str != '\0')
        {
                putchar(*str);
                ++str;
        }
}

// Gets a single number from user input
size_t getUserNumber(char *msg)
{
        size_t retNumber = 0;
        char lastChar = 0;
        printString(msg);
        while(1)
        {
                scanf("%lu%c", &retNumber, &lastChar);
                if(lastChar == '\n')
                        break;

                flushStdin();
                printString(msg);
        }

        return(retNumber);
}

// Prints the triangle in a nice format
void printTriangle(unsigned char triangleCells[], size_t triangleLength)
{
        for(size_t yOffset = 0; yOffset < triangleLength; ++yOffset)
        {
                for(size_t padding = 0; padding < triangleLength - yOffset - 1; ++padding)
                        putchar(' ');

                for(size_t xOffset = 0; xOffset <= yOffset; ++xOffset)
                {
                        putchar(TOC_TO_CHAR[ triangleCells[yOffset * triangleLength + xOffset] ]);
                        putchar(' ');
                }
                putchar('\n');
        }

        return;
}

// Tries filling the triangle, returns success (1) or failure (0)
unsigned char tryFillTriangle(unsigned char triangleCells[], size_t triangleLength,
                size_t queenCount, const size_t yOffset)
{
        // Don't bother trying to fill it if we know that there's not enough space
        if(queenCount > (2 * triangleLength + 1) / 3)
                return(0);


        // Try filling in the board by placing the queens alternating between left and right
        // triangle sides while ensuring the queens on either side are above each other
        size_t curYOffset = yOffset;
        while(queenCount > 0)
        {
                switch(curYOffset & 1)
                {
                case(1):
                        triangleCells[curYOffset * triangleLength + ((curYOffset + yOffset + 1) >> 1)] = TOC_QUEEN;
                        break;
                default:
                        triangleCells[curYOffset * triangleLength + ((curYOffset - yOffset) >> 1)] = TOC_QUEEN;
                }

                --queenCount;
                ++curYOffset;
        }

        return(1);
}

int main(int argc, char *argv[])
{
        printString("Randa N valdoviu issidestyma M ilgio trikampeje lentoje, kad valdoves viena kitos nekirstu.\n");

        size_t queenCount = getUserNumber("Iveskite neneigiama sveika skaiciu N (valdoviu skaiciu): ");
        size_t triangleLength = getUserNumber("Iveskite neneigiama sveika skaiciu M (trikampes lentos ilgi): ");
        // Yes, this is >=50% space efficient; too bad it's easier to implement it this way
        unsigned char *triangleCells = malloc(triangleLength * triangleLength * sizeof(unsigned char) + 16);
        if(triangleCells == NULL)
        {
                printf("Klaida: %s\n", strerror(errno));
                goto earlyReturn;
        }
        // Prepares the memory space
        memset(triangleCells, TOC_EMPTY, triangleLength * triangleLength * sizeof(unsigned char));


        switch(tryFillTriangle(triangleCells, triangleLength, queenCount, triangleLength - queenCount))
        {
        case(0): // Failure
                printString("Nera tokio issidestymo.\n");
                break;
        default: // Success
                printString("Yra issidestymas:\n");
                printTriangle(triangleCells, triangleLength);
        }


        free(triangleCells);

earlyReturn:
        return(EXIT_SUCCESS);
}
