#include <stdio.h>
#include <stdlib.h>

int parseArguments(int argc, char *argv[]) {
    int tableSize;
    if(argc == 2 && (tableSize = atoi(argv[1]))) {
        return tableSize;
    }
    fprintf(stderr, "Wrong arguments. Pass tableSize as an argument\n");
    exit(-1);
}

int isInMemory(int pageRequest, int *pageTable, int tableSize) {
    int i;
    for(i = 0; i < tableSize; i++) {
        if(pageRequest == pageTable[i]) {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int tableSize = parseArguments(argc, argv);    //set table size, aka number of pages, from user input
    int pageRequest, pageTableIndex = 0, firstIn = 0, numRequest = 0, numMisses = 0;
    int *pageTable = (int *) malloc(sizeof(int)*tableSize);
    double hitRate = 0;
    char *input = NULL;
    size_t inputAllocated = 0;
    ssize_t bytesRead;
    FILE *filept, *fifoOut;

    filept= fopen ("accesses.txt", "r");
    fifoOut= fopen("fifoOut", "w");
    
    while((bytesRead = getline(&input, &inputAllocated, filept)) != -1) { //while you are still reading input
        pageRequest = atoi(input); //page request is stored from the input
        if(pageRequest == 0) { 
            continue;
        }
        numRequest++; //increment to keep track of number of page requests
        if(!isInMemory(pageRequest, pageTable, tableSize)) { //if misses
            //printf("Page %d caused a page fault.\n", pageRequest);
            fprintf(fifoOut, "Page %d caused a page fault.\n", pageRequest);
            numMisses++;
            if(pageTableIndex < tableSize) { //if there are empty pages
                pageTable[pageTableIndex++] = pageRequest; //add to the empty page and increment the index
            } else {
                //use FIFO replacement algorithm to replace the page
                pageTable[firstIn] = pageRequest;
                firstIn = (firstIn+1) %tableSize;
            }
        }
    }
    hitRate = (numRequest-numMisses)/(double)numRequest;

    printf("Number of misses = %d\n", numMisses);
    printf("Hit rate = %f\n", hitRate);
    fprintf(fifoOut, "Hit rate = %f\n", hitRate);

    fclose(filept);
    fclose(fifoOut);

    free(input);
    free(pageTable);
    return 0;
}
