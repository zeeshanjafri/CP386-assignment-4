#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>

#define MAXP 100

/* global variable declaration */


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
					return;
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
					return;
					break;
				}
			}
			break;

		}


	}
	printf("Process not found.\n");

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
//returns -1 or index
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
	printf("Allocated %d bytes of memory\n", size);

	int i = 0;



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


	for (;;) /* input loop */
	{
		fputs("Command>", stdout);
		fflush(stdout);
		fgets(input, sizeof input, stdin);
		if (strcmp(input, str1) == 10)
			break;

		char *pch;
		pch = strtok(input, " ,.-");



		char *arg1 = NULL;
		char *arg2 = NULL;
		char *arg3 = NULL;


		if (pch != NULL) { //extract args from successive input
			arg1 = pch;
			pch = strtok(NULL, " ,.-");

			if (pch != NULL) {
				arg2 = pch;
				pch = strtok(NULL, " ");
				arg2[2] = '\0';


			}
			if (pch != NULL) {
				arg3 = pch;
				pch = strtok(NULL, " ");

			}


		} //grab 3 arguments from command line for requests




				if (strcmp(arg1, "RQ")==0) /* Case of "RQ" */
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


				}else if (strcmp(arg1, "RL")==0) { //Case of RL

					release_process(process_list, process_full, arg2);

				}else if (strcmp(arg1, "Status\n")==0) { //Case of Status
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

				}else if(strcmp(arg1, "EXIT\n")==0){
					printf("Exiting.....\n");
					return 0;
				}else{
					//invalid input
					printf("Invalid input\n");
				}


		}

	}


