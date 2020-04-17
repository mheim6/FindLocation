# FindLocation

In the [findlocationfast.c] program, we implement a tool to always take two arguments: 
(1) a filename, and (2) a 6-digit numerical prefix. This tool opens a file given by the first
argument and performs a binary search on it for the location identified by the prefix specified by
the second argument. If a corresponding line from the file is found, then the line is output to standard 
output and strips the spaces at the end the prefix at the beginning. The complexity of this search is O(log n). 
In order to avoid using level-3 system calls, a decision was made to make our own functions to implement string 
comparisons and checking inputs from the second argument. To compile this program, use the following command:
gcc -Wall -O3 -o findlocationfast findlocationfast.c. To run the program, use the following command (in Linux): 
./findlocationfast <arg1> <arg2>.
In the [findlocationfastmemory.c] program, we implement the same tool as [findlocationfast.c], 
but we use mapping of the contents in the file instead. The complexity of this search is also O(log n). 
In order to avoid using level-3 system calls, a decision was made to make our own functions to implement 
string comparisons and checking inputs from the second argument. However, it was discovered this program still 
has errors and fails to compile and run correctly. To compile this program, use the following command: gcc -Wall -O3 -o
findlocationfastmemory findlocationfastmemory.c. To run the program, use the following command (in Linux): 
./findlocationfastmemory <arg1> <arg2>.
