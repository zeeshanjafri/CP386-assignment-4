#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>


int customers = 5;
static const int resources = 4;


//typedef struct {
//    int alloc[4];
//    int need[4];
//    int avail[4];
//    int process;
//
//} bankersInfo;

void needs(int max[customers][resources],int alloc[customers][resources],int need[customers][resources]){

	for (int y = 0;y < customers;y++){
			for (int x = 0; x < resources;x++){
				need[y][x] = max[y][x] - alloc[y][x];

			}
		}
	return;

}

void isSafe(int alloc[customers][resources],int need[customers][resources],int avail[resources],int seq[customers]){
	 int k,i,j,y;
	 int f[customers],ind = 0;

	for (k = 0; k < customers; k++) {  //initializing an array to keep track of executed processes
	        f[k] = 0;
	    }


	 while (ind < customers) { //Till we find a sequence
	    for (i = 0; i < customers; i++) { // loop through processes
	             if (f[i] == 0) {         //it has not been run yet

	                 int flag = 0;			//flag indicating if it failed or not
	                 for (j = 0; j < resources; j++) {  // loop through all its resources
	                     if (need[i][j] > avail[j]){
	                         flag = 1;					// indicating it failed so break out
	                          break;
	                     }
	                 }

	                 if (flag == 0) {			// it didnt fail so add to sequence
	                     seq[ind++] = i;   //increase index of sequence and assign to process number
	                     for (y = 0; y < resources; y++){  //loop through to add resources to available
	                    	 avail[y] += alloc[i][y];
	                     }
	                     f[i] = 1;  	//closing the process as it is done executing
	                 }
	             }
	         }
	     }





}


int findrequest(int need[resources],int avail[resources],int request[resources]){

	for(int i = 0; i< resources;i++){
		if(request[i] > need[i] && request[i] > avail[i]){
			printf("The following system is not safe");
			return 0;
		}

	}
	for(int i = 0; i < resources;i++){
		avail[i] -= request[i];

	}

	printf("State is safe, and request is satisfied");

	return 1;

}

void *thread_handler(void *param){



	printf("      Thread has started\n");
	printf("      Thread has finished\n");
	printf("      Thread is releasing resources\n");
	return NULL;
}

