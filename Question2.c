#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>

#define MAXP 100

/* global variable declaration */
//int size = argv[0];  //get this as cmd line arg
int blocksize[1000];  //will contain the sizes of different processes
int alloc;

struct Process {
	char name[2];
	int size;
	int start;
	int end;
} Process;

void print_process(struct Process *process) {
	printf("Address [%d:%d] Process %s\n", process->start, process->end,
			process->name);
	return;
}

void release_process(struct Process *processes[],
		struct Process *process_full[], char *process) {

	for (int i = 0; i < MAXP; i++) {
		if (process_full[i] != NULL
				&& strcmp(process_full[i]->name, process) == 0) {

			alloc -= process_full[i]->size;
			for (int j = 0; j < MAXP; j++) {
				if (processes[j] == NULL) {
					struct Process *p;
					p = malloc(sizeof(Process));

					p->start = process_full[i]->start;
					p->size = process_full[i]->size;
					p->end = p->start + p->size - 1;
					processes[j] = p;
					process_full[i] = NULL;
					printf("releasing memory for process %s\n", process);
					printf("Successfully released memory for process %s.\n",
							process);
					break;

				} else if (processes[j]->start == process_full[i]->end + 1) {
//					printf("HERE");
					processes[j]->start = process_full[i]->start;
					processes[j]->size = processes[j]->size
							+ process_full[i]->size;
					processes[j]->end = processes[j]->start + processes[j]->size
							- 1;
					process_full[i] = NULL;
					printf("releasing memory for process %s\n", process);
					printf("Successfully released memory for process %s.\n",
							process);
					break;
				}
			}
			break;

		}
	}

}

int insert_plist(struct Process *processes[], struct Process *process_full[],
		char *process, int size, int index) {

	for (int i = 0; i < MAXP; i++) {
		if (process_full[i] != NULL
				&& strcmp(process_full[i]->name, process) == 0) {
			printf("Process has already claimed memory.\n");
			return 1;
		}

		if (process_full[i] == NULL) {
			struct Process *p;
			p = malloc(sizeof(Process));

			strcpy(p->name, process);
			p->start = processes[index]->start;
			p->size = size;
			p->end = p->start + size - 1;
			process_full[i] = p;
			break;
		}
	}

	processes[index]->start += size;

	processes[index]->size -= size;
	return 0;
}

//checks the Process list for best-fitting hole
//returns 0 or index
int best_fit(struct Process *processes[], int size) {
	int i;
	int bestFit = -1;
	for (i = 0; i < MAXP; i++) {
		if (processes[i] == NULL) {
			break;
		} else if (processes[i]->size > size) {
			if (bestFit == -1
					|| processes[bestFit]->size > processes[i]->size) {
				bestFit = i;
			}

		}

	}
	return bestFit;
}

int main(int argc, char *argv[]) {

	int size = atoi(argv[1]);
	//int memory[size];
	printf("Allocated %d bytes of memory\n", size);

	int i = 0;

	//initialize the RAM to work with

	//initial list of processes with all NULLs
	struct Process *process_list[100];
	struct Process *process_full[100];
	for (i = 0; i < 100; i++) {  //initialize memory block with NULL's
		process_list[i] = NULL;
		process_full[i] = NULL;

	}
	struct Process *process;
	process = malloc(sizeof(Process));		//initializes first block of memory

	process->start = 0;
	process->size = size;
	process->end = size - 1;
	process_list[0] = process;
	//create block list
	//to have format Address [200000:549999] Process P1

	char input[40];
	char str1[15];
	strcpy(str1, "Exit");

//	char *token;

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
//		char *arg4 = NULL;

		if (pch != NULL) { //extract args from successive input
			arg1 = pch;
			pch = strtok(NULL, " ,.-");
			//	printf("%s\n", arg1);
			if (pch != NULL) {
				arg2 = pch;
				pch = strtok(NULL, " ");
				arg2[2] = '\0';

			}
			if (pch != NULL) {
				arg3 = pch;
				pch = strtok(NULL, " ");
//				printf("%s\n", arg3);
			}
//			if (pch != NULL) {
//				arg4 = pch;
//				pch = strtok(NULL, " ");
//				//	printf("%s\n", arg4);
//			}
		} //grab 4 arguments from command line for requests

		int case_ = (strcmp(arg1, "RQ")); //	printf("%d\n",strcmp(arg1,"RQ")); (to get switch case value)
		if (case_ == 0) /* Case of "RQ" */
		{

			int index = best_fit(process_list, atoi(arg3));
			if (index == -1) {
				printf("No hole of sufficient size.\n");
			} else {

				int check = 0;

				if (check == 0) {
					int empty = 1;
					empty = insert_plist(process_list, process_full, arg2,
							atoi(arg3), index);
					if (empty == 0) {
						printf("Successfully allocated %d to process %s.\n",
								atoi(arg3), arg2);
						alloc += atoi(arg3);
					}

				}

			}

			//break;
		}
		if (case_ == -5) { //Case of RL
			release_process(process_list, process_full, arg2);

		//	break;
		}
		if (case_ == 1) { //Case of Status
			printf("Partitions [Allocated memory = %d]\n", alloc);

			for (int i = 0; i < MAXP; i++) {
				if (process_full[i] != NULL) {
					printf("Address [%d:%d] Process %s\n",
							process_full[i]->start, process_full[i]->end,
							process_full[i]->name);
				}
			}

			printf("\nHoles [Free memory = %d]:\n", size - alloc);

			for (int i = 0; i < MAXP; i++) {
				if (process_list[i] != NULL) {
					printf("Address [%d:%d] len = %d\n", process_list[i]->start,
							process_list[i]->end, process_list[i]->size);
				}
			}
			//break;
		}
		//invalid input
		if (case_ != 0 && case_ != 1 && case_ != -5) {
			printf("The value is not correct\n");
//			int i = 0;
//			int j = 0;
//			while (i < 1000000) {
//				if (mem->memory[i] != -1) {
//					j++;
//				}
//				i++;
//			}
//			printf("j: %d\n", j);
//			printf("%s\n", arg1);
//			printf("%d\n", strcmp(arg1, "RQ"));
			//break;
		}

	}

}
