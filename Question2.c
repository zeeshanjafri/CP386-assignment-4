#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>

#define MAXP 1000

/* global variable declaration */
//int size = argv[0];  //get this as cmd line arg
int blocksize[1000];  //will contain the sizes of different processes

struct Process {
	char *name;
	int size;
	int start;
	int end;
} Process;
//check if a new process can fit in memory by:
//create an array of processes
//identify holes in memory by creating an array of holes and populate it.
//loop thru array of processes.

struct Memory {
	int size;
	int blocklist[1000]; 	// [startP0,endP0,startP0,endP0...]
	int blocksize[1000];	// [sizeP0,sizeP0,sizeP1,sizeP1...]
	int memory[1000000];	// [-1,-1,-1,-1,-1,1,1,1,1,1,1,1,1]
} Memory;

void init_memory(struct Memory *memory, int size) {
	if (!memory) {
		printf("invalid memory passed to create memory\nExiting..\n");
		return;
	}
	int i = 0;
	for (i = 0; i < size; i++) {  //initialize memory block with -1's
		memory->memory[i] = -1;
	}
	memory->size = size;
	printf("Allocated %d bytes of memory\n", size);

//	printf("\ndynamically allocated memory: %d\n",memory->memory[10000]);
	return;
}

struct Process* create_process(char *name, int size) {
	struct Process *process;
	process = malloc(sizeof(Process));

	process->name = name;
	process->size = size;
	/*	printf("Inside the creator function\n");
	 printf("Process name: %s\n", process->name);
	 printf("Process start: %d\n", process->start);
	 printf("Process end: %d\n", process->end);
	 printf("Process size: %d\n", process->size);
	 printf("\n"); */
//	printf("*Created Process*\n");
	return process;
}

void print_process(struct Process *process) {
	printf("Address [%d:%d] Process %s\n", process->start, process->end,
			process->name);
	return;
}

void insert_plist(struct Process *processes[], struct Process *process) {
	int i = 0;

	while (i < MAXP) {

		if (processes[i] == NULL) { //assign process to first NULL space in process list
			processes[i] = process;
		}
		if (processes[i] == NULL)
			break;
		i++;
	}
}

//checks the Process list for best-fitting hole
//returns 0 or index
int best_fit(struct Memory *memory, struct Process *process) {
	printf("entered best fit\n");
	//int memory->memory[1000]; //accessed 0-1 for each start-end //
	int currsize = 0;
	int currblockstart = 0;
	int currblockend;

	int j = 0; //for inserting into memory
	int i;
	printf("Before the for loop...\n");
	for (i = 0; i < memory->size +1 ; i++) {
		if (memory->memory[i] == -1) {	//count contiguous -1's showing holes
			currsize++;
		}
		if (memory->memory[i] != -1) {//when encounter another process, mark size of previous block
			currblockend = i;
			memory->blocklist[j] = currblockstart;
			memory->blocklist[++j] = currblockend;

			memory->blocksize[j] = currsize; //block size corresponds with currblockend
			memory->blocksize[j - 1] = currsize; //block size corresponds with currblockend

			currsize = 0;
			currblockstart = currblockend + 1;
			++j;
		}

	} //have to get last block

	//determine block for best fit

	i = 0;
	//int inputsize = 200000;
	int bestblock = 0;
	int inputsize = process->size;

	printf("I made it to this while loop\n");
	while (memory->blocksize[i] != -1 && i < 1000) {

		if (memory->blocksize[i] > inputsize) {
			if (memory->blocksize[i] > memory->blocksize[bestblock])
				bestblock = i;
		}
		i++;
	}
	printf("I survived this while loop\n");
	//bestblock is index of block to use for best fit

//	printf("Bestblock size: %d	\n", memory->blocksize[bestblock]);
//	printf("Bestblock is between: %d and %d\n", memory->blocklist[bestblock],
//			memory->blocklist[bestblock + 1]);

	if (memory->blocksize[bestblock]) {
		process->start = memory->blocklist[bestblock];
		process->end = memory->blocklist[bestblock] + process->size;
		int i = process->start;
//		printf("Process->start: %d\n", process->start);
//		printf("Process->end: %d\n", process->end);

		printf("memory->memory[i]: %d\n", memory->memory[i]);
		while (i < process->end) {		//write into memory for this process
//			if (memory->memory[i] != -1)
//				break;
			memory->memory[i] = 1;
			i++;
		}
		printf("Successfully allocated %d to process %s\n", process->size,
				process->name); //goes in allocation function
	}

	return bestblock;
}

