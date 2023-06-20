#!/bin/bash

cd `dirname $0`

EXECUTABLE_FILE=exec.elf
SOURCE_FILES=`find ./ -name "*.c"`
HEADER_FILES=`find ./ -name "*.h"`
OBJECT_FILES=${SOURCE_FILES//\.c/.o}

for SOURCE_FILE in $SOURCE_FILES; do
        echo gcc -pedantic -Wall $SOURCE_FILE -c -o ${SOURCE_FILE/\.c/.o}
        gcc -pedantic -Wall $SOURCE_FILE -c -o ${SOURCE_FILE/\.c/.o}
done

echo gcc -pedantic -Wall $OBJECT_FILES -o $EXECUTABLE_FILE
gcc -pedantic -Wall $OBJECT_FILES -o $EXECUTABLE_FILE

#gcc -pedantic -Wall ./main.c -o ./main.elf
