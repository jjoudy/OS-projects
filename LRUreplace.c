#include <stdio.h>
#include <stdlib.h>

struct pair { //the struct pair contains the data for a page in memory and its reference bit
    int page;
    int counter;
};

int lrucount;

int parseArguments(int argc, char *argv[]) {
    int tableSize;
    if(argc == 2 && (tableSize = atoi(argv[1]))) {
        return tableSize;
    }
    fprintf(stderr, "Wrong arguments. Pass tableSize as an argument\n");
    exit(-1);
}

int isInMemory(int pageRequest, struct pair *pageTable, int tableSize) {
    int i;
    for(i = 0; i < tableSize; i++) {
        if(pageRequest == pageTable[i].page) {
            pageTable[i].counter = lrucount;
            return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    lrucount = 0;

    int tableSize = parseArguments(argc, argv);  //set table size, aka number of pages, from user input
    int pageRequest, pageTableIndex = 0, numRequest = 0, numMisses = 0;
    struct pair pageTable[tableSize];
    double hitRate = 0;
    char *input = NULL;
    size_t inputAllocated = 0;
    ssize_t bytesRead;
    FILE *filept, *lruOut;

    filept= fopen ("accesses.txt", "r");
    lruOut= fopen("lruOut", "w");
    
    while((bytesRead = getline(&input, &inputAllocated, filept)) != -1) { //while you are still reading input
        pageRequest = atoi(input); //page request is stored from the input
        if(pageRequest == 0) { 
            continue;
        }
        lrucount++;
        numRequest++; //increment to keep track of number of page requests
        if(!isInMemory(pageRequest, pageTable, tableSize)) { //if misses
            //printf("Page %d caused a page fault.\n", pageRequest);
            fprintf(lruOut, "Page %d caused a page fault.\n", pageRequest);
            numMisses++;
            if(pageTableIndex < tableSize) { //if there are empty pages
                pageTable[pageTableIndex++].page = pageRequest; //add to the empty page and increment the index
                pageTable[pageTableIndex].counter = lrucount;
            }else{ //no empty pages-use LRU page replacement algorithm
                int i, min = __INT_MAX__, lruIndex = 0;
                for(i = 0; i < tableSize; i++){
                    if(pageTable[i].counter < min){
                        min = pageTable[i].counter;
                        lruIndex = i;
                    } 
                }
                pageTable[lruIndex].page = pageRequest;
                pageTable[lruIndex].counter = lrucount; 
            }
        }
    }
    hitRate = (numRequest-numMisses)/(double)numRequest;

    printf("Number of misses = %d\n", numMisses);
    printf("Hit rate = %f\n", hitRate);
    fprintf(lruOut, "Hit rate = %f\n", hitRate);

    fclose(filept);
    fclose(lruOut);

    free(input);
    return 0;
}