int main(int argc, char *argv[]) {

	/*test process
	 struct Process *p_test;
	 p_test = malloc(sizeof(Process));

	 /*	p_test->name = "P0";
	 p_test->start = 10000;
	 p_test->end = 20000;
	 p_test->size = p_test->end - p_test->start;
	 printf("Process name: %s\n", p_test->name);
	 printf("Process start: %d\n", p_test->start);
	 printf("Process end: %d\n", p_test->end);
	 printf("Process size: %d\n", p_test->size);
	 printf("\n");

	 print_process(p_test); */		//Testing process creation
	//test process creator
//	char string_name[20] = "P1";
//	int sizee = 10000;
//	struct Process *p1 = create_process(string_name, sizee);
//	printf("Printing Process:\n");
//	print_process(p1);
	int size = atoi(argv[1]);
	int memory[size];

	int i = 0;

	//initialize the RAM to work with
	struct Memory *mem;
	mem = malloc(sizeof(Memory));
	init_memory(mem, size);

	//initial list of processes with all NULLs
	struct Process *process_list[1000];
	for (i = 0; i < 1000; i++) {  //initialize memory block with NULL's
		process_list[i] = NULL;
	}

	//create block list
	//to have format Address [200000:549999] Process P1

	char input[40];
	char str1[15];
	strcpy(str1, "Exit");

	char *token;

	for (;;) /* input loop */
	{
		fputs("Command>", stdout);
		fflush(stdout);
		fgets(input, sizeof input, stdin);
		if (strcmp(input, str1) == 10)
			break;

		char *pch;
		pch = strtok(input, " ,.-");

		//			printf("%s\n", pch);
		//			pch = strtok(NULL, " ,.-");

		char *arg1 = NULL;
		char *arg2 = NULL;
		char *arg3 = NULL;
		char *arg4 = NULL;

		if (pch != NULL) { //extract args from successive input
			arg1 = pch;
			pch = strtok(NULL, " ,.-");
			//	printf("%s\n", arg1);
			if (pch != NULL) {
				arg2 = pch;
				pch = strtok(NULL, " ");
				//	printf("%s\n", arg2);
			}
			if (pch != NULL) {
				arg3 = pch;
				pch = strtok(NULL, " ");
				//	printf("%s\n", arg3);
			}
			if (pch != NULL) {
				arg4 = pch;
				pch = strtok(NULL, " ");
				//	printf("%s\n", arg4);
			}
		} //grab 4 arguments from command line for requests

		switch (strcmp(arg1, "RQ")) { //	printf("%d\n",strcmp(arg1,"RQ")); (to get switch case value)
		case 0: /* Case of "RQ" */
		{
			struct Process *p;
			p = create_process(arg2, atoi(arg3));
			insert_plist(process_list, p);
			best_fit(mem, p);
			print_process(p);
//			printf("%s\n", arg1);
//			printf("%d\n", strcmp(arg1, "RQ"));

			break;
		}
		case -5: //Case of RL
			printf("releasing memory for process\n");
			break;
		case 1: //Case of Status
			printf("Partitions [Allocated memory = .... ]\n");
			break;
		default: //invalid input
		{
			printf("The value is not correct\n");
			int i = 0;
			int j = 0;
			while (i < 1000000) {
				if (mem->memory[i] != -1) {
					j++;
				}
				i++;
			}
			printf("j: %d\n", j);
			printf("%s\n", arg1);
			printf("%d\n", strcmp(arg1, "RQ"));
			break;
		}
		}

	}

}

