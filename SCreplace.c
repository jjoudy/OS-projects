#include <stdio.h>
#include <stdlib.h>

struct pair { //the struct pair contains the data for a page in memory and its reference bit
    int page;
    int reference;
};

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
            pageTable[i].reference = 1;
            return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int tableSize = parseArguments(argc, argv);  //set table size, aka number of pages, from user input
    int pageRequest, pageTableIndex = 0, firstIn = 0, numRequest = 0, numMisses = 0;
    struct pair pageTable[tableSize];
    double hitRate = 0;
    char *input = NULL;
    size_t inputAllocated = 0;
    ssize_t bytesRead;
    FILE *filept, *scOut;

    filept= fopen ("accesses.txt", "r");
    scOut= fopen("SCOut", "w");
    
    while((bytesRead = getline(&input, &inputAllocated, filept)) != -1) { //while you are still reading input
        pageRequest = atoi(input); //page request is stored from the input
        if(pageRequest == 0) { 
            continue;
        }
        numRequest++; //increment to keep track of number of page requests
        if(!isInMemory(pageRequest, pageTable, tableSize)) { //if misses
            //printf("Page %d caused a page fault.\n", pageRequest);
            fprintf(scOut, "Page %d caused a page fault.\n", pageRequest);
            numMisses++;
            if(pageTableIndex < tableSize) { //if there are empty pages
                pageTable[pageTableIndex++].page = pageRequest; //add to the empty page and increment the index
            }else{ //need to use SC page replacement algorithm
                while(pageTable[firstIn].reference == 1){ // if the ref bit is 1, use Second Chance replacement algorithm
                    pageTable[firstIn].reference = 0;
                    firstIn = (firstIn+1) % tableSize; //increment firstIn clock counter
                }
                //the reference bit of this page is 0, so replace
                pageTable[firstIn].page = pageRequest;
                firstIn = (firstIn+1) % tableSize; //increment firstIn clock counter, since page has been replaced
                
            }
        }
    }
    hitRate = (numRequest-numMisses)/(double)numRequest;

    printf("Number of misses = %d\n", numMisses);
    printf("Hit rate = %f\n", hitRate);
    fprintf(scOut, "Hit rate = %f\n", hitRate);

    fclose(filept);
    fclose(scOut);

    free(input);
    //free(pageTable);
    return 0;
}