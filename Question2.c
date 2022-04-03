#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

	for (i = 0; i < 1000; i++) {
		blocksize[i] = -1;
	}

	memory[100000] = 2;
	memory[200000] = 2;
	memory[300000] = 2;
	memory[700000] = 2;
	memory[800000] = 2;
	memory[900000] = 2;
	memory[1000000] = 2;

	//create block list
	//to have format Address [200000:549999] Process P1

	int blocklist[1000]; //accessed 0-1 for each start-end //
	int currsize = 0;
	int currblockstart = 0;
	int currblockend;

	int j = 0; //for inserting into blocklist

	for (i = 0; i < size + 1; i++) {
		if (memory[i] == -1) {	//count contiguous -1's showing holes
			currsize++;

		}
		if (memory[i] != -1) {//when encounter another process, mark size of previous block
			currblockend = i;
			blocklist[j] = currblockstart;
			blocklist[++j] = currblockend;

			blocksize[j] = currsize; //block size corresponds with currblockend
			blocksize[j - 1] = currsize; //block size corresponds with currblockend

			currsize = 0;
			currblockstart = currblockend + 1;
			++j;
		}

	} //have to get last block

	printf("start		 1: %d\n", blocklist[0]);
	printf("end                 %d\n", blocklist[1]);
	printf("start 		 2: %d\n", blocklist[2]);
	printf("end 		    %d\n", blocklist[3]);
	printf("start		 3: %d\n", blocklist[4]);
	printf("end 		    %d\n", blocklist[5]);
	printf("start		 4: %d\n", blocklist[6]);
	printf("end 		    %d\n", blocklist[7]);
	printf("start 		 5: %d\n", blocklist[8]);
	printf("end 		    %d\n", blocklist[9]);
	printf("start 	     6: %d\n", blocklist[10]);
	printf("end				%d\n", blocklist[11]);
	printf("start		 7: %d\n", blocklist[12]);
	printf("end			    %d\n", blocklist[13]);


	printf("\nblocksize 0: %d\n", blocksize[0]);
	printf("blocksize 1: %d\n", blocksize[1]);
	printf("blocksize 2: %d\n", blocksize[2]);
	printf("blocksize 3: %d\n", blocksize[3]);
	printf("blocksize 4: %d\n", blocksize[4]);
	printf("blocksize 5: %d\n", blocksize[5]);
	printf("blocksize 6: %d\n", blocksize[6]);
	printf("blocksize 7: %d\n", blocksize[7]);
	printf("blocksize 8: %d\n", blocksize[8]);
	printf("blocksize 9: %d\n", blocksize[9]);
	printf("blocksize 10: %d\n", blocksize[10]);
	printf("blocksize 11: %d\n", blocksize[11]);
	printf("blocksize 12: %d\n", blocksize[12]);
	printf("blocksize 13: %d\n", blocksize[13]);
	printf("blocksize 14: %d\n", blocksize[14]);
	printf("blocksize 15: %d\n", blocksize[15]);
	printf("blocksize 16: %d\n", blocksize[16]);
	printf("blocksize 17: %d\n", blocksize[17]);


	//determine block for best fit

	i = 0;
	int inputsize = 200000;
	int bestblock = 0;

	while (blocksize[i] != -1 && i < 1000) {

		if (blocksize[i] > inputsize && blocksize[i] <= blocksize[bestblock]) {
			bestblock = i;
		}
		i++;
	}
	//bestblock is index of block to use for best fit

	printf("Bestblock: %d	\n", blocksize[bestblock]);

}

