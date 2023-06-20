#!/bin/bash
mkdir windows
for INPUTF in $(find "./" -name "*duom*.txt" -type f); do
        OUTPUTF=${INPUTF//duom/protokolas}
        ./exec.elf "$INPUTF" > "$OUTPUTF"
        cat $OUTPUTF > "./windows/$OUTPUTF"
        cat $INPUTF > "./windows/$INPUTF"
        unix2dos "./windows/$OUTPUTF"
        unix2dos "./windows/$INPUTF"
done
