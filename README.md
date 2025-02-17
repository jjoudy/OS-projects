# OS-projects
Project done for an undergraduate OS class.
Jessica Joudy

Each of the page replacement/caching algorithms work off of the same base code: 
there are two helper functions called "parseArguments", which parses the passed argument of the program 
for the number of pages, and "IsInMemory", which checks if the requested page is in the page table. 
Each algorithm has a page table of the size "tableSize", which represents the number of page frames. 
In the main, each implementation opens the file and reads page requests from "accesses.txt", which is the provided lab text of the list of 
page requests to test our algorithms. Each algorithm keeps track of the number of requests and the number of misses. 
Each algorithm is tested through the 'diff' command, comparing our results of a simulation of 100 page frames, which is a list of all the 
misses in each algorithm, to the given files that should have the proper results of the correct implementations. 
If there are no differences in the output in each file, the algorithms were correct. 
I tested all of the algorithms with the resulting output files in this way.

FIFO:
The implementation of the FIFO page replacement/caching algorithm simulation is in the file "FIFOreplace.c". 
This algorithm works using a page table which is simply an array of integers. 
It reads the page requests from the input file, here accesses.txt, and then checks if it is in the page table using IsInMemory. 
If it is not, it is a miss. Then, it checks if there are empty spots in the page table using an index called pageTableIndex. 
If there are, it simply puts the page request in the next available page frame and increments. 
If not, the index called firstIn keeps track of the first-in, implementing a circular array. 
The page request replaces the page in index firstIn, and the index is set to `firstIn = (firstIn+1) %tableSize` 
to increment it with a circular property. 

Second Chance:
The implementation of the SC page replacement/caching algorithm simulation is in the file "SCreplace.c". 
There is an additional struct called pair, made of two ints: the page number and the reference bit. 
This algorithm works using a page table which is an array of these pairs. 
It reads the page requests from the input file, here accesses.txt, and then checks if it is in the page table using IsInMemory, and then sets the reference bit to 1 if it is. 
If it is not, it is a miss. 
Then, it checks if there are empty spots in the page table using an index called pageTableIndex. 
If there are, it simply puts the page request in the next available page frame and increments. 
If not, a while loop runs while the reference bit of the pair at firstIn is 1, which sets the refence bit to 0
and increments firstIn as the index of a circular array.
Then, when it reaches the first page frame where the page has a 0'd reference bit, it replaces that page in the same manner as FIFO.


LRU:
The implementation of the LRU page replacement/caching algorithm simulation is in the file "LRUreplace.c". 
The struct pair has two ints: the page number and the counter. 
It also maintains a global counter, lrucount, which increments each time a page is requested. 
This algorithm works using a page table which is an array of these pairs. 
It reads the page requests from the input file, here accesses.txt, and then checks if it is in the page table using IsInMemory, 
and then sets the counter to lrucount if it is. If it is not, it is a miss. 
Then, it checks if there are empty spots in the page table using an index called pageTableIndex. 
If there are, it simply puts the page request in the next available page frame and increments, then sets the counter to lrucount. 
If not, then the program loops through the page table to find the page frame with the lowest counter, 
meaning that it is the least recently used, and replaces it, resetting the counter to lrucount.


Graph:
Plotted a graph, which is available in the file "MISSES PER PAGE FRAMES", as PDF or SVG. 
The graph is a scatter plot which plots the number of misses per amount of page frames available, testing for page frames between 10 and 500, in a sequence of 10, 50, 100, etc. page frames. 
This test was done for all three algorithms simulated, and they are all plotted on this graph. 
The blue is FIFO, the red is SC, and the yellow is LRU. 
According to the graph, even though the numbers of misses were all fairly similar, the LRU algorithm appears to be the most effective 
because it has the least number of misses most consistently, compared to the other two. 
However, LRU as properly implemented does not work as a page replacement algorithm, only a caching algorithm.