int main(int argc,  char **argv){
	int y,x,i;
	if(argc < 2){
			printf("No args");
			return 0;
		}
	i = 1;
	int avail[4];
	while(i < argc){
		if(isdigit(*argv[i])){
			avail[i-1] = atoi(argv[i]);


		i++;
		}
	}



	int max_res[5][4] = {
			{6, 4, 7, 3},
			{4, 2, 3, 2},
			{2, 5, 3, 3},
			{6, 3, 3, 2},
			{5, 5, 7, 2}};

	int alloc[customers][resources];
	int need[customers][resources];



	printf("Number of Customers: %d\n", customers);
	printf("Currently Available resources: \n");

	for(y = 0;y<resources;y++){
		printf("%d ",avail[y]);
	}
	printf("\n");
	printf("Maximum resources from file\n");

	for (y = 0;y < customers;y++){
		for (x = 0; x < resources;x++){
			printf("%d ",max_res[y][x]);
			alloc[y][x] = 0;
			need[y][x] = 0;
		}
		printf("\n");
	}

	needs(max_res,alloc,need); // initializes needs array;


	printf("\nEnter Command: ");
	char input[20];
	fgets(input,20,stdin);
	input[ strlen(input)-1] ='\0';


	while(strcmp(input,"EXIT") != 0){		//Check if user enter EXIT
		char key[3];
		strncpy(key, input, 2);
		key[2] = '\0';						//Check first two characters

		char numbers[10];
		memcpy( numbers, &input[3], strlen(input));

		if (strcmp(key,"RQ") == 0){ // check if request can be satisfied
			char numbers[10];
			int copy_alloc[resources];
			int copy_need[resources];
			int request[resources+1];
			memcpy( numbers, &input[3], strlen(input));
			int process,curr;

			if(isdigit(numbers[0])){

				process = atoi(&numbers[0]);

				curr = 0;
				if (process < customers){


				for(i = 1; i < strlen(numbers);i++){
					if(isdigit(numbers[i])){
						copy_alloc[curr] = alloc[process][curr] + atoi(&numbers[i]);
						request[curr] = atoi(&numbers[i]);
						curr++;
						if (curr >= resources){
							break;
						}
					}
			}
			int safe;

			for (i= 0 ; i < resources;i++){
				copy_need[i] = max_res[process][i]  - copy_alloc[i];
			}

			safe = findrequest(copy_need,avail,request);

			if (safe == 1){
				for (i= 0 ; i < resources;i++){
					need[process][i] = copy_need[i];		//updating arrays since they are safe
					alloc[process][i] = copy_alloc[i];

					}
			}
				printf("\nEnter Command: ");
				fgets(input,20,stdin);
				input[ strlen(input)-1] ='\0';
				continue;

				}


				}else{
					printf("Incorrect format");
					printf("\nEnter Command: ");
					fgets(input,20,stdin);
					input[ strlen(input)-1] ='\0';
					continue;
				}


		}if (strcmp(key,"RL") == 0){ // check if request can be satisfied
			char numbers[10];
			int request[resources+1];
			memcpy( numbers, &input[3], strlen(input));
			int process,curr;

			if(isdigit(numbers[0])){

			process = atoi(&numbers[0]);

			curr = 0;


			for(i = 1; i < strlen(numbers);i++){
				if(isdigit(numbers[i])){
					alloc[process][curr] -= atoi(&numbers[i]);
					request[curr] = atoi(&numbers[i]);
					curr++;
					if (curr >= resources){
						break;
							}
						}
				}

			needs(max_res,alloc,need); //update need array
			for(int i = 0; i < resources;i++){
				avail[i] += request[i];

			}
			printf("Successfully released.");

				printf("\nEnter Command: ");
				fgets(input,20,stdin);
				input[ strlen(input)-1] ='\0';
				continue;




				}else{
					printf("Incorrect format");
					printf("\nEnter Command: ");
					fgets(input,20,stdin);
					input[ strlen(input)-1] ='\0';
					continue;
				}


		}else if (strcmp(input,"RUN") == 0){
			int sequence[customers];
			int copy_avail[resources];

			for (i = 0; i<resources;i++){
				copy_avail[i] = avail[i];
			}
			isSafe(alloc,need,avail,sequence);

			 /* the thread identifiers */
			printf("Safe Sequence is: ");
			for (i = 0;i<customers;i++){
			printf("%d ",sequence[i]);
			}
			printf("\n");
			for (i = 0;i<customers;i++){
				pthread_t tid1;


				/* create the thread */
				printf("- - > Customer/Thread %d\n",sequence[i]);
				printf("      Allocated resources: ");

				for (y=0 ;y < resources;y++){
					printf("%d ",alloc[sequence[i]][y]);


				}
				printf("\n");

				printf("      Needed: ");
				for (y=0 ;y < resources;y++){
						printf("%d ",need[sequence[i]][y]);
				}
				printf("\n");

				printf("      Available: ");
				for (y=0 ;y < resources;y++){
						printf("%d ",copy_avail[y]);
				}
				printf("\n");


				pthread_create(&tid1, NULL, thread_handler,NULL );
				pthread_join(tid1, NULL);
				printf("      New Available: ");

				for (x = 0; x<resources;x++){
					copy_avail[x] += alloc[sequence[i]][x];
					printf("%d ",copy_avail[x]);
					alloc[sequence[i]][x] = 0;
				}
				printf("\n");



			}
			needs(max_res,alloc,need); //update need array

			printf("\nEnter Command: ");
			fgets(input,20,stdin);
			input[ strlen(input)-1] ='\0';
			continue;

		}else if(strcmp(input,"STATUS") == 0){
			printf("Available Resources: \n");
			for(i = 0; i < resources;i++){
				printf("%d ",avail[i]);
			}


			printf("\nMaximum Resources: \n");
			for (y = 0;y < customers;y++){
					for (x = 0; x < resources;x++){
						printf("%d ",max_res[y][x]);
					}
					printf("\n");
				}

			printf("Allocated Resources: \n");
			for (y = 0;y < customers;y++){
				for (x = 0; x < resources;x++){
					printf("%d ",alloc[y][x]);
				}
				printf("\n");
			}
			printf("Need Resources: \n");
			for (y = 0;y < customers;y++){
				for (x = 0; x < resources;x++){
					printf("%d ", need[y][x]);
				}
				printf("\n");
			}

			printf("\nEnter Command: ");
			fgets(input,20,stdin);
			input[ strlen(input)-1] ='\0';
			continue;


		}else{
		printf("\nInvalid input, use one of RQ, RL, Status, Run, Exit");
		printf("\nEnter Command: ");
		fgets(input,20,stdin);
		input[ strlen(input)-1] ='\0';
		}

	}
	pthread_exit(NULL);
	return 0;
}
