#include "myGlobal.h"
int sharedID, semID;
unsigned int randomSecond, randomNanoSecond;
int main()
{
	
	//number of resources
	//number used
	//shareable - boolean

	key_t key = ftok(sharedKey, sharedInt);
	sharedID = shmget(key, 62 * sizeof(unsigned int), 0666|IPC_CREAT);
	semID = semget(1963, 1, IPC_CREAT|0666);
	semctl(semID, 0, SETVAL, 1);
	randomSecond = 0;
	randomNanoSecond = 0;
	SetSemID(semID);
	SetSharedID(sharedID);	
	printf("semID %d sharedID %d\n", semID, sharedID);
	arr = (int*) shmat(sharedID, NULL, 0);
	arr[0] = 0;
	arr[1] = 0;
	int i = 0;
	for(i = 2; i < 20; i = i + 3)
	{
		arr[i] = randomResourceNumber();
		arr[i+1] = 0;
		arr[i+2] = 0;

	
	}
	

	for(i = 0; i < 5; i++)
	{
		int index = randomShareable()*3 + 2;
		index = index + 2;
		arr[index] = 1;
		

	}

	shmdt(arr);
	
		
	StartProgram();


        if(shmctl(sharedID, IPC_RMID, NULL)< 0)
                fprintf(stderr, "Shared memory was not deallocated: remove it manually\n");

        remove(sharedIDFile);
	remove(semIDFile);
            
        if((semctl(semID, 0, IPC_RMID)) < 0)
        	fprintf(stderr, "Semaphore array was not deallocated: remove it manually\n");


	return 0;
}
void StartProgram()
{

	int maxAllowed = 20;
	int maxAtOneTime = 18;
	int childCreated = 0;
	int aliveChilds = 0;
	char ** argToPass = (char**)malloc(sizeof(char *) * 2);
	int i,k = 0;
	int pid,pida,status;
	int num2 = 63;
	int exitFlag = 0;
	int childWaitFlag = 0;
	for(i = 0; i < 2; i++)
		argToPass[i] = (char*)malloc(sizeof(int) * 8);
	i = 0;
	RandomTime();
	do
	{
		
		if(maxAtOneTime > aliveChilds && childCreated < maxAllowed && CheckTime() == 1)
		{
			pid = fork();
			if(pid < 0)	
			{
				perror("Creation of child process was unsuccessful\n");
			}			
			else if(pid == 0)
			{
				char * st;
				st = (char*)malloc(sizeof(char) * 8);
				sprintf(st, "%d", pid);
				strcpy(argToPass[0], st);
				sprintf(st, "%d", num2);
				strcpy(argToPass[1], st);
				execv("child", argToPass);
				free(st);
				exit(0);

			}
			else if(pid > 0)
			{
				sharedID = GetSharedIDFromFile();
				arr = (int*)shmat(sharedID, NULL, 0);
				printf("Child created %d at time %d\n", pid, arr[0]);
				shmdt(arr);
				aliveChilds++;
				childCreated++;
				RandomTime();
			}
		
		}
		for(k = 0; k < aliveChilds; k++)
		{
			pida = waitpid(pida, &status, WNOHANG);
			if(pida == -1)
			{

			}
			else if(pida == 0)
			{
	
			}
			else if(pida > 0)
			{
				sharedID = GetSharedIDFromFile();
				arr = (int*)shmat(sharedID, NULL, 0);
				printf("Parent: child %d completed at time %d\n", pida, arr[0]);
				shmdt(arr);
				aliveChilds--;	
			}
		}
		UpdateTime();
	
	
	if(childCreated == maxAllowed)
	{
		if(aliveChilds == 0)
		{
			exitFlag = 1;
		}

	}
	
	}while(exitFlag == 0);
	free(argToPass);

}


void SetTime()

{
	sharedID = GetSharedIDFromFile();
	int * arr = (int*)shmat(sharedID, NULL, 0);
	arr[0] = 0;
	arr[1] = 0;
	shmdt(arr);
}





int GetSemIDFromFile()
{
	FILE * fptr;
	int num = 0;
	fptr = fopen(semIDFile, "r");
	fscanf(fptr, "%d", &num);
	fclose(fptr);
	return num;
}


int GetSharedIDFromFile()
{
	FILE * fptr;
	int num = 0;
	fptr = fopen(sharedIDFile, "r");
	fscanf(fptr, "%d", &num);
	fclose(fptr);
	return num;
}
void SetSharedID(int num)
{
	FILE * fptr;
	if((fptr = fopen(sharedIDFile, "w")) == NULL)
	{
		perror("file problems\n");
		exit(1);
	}	
	fprintf(fptr, "%d", num);
	fclose(fptr);
}

void SetSemID(int num)
{
	FILE * fptr;
	if((fptr = fopen(semIDFile, "w")) == NULL)
	{
		perror("semaphore file problems\n");
		exit(1);
	}
	fprintf(fptr, "%d", num);
	fclose(fptr);
}




void UpdateTime()
{
	struct sembuf sbuf;
	
	sbuf.sem_num = 0;
	sbuf.sem_op = -1;
	sbuf.sem_flg = 0;
	semID = GetSemIDFromFile();
	semop(semID, &sbuf, 1);
	sharedID = GetSharedIDFromFile();
	arr = (int*)shmat(sharedID, NULL, 0);
	
	arr[1] = arr[1] + 10000000;
	if(arr[1] > 100000000)
	{
		arr[0] = arr[0] + 1;
		arr[1] = 0;

	}
	
	shmdt(arr);
	sbuf.sem_num = 0;
	sbuf.sem_op = 1;
	sbuf.sem_flg = 0;
	semop(semID, &sbuf, 1);
}

int CheckTime()
{
	int flag = 0;
	sharedID = GetSharedIDFromFile();
	arr = (int*)shmat(sharedID, NULL, 0);
	if(randomSecond < arr[0])
	{
		flag = 1;
	}
	shmdt(arr);
	return flag;
	
}
int RandomeShareable()
{
	struct timeval t1;
	gettimeofday(&t1, NULL);
	srand(t1.tv_usec * t1.tv_sec);
	return ((rand() % 21));

}

int RandomResourceNumber()
{

	struct timeval t1;
	gettimeofday(&t1, NULL);
	srand(t1.tv_usec * t1.tv_sec);
	
	return ((rand() % 10) + 1);
}

void RandomTime()
{
	struct timeval t1;
	gettimeofday(&t1, NULL);
	
	srand(t1.tv_usec * t1.tv_sec);
	randomSecond = ((rand() % 3)+1);

	sharedID = GetSharedIDFromFile();
	arr = (int*)shmat(sharedID, NULL, 0);
	randomSecond = randomSecond + arr[0];
		
}
