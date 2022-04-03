#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>

/* global variable declaration */
int size = 1000000;  //get this as cmd line arg
int blocksize[1000];  //will contain the sizes of different processes

//void allocate(char *process, int process_size) {
//	int i = 0;
//
//	int max = 0;
//	int count = 1;
//
//	for (i = 0; i < size; i++) { //loop through entire memory to find make list block sizes
//		if (memory[i] == -1) {
//			count++;
//
//		} else {
//			if (count > max)
//				max = count;
//
//		}
//
//	}
//	printf("%d\n", count);
//	printf("%d\n", max);
//
//	return;
//}

int main(void) {

	int memory[size];

	int i = 0;

	for (i = 0; i < size; i++) {  //initialize memory block with -1's
		memory[i] = -1;
	}


	//create block list
	//to have format Address [200000:549999] Process P1

	int blocklist[1000]; //accessed 0-1 for each start-end //
	int currsize = 0;
	int currblockstart = 0;
	int currblockend;

	int j = 0; //for inserting into blocklist

	for (i = 0; i < size; i++) {
		if (memory[i] == -1) {	//count contiguous -1's showing holes
			currsize++;

		}
		if (memory[i] != -1) {//when encounter another process, mark size of previous block
			currblockend = i;
			blocklist[j] = currblockstart;
			blocklist[++j] = currblockend;

			blocksize[j] = currsize; //block size corresponds with currblockend
			currsize = 0;
			currblockstart = currblockend+1;
			++j;
		}

	}



	printf("blocklist 0: %d\n", blocklist[0]);
	printf("blocklist 1: %d\n", blocklist[1]);
	printf("blocklist 2: %d\n", blocklist[2]);
	printf("blocklist 3: %d\n", blocklist[3]);
	printf("blocklist 4: %d\n", blocklist[4]);
	printf("blocklist 5: %d\n", blocklist[5]);
	printf("blocklist 6: %d\n", blocklist[6]);
	printf("blocklist 7: %d\n", blocklist[7]);


	printf("\nblocksize 0: %d\n", blocksize[1]);
	printf("blocksize 1: %d\n", blocksize[3]);
	printf("blocksize 2: %d\n", blocksize[5]);
	printf("blocksize 3: %d\n", blocksize[7]);

	//determine block for best fit


}

